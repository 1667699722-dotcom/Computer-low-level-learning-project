.include "src/memory.inc"  
.extern heap           
.extern heap_end 
.global memory_free

memory_free:
    ;保存寄存器，压栈
    stp x19,x20,[sp,#-64]!
    stp x21,x22,[sp,#16]
    stp x23,x24,[sp,#32]
    stp x25,x26,[sp,#48]
    ;找到当前堆头标记空闲
    cbz x0,free_done
    sub x19,x0,#8
    mov w1,#0
    str w1,[x19,#4]
    ;准备遍历找前一块
    adrp x20,heap@PAGE
    add x20,x20,heap@PAGEOFF
    adrp x21,heap_end@PAGE
    add x21,x21,heap_end@PAGEOFF
    mov x22,x20
    mov x23,#0
find_prev_loop:
    cmp x22,x19
    b.hs check_prev
    cmp x22,x21
    b.hs check_prev

    mov x23,x22
    ldr w24,[x22]
    add x22,x22,#8
    add x22,x22,x24
    b find_prev_loop
check_prev:
    cbz x23,free_done
    ldr w25,[x23,#4]
    cmp w25,#0
    b.ne free_done

    ldr w24,[x23]
    ldr w26,[x19]
    add w24,w24,w26
    add w24,w24,#8
    str w24,[x23]
free_done:
    ldp x25,x26,[sp,#48]
    ldp x23,x24,[sp,#32]
    ldp x21,x22,[sp,#16]
    ldp x19,x20,[sp],#64
free_done_flag:
    ret
