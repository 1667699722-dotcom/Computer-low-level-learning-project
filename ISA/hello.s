.include "src/memory.inc"  
.data  ;数据区
.align 4  ;
.global heap
.global heap_end
heap:                  ;堆内存的开始位置
    .space HEAP_SIZE   ;开辟一块长度为HEAP_SIZE的内存空间
heap_end:              ;堆内存的结束位置

.text 
.global _main
.extern exit
.extern memory_init
.extern memory_alloc
.extern memory_free

_main:
    sub sp,sp,#32
    bl memory_init

loop:

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


    ;add x3, x3, #1
    ;cmp x3, #2
    ;b.eq do_exit
    b loop  
 
do_exit:
    sub sp,sp,#32
    b exit

    