

#define UART0_BASE 0x09000000

volatile unsigned int *uart = (volatile unsigned int *)UART0_BASE;

void uart_putc(char c) {
    while ((uart[6] & (1 << 5)) != 0);
    uart[0] = c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}