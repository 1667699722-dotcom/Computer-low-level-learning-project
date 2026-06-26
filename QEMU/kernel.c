#include "src/include/uart.h"
#include "src/include/exit.h"

char buf[128];

void kernel_main() {
    uart_init();
    uart_puts("Hello from C kernel!\n");

    char buf[128];
    uart_gets(buf, sizeof(buf));
    uart_puts(buf);
    uart_puts("\n");

    qemu_exit(0);
}