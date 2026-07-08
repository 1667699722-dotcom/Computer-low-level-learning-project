#include "./../src/include/uart.h"
#include "./../src/include/exit.h"
#include "./../src/include/lib.h"
#include "./../src/include/memory.h"
#include "./../src/include/exceptionhandler.h"
#include "./../src_1/include/exceptionsolution.h"
#include "./../src/include/page.h"
#include "./../src/include/cmemory.h"
#include "./../src/include/virtio.h"
#include "./../src/include/gpu.h"
#include "./../src/include/spinlock.h"
#include "./../src/include/fs.h"
#define INPUT_BUFFER_SIZE 64
uint8_t test_data2[] = "Hello, this is a test file!";
void testfs1()
{
    uart_puts("=== Running binary code ===\n");
   static uint32_t prog_data[]  = {
    0xD28000A0,
    0xD2800061,
    0x8B010000,
    0xD65F03C0
};
    uint64_t (*func)(void) = (uint64_t (*)(void))prog_data;
    uint64_t val = func();
    uart_puts("Result: ");
    uart_put_dec(val);
    uart_puts("\n");
  
    uart_put_dec( fs_create("test2.txt"));
    uart_puts("\n");
    fs_write("test2.txt", (char *)prog_data, sizeof(prog_data));
    char *ptr2 = (char *)cmemory_alloc(INPUT_BUFFER_SIZE*sizeof(char));
    fs_read("test2.txt", ptr2, sizeof(test_data2));
    uint64_t (*func1)(void) = (uint64_t (*)(void))ptr2;
    uint64_t val2 = func1();
    uart_put_dec(val2); 
    uart_puts("\n");
}