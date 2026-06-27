void page_init();
//void test_page_init(unsigned long total_pages,unsigned long bitmap_pages,unsigned long page_area_start,unsigned long page_bitmap);

void *page_alloc();

void free_page(void* page);