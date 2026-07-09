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
#include "src/include/GIC.h"
#include "src/include/time.h"

uint8_t test_data1[] = "Hello, this is a test file!";
#define INPUT_BUFFER_SIZE 64
spinlock_t test_lock;
int shared_counter = 0;

static inline void wfe(void)
{
    asm volatile("wfe");
}

void delay1(uint64_t cycles)
{
    for (volatile uint64_t i = 0; i < cycles; i++) {
        // 空循环，消耗时间
    }
}

void kernel_main(void) {
    
    while (1) {
        wfe();
    }

}


void kernel_main_cpu1() {
    exception_init();
    memory_init();  
    page_init();
    cmemory_init();
    virtio_blk_init();
    fs_format();
    fs_init();
    uart_init();
   
    spinlock_init(&test_lock);
    spinlock_acquire(&test_lock);
    uart_puts("CPU1 is running!\n");
    spinlock_release(&test_lock);
    
    
    readCNTHP_CTL_EL2();
    cnthp_set_period(10000);
    cnthp_start_irq();
    readCNTHP_CTL_EL2();
    delay1(1000000);
    readCNTHP_CTL_EL2();
    
    
    cnthp_set_period(10000);
    readCNTHP_CTL_EL2();
    cnthp_start_irq();
    readCNTHP_CTL_EL2();
    delay1(1000000);
    readCNTHP_CTL_EL2();
    
    cnthp_stop();
    readCNTHP_CTL_EL2();

    
 

    qemu_exit(0);
}



void kernel_main_cpu2(void) {

    while (1) {
        wfe();
    }
}

void kernel_main_cpu3(void) {


    while (1) {
        wfe();
    }

}
