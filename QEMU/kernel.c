#include "src/include/uart.h"
#include "src/include/exit.h"
#include "src/include/lib.h"
#include "src/include/memory.h"
#include "src/include/exceptionhandler.h"
#include "src_1/include/exceptionsolution.h"
#include "src/include/page.h"
#include "src/include/cmemory.h"
#include "src/include/virtio.h"
#define INPUT_BUFFER_SIZE 64
void kernel_main() {
    uart_init();
    // 初始化UART
    //uart_puts("Kernel started\n");
    // 读取当前EL
    //read_current_el();
    // 初始化内存
    memory_init();  
    
    char *ptr1 = (char *)my_malloc(INPUT_BUFFER_SIZE*sizeof(char));
    char *ptr2 = (char *)my_malloc(INPUT_BUFFER_SIZE*sizeof(char));
    if (ptr1) 
    {
        uart_puts("Enter text (max 63 chars): ");
        uart_gets((char *)ptr1, INPUT_BUFFER_SIZE*sizeof(char));
        uart_puts("You entered: ");
        uart_puts((char *)ptr1);
        uart_puts("\n");
        my_memset(ptr1,100 ,5);
        uart_puts("revised: ");
        uart_puts((char *)ptr1);
        uart_puts("\n");
        uart_puts("ptr2: ");
        uart_puts((char *)my_strncpy(ptr2, ptr1, 4));
        uart_puts("\n");
        uart_put_dec(my_strcmp(ptr1, ptr2));
        uart_puts("\n");
        uart_put_dec(my_strlen(ptr2));
        uart_puts("\n");
    }
    else 
    {
        uart_puts("Allocation failed!\n");
    }

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

    

    // uart_puts("=== Memory Allocator Test Start ===\n");
    // uart_puts("\n--- Test 1: Basic Allocation ---");
    //void *ptr1 = cmemory_alloc(4050);
    // uart_puts("\nptr1 : ");
    //uart_put_hex((unsigned long)ptr1);
    // void *ptr2 = cmemory_alloc(200);
    // uart_puts("\nptr2 (200 bytes): ");
    // uart_put_hex((unsigned long)ptr2);
    // void *ptr3 = cmemory_alloc(300);
    // uart_puts("\nptr3 (300 bytes): ");
    // uart_put_hex((unsigned long)ptr3);
    // uart_puts("\n--- Test 2: Free and Reuse ---");
    // cmemory_free(ptr2);
    // uart_puts("\nFreed ptr2");
    // void *ptr4 = cmemory_alloc(150);
    // uart_puts("\nptr4 (150 bytes): ");
    // uart_put_hex((unsigned long)ptr4);
    // uart_puts("\n--- Test 3: Multiple Pages ---");
    // void *ptr5 = cmemory_alloc(4000);
    // uart_puts("\nptr5 (4000 bytes): ");
    // uart_put_hex((unsigned long)ptr5);
    // void *ptr6 = cmemory_alloc(4000);
    // uart_puts("\nptr6 (4000 bytes): ");
    // uart_put_hex((unsigned long)ptr6);
    // uart_puts("\n--- Test 4: Coalescing ---");
    // cmemory_free(ptr1);
    // cmemory_free(ptr3);
    // cmemory_free(ptr4);
    // uart_puts("\nFreed ptr1, ptr3, ptr4");
    // void *ptr7 = cmemory_alloc(500);
    // uart_puts("\nptr7 (500 bytes): ");
    // uart_put_hex((unsigned long)ptr7);
    // uart_puts("\n--- Test 5: Edge Cases ---");
    // void *ptr8 = cmemory_alloc(0);
    // uart_puts("\nptr8 (0 bytes): ");
    // uart_put_hex((unsigned long)ptr8);
    // void *ptr9 = cmemory_alloc(8);
    // uart_puts("\nptr9 (8 bytes): ");
    // uart_put_hex((unsigned long)ptr9);
    // void *ptr10 = cmemory_alloc(1);
    // uart_puts("\nptr10 (1 byte): ");
    // uart_put_hex((unsigned long)ptr10);
    // uart_puts("\n--- Cleanup ---");
    // cmemory_free(ptr5);
    // cmemory_free(ptr6);
    // cmemory_free(ptr7);
    // cmemory_free(ptr9);
    // cmemory_free(ptr10);
    // uart_puts("\n=== Memory Allocator Test End ===\n");
    
    //virtio_blk_init();
    // uint8_t write_buf[512];
    // for (int i = 0; i < 512; i++) {write_buf[i] = i & 0xFF;}
    // virtio_blk_write(0, write_buf);
    // uint8_t read_buf[512];
    // virtio_blk_read(0, read_buf);
    
    
    // 导致内存泄漏
    //asm volatile(".word 0x00000000");
    // 退出内核
    //uart_puts("Kernel exit\n");
    // 退出QEMU
    qemu_exit(0);
}