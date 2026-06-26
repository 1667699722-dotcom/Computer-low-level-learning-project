#include "src/include/uart.h"
#include "src/include/exit.h"
#include "src/include/lib.h"
#include "src/include/memory.h"

void kernel_main() {
    uart_init();
    memory_init();
    
    char *ptr1 = (char *)my_malloc(99*sizeof(char));
    if (ptr1) 
    {
        uart_puts("Enter text (max 99 chars): ");
        uart_gets((char *)ptr1, 99*sizeof(char));
        uart_puts("You entered: ");
        uart_puts((char *)ptr1);
        uart_puts("\n");
    }
    else 
    {
        uart_puts("Allocation failed!\n");
    }
    my_free(ptr1);
    qemu_exit(0);
}