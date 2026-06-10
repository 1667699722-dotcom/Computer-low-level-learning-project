.global exit
.global sys_write
.global sys_read
.align 4
exit:
    movz x16, #0x1
    movk x16, #0x200, lsl #16
    svc #0x80
sys_write:
    movz x16, #0x4
    movk x16, #0x200, lsl #16
    svc #0x80
    ret
sys_read:
    movz x16, #0x3
    movk x16, #0x200, lsl #16
    svc #0x80
    ret