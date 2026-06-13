.text
.global find_two_spaces
find_two_spaces:
    stp x29, x30, [sp, #-16]!
    mov x29,sp
    mov x0,x1
    mov x1,x2
    mov x2,#0
    mov x3,#-1
    mov x4,#-1
find_two_spaces_loop:
    cmp x2,x1
    b.hs find_two_spaces_fail_check

    ldrb w5,[x0,x2]
    cmp w5,#' '
    b.ne find_two_spaces_next
    cmp x3,#-1
    b.ne find_two_spaces_second
    mov x3,x2
    b find_two_spaces_next
find_two_spaces_second:
    mov x4,x2
    b find_two_spaces_done
find_two_spaces_next:
    add x2,x2,#1
    b find_two_spaces_loop
find_two_spaces_fail_check:
    cmp x3,#-1
    b.eq find_two_spaces_fail
    cmp x4,#-1
    b.eq find_two_spaces_fail
    b find_two_spaces_done
find_two_spaces_fail:
    mov x0, #-1
    mov x1, #-1
    b find_two_spaces_return
find_two_spaces_done:
    mov x0,x3
    mov x1,x4
find_two_spaces_return:
    ldp x29, x30, [sp], #16
    ret
