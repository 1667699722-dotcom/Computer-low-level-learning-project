#include "src/include/uart.h"
#include "src/include/exit.h"
#include "src/include/lib.h"
#include "src/include/memory.h"
#include "src/include/exceptionhandler.h"
#include "src_1/include/exceptionsolution.h"
#include "src/include/page.h"
#include "src/include/cmemory.h"
#include "src/include/virtio.h"
#include "src/include/fs.h"
#include "src/include/gpu.h"
#include "src/include/spinlock.h"
#include "src_1/include/testfs.h"
#include "src_1/include/testgpu.h"

uint8_t test_data1[] = "Hello, this is a test file!";

#define INPUT_BUFFER_SIZE 64

spinlock_t test_lock;

int shared_counter = 0;

// 简单延时函数


void kernel_main() {
    exception_init();
    uart_init();
    memory_init();  
    page_init();
    cmemory_init();
    virtio_blk_init();
    fs_format();
    fs_init();
    //virtio_gpu_init();

    spinlock_init(&test_lock);

    spinlock_acquire(&test_lock);

    uart_puts("hello\n");
    testfs1();
    spinlock_release(&test_lock);

    // 不会到这里
    qemu_exit(0);
}
