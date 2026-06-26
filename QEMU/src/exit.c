void qemu_exit(int code) {
    register long x0 asm("x0") = 0x18;
    register long x1 asm("x1") = code;
    asm volatile("hlt 0xf000" : : "r"(x0), "r"(x1));
}