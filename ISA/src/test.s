.include "src/memory.inc"  

.data  ;数据区
.align 4  ;
.global hello_msg
.global hello_len
hello_msg:
     .asciz "Hello from write syscall!\n"
hello_len=.-hello_msg

.text 
.extern heap           
.extern heap_end 
.extern sys_write  
.extern sys_read           
.extern memory_alloc
.extern memory_free
.extern input_buf
.global test_write
.global test_memory 
.global test_read
.global test_write_read
.global test_write_memory

test_read:
    stp x29, x30, [sp, #-16]!
    mov x0,#0
    adrp x1, input_buf@PAGE
    add x1, x1, input_buf@PAGEOFF
    mov x2,#256
    bl sys_read
    ldp x29, x30, [sp], #16
    ret

test_write_read:
    stp x29, x30, [sp, #-16]!
    mov x0,#1
    adrp x1,input_buf@PAGE
    add x1,x1,input_buf@PAGEOFF
    mov x2,#256
    bl sys_write
    ldp x29, x30, [sp], #16
    ret

test_write_memory:
    stp x29, x30, [sp, #-16]!
    mov x1,x0
    mov x0,#1
    mov x2,#256
    bl sys_write
    ldp x29, x30, [sp], #16
    ret

test_write:
    stp x29, x30, [sp, #-16]!
    mov x0,#1
    adrp x1,hello_msg@PAGE
    add x1,x1,hello_msg@PAGEOFF

    mov w29, #'H'
    strb w29, [x1, #0]  ; 第 0 个字符
    mov w29, #'E'
    strb w29, [x1, #1]  ; 第 1 个字符
    mov w29, #'L'
    strb w29, [x1, #2]  ; 第 2 个字符
    mov w29, #'L'
    strb w29, [x1, #3]  ; 第 3 个字符
    mov w29, #'O'
    strb w29, [x1, #4]  ; 第 4 个字符

    mov x2,#hello_len
    bl sys_write
    ldp x29, x30, [sp], #16
    ret

test_memory:
    stp x29, x30, [sp, #-16]!
    sub sp,sp,#32

    mov x0,#8
    bl memory_alloc
    str x0,[sp,#0]
    ldr w4, =0x11111111
    ldr w5, =0x11111111
    str w4, [x0]
    str w5, [x0,#4]

    mov x0,#8
    bl memory_alloc
    str x0,[sp,#8]
    ldr w4, =0x22222222
    ldr w5, =0x22222222
    str w4, [x0]
    str w5, [x0,#4]

    mov x0,#8
    bl memory_alloc
    str x0,[sp,#16]
    ldr w4, =0x33333333
    ldr w5, =0x33333333
    str w4, [x0]
    str w5, [x0,#4]

    ldr x0,[sp,#16]
    bl memory_free
    str xzr, [sp,#16]
    
    ldr x0,[sp,#8]
    bl memory_free
    str xzr, [sp,#8]
    
    add sp,sp,#32
    ldp x29, x30, [sp], #16
    ret
    