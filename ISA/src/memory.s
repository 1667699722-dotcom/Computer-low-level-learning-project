.include "src/memory.inc"  
.extern heap           
.extern heap_end    
.global memory_init
.global memory_alloc
memory_init:
    adrp x0, heap@PAGE
    add x0, x0, heap@PAGEOFF
    mov x1, #HEAP_SIZE
    sub x1, x1, #8
    str w1, [x0]
    mov w2, #0
    str w2, [x0,#4]
    ret
memory_alloc:
    stp x19,x20,[sp,#-32]!
    stp x21,x22,[sp,#16]
    mov x19,x0
    adrp x20, heap@PAGE          
    add x20, x20, heap@PAGEOFF
    adrp x21, heap_end@PAGE      
    add x21, x21, heap_end@PAGEOFF
alloc_loop:
    cmp x20,x21
    b.ge alloc_fail

    ldr w22, [x20]    ;size of block    
    ldr w23, [x20, #4]  ;flag of block

    cmp w23, #1               
    b.eq next_block 
    cmp w22, w19             
    b.lt next_block  
    
    mov w0,#1
    str w0,[x20,#4]
    sub x1,x22,x19
    cmp x1,#8
    b.le no_split
    add x2,x20,#8
    add x2,x2,x19
    sub x3,x1,#8
    str w3,[x2]
    mov w4,#0
    str w4,[x2,#4]

    str w19,[x20]
no_split:
    add x0,x20,#8
    b alloc_done
next_block:
    add x20, x20, #8          
    add x20, x20, x22        
    b alloc_loop
alloc_fail:
    mov x0,#0
alloc_done:
    ldp x21, x22, [sp, #16]   
    ldp x19, x20, [sp], #32
    ret