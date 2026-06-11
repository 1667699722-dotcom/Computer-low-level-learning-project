.include "src/memory.inc"  

.data  ;数据区
.align 4  ;

hello_msg:
     .asciz "Hello from write syscall!\n"
hello_len=.-hello_msg

.text 
.extern heap           
.extern heap_end 
.extern sys_write           
.extern memory_alloc
.extern memory_free

.global test_write
.global test_memory 


test_write:
    stp x29, x30, [sp, #-16]!
    mov x0,#1
    adrp x1,hello_msg@PAGE
    add x1,x1,hello_msg@PAGEOFF
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
    