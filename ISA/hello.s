.include "src/memory.inc"  
.data
.align 4 
.global heap
.global heap_end
heap:
    .space HEAP_SIZE
heap_end: 

.text 
.global _main
.extern exit
.extern memory_init
.extern memory_alloc

_main:
    mov x3, #0 
    bl memory_init

loop:
    mov x0,#100
    bl memory_alloc
    add x3, x3, #1
    cmp x3, #2
    b.eq do_exit
    b loop  
    
do_exit:
    b exit

    