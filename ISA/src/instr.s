.include "src/memory.inc"  

.data  ;数据区
.align 4  ;
.extern heap           
.extern heap_end 
.equ MAX_INSTR,64
instr_addrs:.space MAX_INSTR*8
count_instr:.word 0
.text
.extern memory_init
.extern memory_alloc
.extern memory_free

.global store_instr_addrs
.global get_instr_addrs

store_instr_addrs:


    stp x19,x20,[sp,#-96]!
    stp x21,x22,[sp,#16]
    stp x23,x24,[sp,#32]
    stp x25,x26,[sp,#48]
    stp x27,x28,[sp,#64]
    stp x29,x30,[sp,#80]

    mov x19,x0 ;第一个空格位置   
    mov x20,x1 ;第二个空格位置
    mov x21,x2 ;返回地址
    mov x22,x3 ;返回长度

    
    adrp x25, count_instr@PAGE
    add x25, x25, count_instr@PAGEOFF
    ldr w26, [x25]            ; 读当前计数到 w1
    cmp w26, #MAX_INSTR      ; 检查是否满了
    b.hs skip_store

    mov x0,#32
    bl memory_alloc
    mov x24,x0 ;内存分配数据区地址

    adrp x23, instr_addrs@PAGE          
    add x23, x23, instr_addrs@PAGEOFF ;存入instr_addrs指针

parse_instr1:
    mov x0,x19
    add x0,x0,#1
    bl memory_alloc
    mov x27,x0
    mov x28,#0
extract_byte_loop1:
    cmp x28,x19
    b.ge extract_done1
    ldrb w29,[x21,x28]
    strb w29,[x27,x28]
    add x28,x28,#1
    b extract_byte_loop1
extract_done1:
    strb wzr,[x27,x28]
    str x27,[x24]

parse_instr2:
    sub x0,x20,x19
    add x0,x0,#1
    bl memory_alloc
    mov x27,x0
    mov x28,#0         ; 新字符串索引从 0 开始
    add x29,x19,#1     ; 原字符串索引从 x19+1 开始
extract_byte_loop2:
    cmp x29,x20
    b.ge extract_done2
    ldrb w30,[x21,x29]
    strb w30,[x27,x28]
    add x28,x28,#1
    add x29,x29,#1
    b extract_byte_loop2
extract_done2:
    strb wzr,[x27,x28]
    str x27,[x24,#8]

parse_instr3:
    sub x0,x22,x20
    add x0,x0,#1
    bl memory_alloc
    mov x27,x0
    mov x28,#0         ; 新字符串索引从 0 开始
    add x29,x20,#1     ; 原字符串索引从 x20+1 开始
extract_byte_loop3:
    cmp x29,x22
    b.ge extract_done3
    ldrb w30,[x21,x29]
    strb w30,[x27,x28]
    add x28,x28,#1
    add x29,x29,#1
    b extract_byte_loop3
extract_done3:
    strb wzr,[x27,x28]
    str x27,[x24,#16]  ; 存第二个数字指针，把原来的 str x21 覆盖掉，或者你自己选位置



    str x27,[x24,#24]


    str x24,[x23,x26,lsl #3]

    mov w0,w26
    add w26,w26,#1
    str w26,[x25];指令序列计数自增

instr_addrs_done:
    ldp x29,x30,[sp,#80]
    ldp x27,x28,[sp,#64]
    ldp x25,x26,[sp,#48]
    ldp x23,x24,[sp,#32]
    ldp x21,x22,[sp,#16]
    ldp x19,x20,[sp],#96
    ret
skip_store:
    mov x0,#-1
    b instr_addrs_done
get_instr_addrs:
    stp x29,x30,[sp,#-32]!
    stp x27,x28,[sp,#16]
    mov x28,x0
    adrp x29, instr_addrs@PAGE          
    add x29, x29, instr_addrs@PAGEOFF ;存入instr_addrs指针
    ldr x0, [x29, x28, lsl #3]
get_instr_addrs_done:
    ldp x27,x28,[sp,#16]   
    ldp x29,x30,[sp],#32
    ret
    


;atoi：

;execute_op：