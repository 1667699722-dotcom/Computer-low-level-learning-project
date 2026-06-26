#include "src/include/uart.h"
#include "src/include/exit.h"
#include "src/include/memory.h"

void kernel_main() {
    uart_init();
    memory_init();
    void *ptr1 = memory_alloc(100);
    if (ptr1) 
    {
        uart_puts("Enter text (max 99 chars): ");
        uart_gets((char *)ptr1, 100);
        uart_puts("You entered: ");
        uart_puts((char *)ptr1);
        uart_puts("\n");
    }
    else 
    {
        uart_puts("Allocation failed!\n");
    }
    memory_free(ptr1);
    uart_puts(ptr1);
    qemu_exit(0);
}