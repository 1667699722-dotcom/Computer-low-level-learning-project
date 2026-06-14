.section .text
.global _start

_start:
    ldr x0, =0x09000000
    ldr x1, =msg

print_loop:
    ldrb w2, [x1], #1
    cbz w2, halt

wait_uart:
    ldr w3, [x0, #0x18]
    tbz w3, #5, write_char
    b wait_uart

write_char:
    str w2, [x0]
    b print_loop

halt:
    wfe
    b halt

.section .rodata
msg:
    .asciz "Hello QEMU AArch64 bare metal!\n"