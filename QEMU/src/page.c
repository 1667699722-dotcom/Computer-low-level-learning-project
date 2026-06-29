#include "include/memory.h"
#include "include/uart.h"
#include "include/page.h"

#define PAGE_SIZE 4096
#define NULL ((void *)0)
#define BITS_PER_PAGE (PAGE_SIZE * 8)

extern unsigned long __heap_start;
extern unsigned long __heap_end;

static unsigned long *page_bitmap;
static unsigned long total_pages;
static unsigned long bitmap_pages;
static unsigned long page_area_start;

void test_page_init(unsigned long total_pages,unsigned long bitmap_pages,unsigned long page_area_start,unsigned long page_bitmap)
{
    uart_puts("Page frame manager initialized\n");
    uart_puts("Total pages: ");
    uart_put_hex(total_pages);
    uart_puts("\n");
    uart_puts("Bitmap pages: ");
    uart_put_hex(bitmap_pages);
    uart_puts("\n");
    uart_puts("Page area start: ");
    uart_put_hex(page_area_start);
    uart_puts("\n");
    uart_puts("Page bitmap start: ");
    uart_put_hex(page_bitmap);
    uart_puts("\n");
}

void page_init()
{
    unsigned long start = (unsigned long)&__heap_start;
    unsigned long end = (unsigned long)&__heap_end;
    // 对齐到页边界
    start = (start + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    end = end & ~(PAGE_SIZE - 1);
    // 计算总页数
    total_pages = (end - start) / PAGE_SIZE;
    // 计算位图页数
    bitmap_pages = (total_pages + BITS_PER_PAGE - 1) / BITS_PER_PAGE;
    if(bitmap_pages==0)
    {
        bitmap_pages=1;
    }
    page_bitmap=(unsigned long *)start;
    page_area_start=start+bitmap_pages*PAGE_SIZE;
    unsigned long i;
    for (i = 0; i < bitmap_pages * PAGE_SIZE / sizeof(unsigned long); i++)
    {
        page_bitmap[i] = 0;
    }
    unsigned long start_page_num = 0;
    unsigned long end_page_num = bitmap_pages;
    for (unsigned long p = start_page_num; p < end_page_num; p++)
    {
        unsigned long ul = p / 64;
        unsigned long bit = p % 64;
        page_bitmap[ul] |= 1UL << bit;
    }
    test_page_init(total_pages,bitmap_pages,page_area_start,(unsigned long)page_bitmap); 
}


void *page_alloc(){
    unsigned long p;
    unsigned long start = (unsigned long)&__heap_start;
    for(p=0;p<total_pages;p++)
    {
        unsigned long ul=p/64;
        unsigned long bit=p%64;
        if(!(page_bitmap[ul]&(1UL<<bit)))
        {
            page_bitmap[ul]|=(1UL<<bit);
            //uart_puts("Page bitmap ");
            //uart_put_hex(p);
            //uart_puts(":");
            //uart_put_hex(start+p*PAGE_SIZE);
            //uart_put_hex(start);
            //uart_puts("\n");
            return (void *)(start+p*PAGE_SIZE);
        }
    }
    //uart_put_hex(start+p);
    //uart_puts("\n");
    return NULL;
}

void free_page(void* page)
{
    if(page==NULL)
    {
        return;
    }
    unsigned long addr=(unsigned long)page;
    unsigned long start=(unsigned long)&__heap_start;
    if(addr<start || addr>=(unsigned long)&__heap_end)
    {
        return;
    }
    unsigned long p=(addr-start)/PAGE_SIZE;
    unsigned long ul=p/64;
    unsigned long bit=p%64;
    page_bitmap[ul] &= ~(1UL << bit);
}


