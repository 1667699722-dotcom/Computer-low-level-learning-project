#include "src/include/uart.h"
#include "src/include/exit.h"
#include "src/include/lib.h"
#include "src/include/memory.h"
#include "src/include/exceptionhandler.h"
#include "src_1/include/exceptionsolution.h"
#include "src/include/page.h"
#include "src/include/cmemory.h"
#define INPUT_BUFFER_SIZE 64
void kernel_main() {
    uart_init();
    // 初始化UART
    //uart_puts("Kernel started\n");
    // 读取当前EL
    //read_current_el();
    // 初始化内存
    memory_init();  
    
    // char *ptr1 = (char *)my_malloc(INPUT_BUFFER_SIZE*sizeof(char));
    // if (ptr1) 
    // {
    //     uart_puts("Enter text (max 63 chars): ");
    //     uart_gets((char *)ptr1, INPUT_BUFFER_SIZE*sizeof(char));
    //     uart_puts("You entered: ");
    //     uart_puts((char *)ptr1);
    //     uart_puts("\n");
    // }
    // else 
    // {
    //     uart_puts("Allocation failed!\n");
    // }

    page_init();
    // 释放内存
    //my_free(ptr1);
    //void *ptr2=page_alloc();
    //void *ptr3=page_alloc();
    //free_page(ptr2);
    //void *ptr4=page_alloc();
    //free_page(ptr3);
    //free_page(ptr4);
    cmemory_init();

    void *ptr5=cmemory_alloc(2742);
    uart_puts("cmemory_alloc address:");
    uart_put_hex((unsigned long)ptr5);
    uart_puts("\n");

    void *ptr6=cmemory_alloc(242);
    uart_puts("cmemory_alloc address:");
    uart_put_hex((unsigned long)ptr6);
    uart_puts("\n");
    
    
    void *ptr7=cmemory_alloc(2267);
    uart_puts("cmemory_alloc address:");
    uart_put_hex((unsigned long)ptr7);
    uart_puts("\n");
    
    cmemory_free(ptr5);
    cmemory_free(ptr6);
    cmemory_free(ptr7);
    

    // 导致内存泄漏
    //asm volatile(".word 0x00000000");
    // 退出内核
    //uart_puts("Kernel exit\n");
    // 退出QEMU
    qemu_exit(0);
}