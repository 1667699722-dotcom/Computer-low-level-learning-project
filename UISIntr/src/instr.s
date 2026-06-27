.include "src/memory.inc"  

.data  ; 数据段
.align 4  ; 4字节对齐
.extern heap           
.extern heap_end 
.equ MAX_INSTR,64       ; 最多支持 64 条指令
instr_addrs:.space MAX_INSTR*8  ; 指令地址数组：每个元素 8 字节指针
count_instr:.word 0            ; 当前已存储的指令数量
.text
.extern memory_init
.extern memory_alloc
.extern memory_free

.global store_instr_addrs
.global get_instr_addrs
.extern atoi
.extern itoa
; ============================================
; store_instr_addrs: 存储并解析一条指令
; 参数:
;   x0 = 第一个空格位置
;   x1 = 第二个空格位置
;   x2 = 指令字符串首地址
;   x3 = 指令字符串长度
; 返回:
;   x0 = 指令索引（成功）或 -1（失败）
; ============================================
store_instr_addrs:

    ; 保存被调用者保存的寄存器（符合 ARM64 AAPCS）
    stp x19,x20,[sp,#-96]!   ; 预索引：sp -= 96，存储 x19,x20
    stp x21,x22,[sp,#16]     ; 存储 x21,x22
    stp x23,x24,[sp,#32]     ; 存储 x23,x24
    stp x25,x26,[sp,#48]     ; 存储 x25,x26
    stp x27,x28,[sp,#64]     ; 存储 x27,x28
    stp x29,x30,[sp,#80]     ; 存储 x29,x30

    ; 保存输入参数到寄存器
    mov x19,x0                ; x19 = 第一个空格位置   
    mov x20,x1                ; x20 = 第二个空格位置
    mov x21,x2                ; x21 = 指令字符串首地址
    mov x22,x3                ; x22 = 指令字符串长度

    ; 检查指令数量是否已满
    adrp x25, count_instr@PAGE
    add x25, x25, count_instr@PAGEOFF  ; x25 = count_instr 的地址
    ldr w26, [x25]            ; 读当前计数到 w26
    cmp w26, #MAX_INSTR      ; 检查是否满了
    b.hs skip_store           ; 如果满了，跳过存储

    ; 分配 32 字节的内存用于存储指令信息结构体
    mov x0,#32
    bl memory_alloc
    mov x24,x0                ; x24 = 分配的结构体地址

    ; 获取 instr_addrs 数组的基地址
    adrp x23, instr_addrs@PAGE          
    add x23, x23, instr_addrs@PAGEOFF  ; x23 = instr_addrs 数组基地址

; ============================================
; parse_instr1: 解析第一部分（开头到第一个空格）
; ============================================
parse_instr1:
    ; 分配内存：大小 = 第一个空格位置 + 1（\0）
    mov x0,x19
    add x0,x0,#1
    bl memory_alloc
    mov x27,x0                ; x27 = 新分配的字符串地址
    mov x28,#0                ; x28 = 新字符串的索引

; 逐字节复制循环
extract_byte_loop1:
    cmp x28,x19               ; 检查是否到达第一个空格
    b.ge extract_done1        ; 如果到达，结束
    ldrb w29,[x21,x28]        ; 从原字符串读取一个字节
    strb w29,[x27,x28]        ; 写入新字符串
    add x28,x28,#1            ; 索引 +1
    b extract_byte_loop1      ; 继续循环

extract_done1:
    strb wzr,[x27,x28]        ; 写入字符串结束符 '\0'
    str x27,[x24]             ; 保存第一部分指针到结构体偏移 0

; ============================================
; parse_instr2: 解析第二部分（第一个空格到第二个空格）
; ============================================
parse_instr2:
    ; 分配内存：大小 = (第二个空格 - 第一个空格) + 1
    sub x0,x20,x19
    add x0,x0,#1
    bl memory_alloc
    mov x27,x0                ; x27 = 新字符串地址
    mov x28,#0                ; x28 = 新字符串索引
    add x29,x19,#1            ; x29 = 原字符串起始位置（第一个空格后）

; 逐字节复制循环
extract_byte_loop2:
    cmp x29,x20               ; 检查是否到达第二个空格
    b.ge extract_done2        ; 如果到达，结束
    ldrb w30,[x21,x29]        ; 从原字符串读取一个字节
    strb w30,[x27,x28]        ; 写入新字符串
    add x28,x28,#1            ; 新索引 +1
    add x29,x29,#1            ; 原索引 +1
    b extract_byte_loop2      ; 继续循环

extract_done2:
    strb wzr,[x27,x28]        ; 写入字符串结束符 '\0'
    str x27,[x24,#8]          ; 保存第二部分指针到结构体偏移 8

; ============================================
; parse_instr3: 解析第三部分（第二个空格到结尾）
; ============================================
parse_instr3:
    ; 分配内存：大小 = (字符串长度 - 第二个空格位置) + 1
    sub x0,x22,x20
    add x0,x0,#1
    bl memory_alloc
    mov x27,x0                ; x27 = 新字符串地址
    mov x28,#0                ; x28 = 新字符串索引
    add x29,x20,#1            ; x29 = 原字符串起始位置（第二个空格后）

; 逐字节复制循环
extract_byte_loop3:
    cmp x29,x22               ; 检查是否到达字符串结尾
    b.ge extract_done3        ; 如果到达，结束
    ldrb w30,[x21,x29]        ; 从原字符串读取一个字节
    strb w30,[x27,x28]        ; 写入新字符串
    add x28,x28,#1            ; 新索引 +1
    add x29,x29,#1            ; 原索引 +1
    b extract_byte_loop3      ; 继续循环

extract_done3:
    strb wzr,[x27,x28]        ; 写入字符串结束符 '\0'
    str x27,[x24,#16]         ; 保存第三部分指针到结构体偏移 16

    ; ============================================
    ; 执行运算：解析两个数字并相加
    ; ============================================
    ; 1. 解析 part2 为数字
    ldr x0, [x24, #8]        ; 加载 part2 的字符串地址
    bl atoi                  ; 调用 atoi 转换
    mov x9,x0               ; x27 = 第一个数字
    
    ; 2. 解析 part3 为数字
    ldr x0, [x24, #16]       ; 加载 part3 的字符串地址
    bl atoi                  ; 调用 atoi 转换
    
    ; 3. 两个数字相加
    add x9,x9,x0           ; x27 = 第一个数字 + 第二个数字



    
    mov x0,x9              ; 临时保存结果
    bl itoa
    mov x27,x0
    ; 保存运算结果到结构体偏移 24
    str x27,[x24,#24]

    ; 将结构体地址存入 instr_addrs 数组
    str x24,[x23,x26,lsl #3]  ; 数组基地址 + 索引*8

    ; 更新指令计数
    mov w0,w26                ; 返回当前索引
    add w26,w26,#1            ; 计数 +1
    str w26,[x25]            ; 保存新计数

instr_addrs_done:
    ; 恢复寄存器
    ldp x29,x30,[sp,#80]
    ldp x27,x28,[sp,#64]
    ldp x25,x26,[sp,#48]
    ldp x23,x24,[sp,#32]
    ldp x21,x22,[sp,#16]
    ldp x19,x20,[sp],#96      ; 后索引：恢复后 sp += 96
    ret

skip_store:
    mov x0,#-1                ; 返回 -1 表示失败
    b instr_addrs_done

; ============================================
; get_instr_addrs: 获取指定索引的指令信息地址
; 参数: x0 = 指令索引
; 返回: x0 = 指令信息结构体地址
; ============================================
get_instr_addrs:
    stp x29,x30,[sp,#-32]!
    stp x27,x28,[sp,#16]
    mov x28,x0                ; 保存索引
    
    ; 获取 instr_addrs 数组的基地址
    adrp x29, instr_addrs@PAGE          
    add x29, x29, instr_addrs@PAGEOFF  ; x29 = instr_addrs 基地址
    
    ; 计算地址并加载：基地址 + 索引*8
    ldr x0, [x29, x28, lsl #3]

get_instr_addrs_done:
    ldp x27,x28,[sp,#16]   
    ldp x29,x30,[sp],#32
    ret


;execute_op: （预留）