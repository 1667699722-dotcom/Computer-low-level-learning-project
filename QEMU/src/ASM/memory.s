.extern heap
.extern heap_end
.global memory_init
.global memory_alloc
.global memory_free
.equ HEAP_SIZE,4096
memory_init:
    ldr x0, =heap
    ldr x1, =4096
    sub x1, x1, #8
    str w1, [x0]
    mov w2, #0
    str w2, [x0, #4]
    ret

memory_alloc:
    stp x19, x20, [sp, #-96]!
    stp x21, x22, [sp, #16]
    stp x23, x24, [sp, #32]
    stp x25, x26, [sp, #48]
    stp x27, x28, [sp, #64]
    stp x29, x30, [sp, #80]
    mov x19, x0
    ldr x20, =heap
    ldr x21, =heap_end

alloc_loop:
    cmp x20, x21
    b.hs alloc_fail

    ldr w22, [x20]
    ldr w23, [x20, #4]

    cmp w23, #1
    b.eq next_block
    cmp w22, w19
    b.lt next_block

    mov w0, #1
    str w0, [x20, #4]
    sub x1, x22, x19
    cmp x1, #8
    b.le no_split
    add x2, x20, #8
    add x2, x2, x19
    sub x3, x1, #8
    str w3, [x2]
    mov w4, #0
    str w4, [x2, #4]

    str w19, [x20]

no_split:
    add x0, x20, #8
    b alloc_done

next_block:
    add x20, x20, #8
    add x20, x20, x22
    b alloc_loop

alloc_fail:
    mov x0, #0

alloc_done:
    ldp x29, x30, [sp, #80]
    ldp x27, x28, [sp, #64]
    ldp x25, x26, [sp, #48]
    ldp x23, x24, [sp, #32]
    ldp x21, x22, [sp, #16]
    ldp x19, x20, [sp], #96
    ret



memory_free:
    stp x19, x20, [sp, #-96]!
    stp x21, x22, [sp, #16]
    stp x23, x24, [sp, #32]
    stp x25, x26, [sp, #48]
    stp x27, x28, [sp, #64]
    stp x29, x30, [sp, #80]

    cbz x0, free_done
    sub x19, x0, #8
    mov w1, #0
    str w1, [x19, #4]

    ldr x20, =heap
    ldr x21, =heap_end
    mov x22, x20
    mov x23, #0

find_prev_loop:
    cmp x22, x19
    b.hs check_prev
    cmp x22, x21
    b.hs check_prev

    mov x23, x22
    ldr w24, [x22]
    add x22, x22, #8
    add x22, x22, x24
    b find_prev_loop

check_prev:
    cbz x23, check_next
    ldr w25, [x23, #4]
    cmp w25, #0
    b.ne check_next

    ldr w24, [x23]
    ldr w26, [x19]
    add w24, w24, w26
    add w24, w24, #8
    str w24, [x23]

check_next:
    ldr w26, [x19]
    add x27, x19, #8
    add x27, x27, x26

    cmp x27, x21
    b.hs free_done

    ldr w28, [x27, #4]
    cmp w28, #0
    b.ne free_done

    ldr w29, [x27]
    add w26, w26, w29
    add w26, w26, #8
    str w26, [x19]

free_done:
    ldp x29, x30, [sp, #80]
    ldp x27, x28, [sp, #64]
    ldp x25, x26, [sp, #48]
    ldp x23, x24, [sp, #32]
    ldp x21, x22, [sp, #16]
    ldp x19, x20, [sp], #96
    ret

