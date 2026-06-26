.section .text.startup
.global _start

_start:
    ldr x0, =__stack_top
    mov sp, x0

    ldr x0, =__bss_start
    ldr x1, =__bss_end
    b clear_bss_loop

clear_bss:
    str xzr, [x0], #8

clear_bss_loop:
    cmp x0, x1
    b.lt clear_bss
    
    bl kernel_main

halt:
    wfe
    b halt