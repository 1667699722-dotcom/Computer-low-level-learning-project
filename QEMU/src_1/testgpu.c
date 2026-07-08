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

//测试GPU
void delay(int count) {
    for(int i=0;i<count;i++) {
        asm volatile("nop");
    }
}

void testgpu1()
{
    uart_puts("Testing GPU display...\n");
    //先清屏为红色 (格式: 0xRRGGBB)
    virtio_gpu_clear_screen(0xFF0000); // R=0xFF, G=0x00, B=0x00
    virtio_gpu_flush();
    uart_puts("Screen red, waiting...\n");
    delay(1000000000);
     //画个绿色方块
    for(int y=100;y<200;y++) {
        for(int x=100;x<200;x++) {
            virtio_gpu_draw_pixel(x,y,0x00FF00); // R=0x00, G=0xFF, B=0x00
        }
    }
    virtio_gpu_flush();
    uart_puts("Green square, waiting...\n");
    delay(1000000000);
    // 画个蓝色方块
    for(int y=250;y<350;y++) {
        for(int x=250;x<350;x++) {
            virtio_gpu_draw_pixel(x,y,0x0000FF); // R=0x00, G=0x00, B=0xFF
        }
    }
    virtio_gpu_flush();
    uart_puts("Blue square, waiting...\n");
    delay(1000000000);
    // 保持运行 - 闪烁三种颜色
    while(1) {
        // 闪烁屏幕
        virtio_gpu_clear_screen(0xFF0000); // 红色
        virtio_gpu_flush();
        delay(2000000000); 
        virtio_gpu_clear_screen(0x00FF00); // 绿色
        virtio_gpu_flush();
        delay(2000000000);
        virtio_gpu_clear_screen(0x0000FF); // 蓝色
        virtio_gpu_flush();
        delay(2000000000);
    }
}