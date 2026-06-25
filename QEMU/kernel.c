#define UART0_BASE 0x09000000

volatile unsigned int *uart = (volatile unsigned int *)UART0_BASE;

void uart_putc(char c) {
    while ((uart[6] & (1 << 5)) != 0)
        ;
    uart[0] = c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

void qemu_exit(int code) {
    register long x0 asm("x0") = 0x18;
    register long x1 asm("x1") = code;
    asm volatile("hlt 0xf000" : : "r"(x0), "r"(x1));
}

void kernel_main() {
    uart_puts("Hello from C kernel!\n");
    qemu_exit(0);
}