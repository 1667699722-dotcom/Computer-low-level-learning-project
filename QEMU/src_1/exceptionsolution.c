#include "../src/include/exceptionhandler.h"
#include "../src/include/uart.h"
#include "../src/include/exit.h"
#include "../src/include/lib.h"
#include "../src/include/memory.h"
extern char exception_vector_table;

 // 读取当前异常级别 (CurrentEL)
void read_current_el() {
    unsigned long current_el;
    asm volatile("mrs %0, CurrentEL" : "=r"(current_el));
    current_el = (current_el >> 2) & 0x3;
    uart_puts("Current EL: ");
    uart_putc('0' + (char)current_el);
    uart_puts("\n");

    // 设置异常向量表
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
    uart_puts("Exception handler initialized\n");
}