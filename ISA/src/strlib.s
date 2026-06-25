
.global atoi
.global itoa
; ============================================
; atoi: 将字符串转换成数字
; 参数: x0 = 字符串地址
; 返回: x0 = 转换后的数字地址
; ============================================
atoi:
    stp x29,x30,[sp,#-32]!
    stp x19,x20,[sp,#16]
    
    mov x19,x0                ; x19 = 字符串地址
    mov x20,#0                ; x20 = result，初始化为0
    
atoi_check_prefix:
    ; 检查是否是 '#' 开头（立即数格式）
    ldrb w0,[x19]
    cmp w0,#'#'
    b.ne atoi_loop_start
    add x19,x19,#1            ; 跳过 '#'

atoi_loop_start:
    mov x0,#0
    
atoi_loop:
    ldrb w0,[x19],#1          ; 读取一个字符，x19++
    cmp w0,#'0'               ; 检查是否小于 '0'
    b.lt atoi_done
    cmp w0,#'9'               ; 检查是否大于 '9'
    b.gt atoi_done
    
    ; 是数字字符，处理：result = result * 10 + (char - '0')
    sub w0,w0,#'0'            ; w0 = 数字值
    mov x1,x20
    lsl x20,x20,#3            ; result * 8
    add x20,x20,x1,lsl #1     ; + result * 2 = result * 10
    add x20,x20,x0            ; 加当前数字
    
    b atoi_loop
    
atoi_done:
    mov x0,x20                ; 返回结果
    ldp x19,x20,[sp,#16]
    ldp x29,x30,[sp],#32
    ret
; ============================================
; atoi: 将字符串转换成数字
; 参数: x0 = 字符串地址
; 返回: x0 = 转换后的数字
; ============================================
itoa:
    stp x19,x20,[sp,#-96]!
    stp x21,x22,[sp,#16]
    stp x23,x24,[sp,#32]
    stp x25,x26,[sp,#48]
    stp x27,x28,[sp,#64]
    stp x29,x30,[sp,#80]
    mov x19,x0
    mov x20,0
    mov x21,x19
itoa_count_len:
    cmp x21,#0
    beq itoa_count_done
    mov x22,#10
    udiv x21,x21,x22
    add x20,x20,#1
    b itoa_count_len
itoa_count_done:
    cmp x20,#0
    bne itoa_count_ok
    mov x20,#1
itoa_count_ok:
    add x0,x20,#1
    bl memory_alloc
    mov x22,x0
    mov x23,x19 ;x23=要转化的数字
    mov x24,x22 ;x24=当前写入位置
    add x24,x24,x20 ;指向字符串末尾
    strb wzr,[x24]
    sub x24,x24,#1
itoa_write_loop:
    cmp x20,#0
    beq itoa_write_done
    mov x25,#10
    udiv x26,x23,x25
    msub x27,x26,x25,x23
    add w27,w27,#'0'
    strb w27,[x24],#-1
    mov x23,x26
    sub x20,x20,#1
    b itoa_write_loop
itoa_write_done:
    cmp x19,#0
    bne itoa_exit
    mov w27,#'0'
    strb w27,[x22]
itoa_exit:
    mov x0,x22                ; 返回字符串地址
    ldp x29,x30,[sp,#80]
    ldp x27,x28,[sp,#64]
    ldp x25,x26,[sp,#48]
    ldp x23,x24,[sp,#32]
    ldp x21,x22,[sp,#16]
    ldp x19,x20,[sp],#96
    ret