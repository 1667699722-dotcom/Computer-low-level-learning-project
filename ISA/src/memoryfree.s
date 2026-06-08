.include "src/memory.inc"  
.extern heap           
.extern heap_end 
.global memory_free

memory_free:
    ;保存寄存器，压栈
    stp x19,x20,[sp,#-96]!
    stp x21,x22,[sp,#16]
    stp x23,x24,[sp,#32]
    stp x25,x26,[sp,#48]
    stp x27,x28,[sp,#64]
    stp x29,x30,[sp,#80]
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
    cbz x23,check_next
    ldr w25,[x23,#4]
    cmp w25,#0
    b.ne check_next

    ldr w24,[x23]
    ldr w26,[x19]
    add w24,w24,w26
    add w24,w24,#8
    str w24,[x23]
check_next:
    ldr w26,[x19]
    add x27,x19,#8
    add x27,x27,x26

    cmp x27,x21
    b.hs free_done
    
    ldr w28,[x27,#4]
    cmp w28,#0
    b.ne free_done

    ldr w29,[x27]
    add w26,w26,w29
    add w26,w26,#8
    str w26,[x19]
free_done:
    ldp x29,x30,[sp,#80]
    ldp x27,x28,[sp,#64]
    ldp x25,x26,[sp,#48]
    ldp x23,x24,[sp,#32]
    ldp x21,x22,[sp,#16]
    ldp x19,x20,[sp],#96
free_done_flag:
    ret
