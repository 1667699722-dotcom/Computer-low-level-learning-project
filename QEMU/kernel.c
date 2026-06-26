#include "src/include/uart.h"
#include "src/include/exit.h"
#include "src/include/lib.h"
#include "src/include/memory.h"
#include "src/include/exceptionhandler.h"

void kernel_main() {
    uart_init();
    uart_puts("Kernel started\n");

    // 读取当前异常级别 (CurrentEL)
    unsigned long current_el;
    asm volatile("mrs %0, CurrentEL" : "=r"(current_el));
    current_el = (current_el >> 2) & 0x3;
    uart_puts("Current EL: ");
    uart_putc('0' + (char)current_el);
    uart_puts("\n");

    // 设置异常向量表
    extern char exception_vector_table;
    uart_puts("Vector table addr: 0x");
    uart_put_hex((unsigned long)&exception_vector_table);
    uart_puts("\n");

    if (current_el == 2) {
        asm volatile(
            "msr VBAR_EL2, %0\n"
            "isb\n"
            : : "r"(&exception_vector_table)
        );
        uart_puts("VBAR_EL2 set\n");
    } else if (current_el == 1) {
        asm volatile(
            "msr VBAR_EL1, %0\n"
            "isb\n"
            : : "r"(&exception_vector_table)
        );
        uart_puts("VBAR_EL1 set\n");
    } else {
        uart_puts("Unknown EL, skip VBAR\n");
    }

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

    asm volatile(".word 0x00000000");
    uart_puts("Kernel exit\n");
    qemu_exit(0);
}