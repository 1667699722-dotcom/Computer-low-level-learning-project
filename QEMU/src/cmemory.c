#define PAGE_SIZE 4096
typedef unsigned long size_t; 
#define NULL ((void *)0)

#include "include/page.h"
#include "include/cmemory.h"

typedef struct{
    unsigned int size;
    unsigned int used;
}block_header_t;

typedef struct mem_region {
    void *start;
    void *end;
    struct mem_region *next;
}mem_region_t;
static mem_region_t *mem_regions=NULL;

void cmemory_init()
{
    mem_regions=NULL;
}

static int extend_cmemory(size_t needed_size)
{
    size_t pages_needed = (needed_size + sizeof(block_header_t) 
    + PAGE_SIZE - 1) / PAGE_SIZE;
    for(size_t i=0;i<pages_needed;i++)
    {
        void *page=page_alloc();
        if(!page)
        {
            return -1;
        }

        mem_region_t *region=(mem_region_t *)page;
        region->start=page+sizeof(mem_region_t);
        region->end=page+PAGE_SIZE;
        region->next=mem_regions;
        mem_regions=region;

        block_header_t *header=(block_header_t *)region->start;
        header->size=region->end-region->start-sizeof(block_header_t);
        header->used=0;
    }
    return 0;
}

void* cmemory_alloc(int size)
{
    if(size<=0)
    {
        return NULL;
    }
    size=(size+7)&~7;

    mem_region_t *region=mem_regions;
    while(region)
    {
        block_header_t* header=(block_header_t*)region->start;
        void *end=region->end;
        while((void*)header<end)
        {
            if (!header->used && (int)header->size >= (int)size)
            {
                 unsigned int remaining=header->size-size-sizeof(block_header_t);
                if(remaining>=8)
                {
                    block_header_t *next_header=(block_header_t*)((char*)header+sizeof(block_header_t)+size);
                    next_header->size=remaining;
                    next_header->used=0;
                    header->size=size;
                }
                header->used=1;
                return (char *)header + sizeof(block_header_t);
            }
            header = (block_header_t *)((char *)header + sizeof(block_header_t) + header->size);
        }
        region=region->next;
    }
    if (extend_cmemory(size) == 0)
        return cmemory_alloc(size);
    return NULL;
}

void cmemory_free(void *ptr)
{
    if (!ptr)
        return;
    
    block_header_t *header = (block_header_t *)((char *)ptr - sizeof(block_header_t));
    header->used = 0;
    
    // 这里可以添加块合并逻辑
    // （为了简化，先不实现合并）
}

