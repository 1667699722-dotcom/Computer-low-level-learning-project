#include "include/uart.h"
#include "include/exit.h"



void kernel_main() {
    uart_puts("Hello from C kernel!\n");



    qemu_exit(0);
}