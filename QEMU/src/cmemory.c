
#define PAGE_SIZE 4096
typedef unsigned long size_t;
#define NULL ((void *)0)

#include "include/page.h"
#include "include/cmemory.h"

typedef struct{
    unsigned int presize;
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
    mem_region_t *old_regions = mem_regions;
    for(size_t i=0;i<pages_needed;i++)
    {
        void *page=page_alloc();
        if(!page)
        {
            mem_region_t *curr = mem_regions;
            while(curr != old_regions)
            {
                mem_region_t *next = curr->next;
                free_page((void *)curr);
                curr = next;
            }
            mem_regions = old_regions;
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
        header->presize=0;
    }
    return 0;
}

void* cmemory_alloc(int size)
{
    if(size<=0 || size>(int)(PAGE_SIZE-sizeof(mem_region_t)-sizeof(block_header_t)))
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
                    next_header->presize=header->size;
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
    {
        return cmemory_alloc(size);
    } 
    else if (extend_cmemory(size) == -1) {
        return NULL;
    }
    return NULL;
}

void cmemory_free(void *ptr)
{
    if (!ptr)
        return;
    
    block_header_t *header = (block_header_t *)((char *)ptr - sizeof(block_header_t));
    header->used = 0;
    
    unsigned long page_addr = (unsigned long)header & ~(PAGE_SIZE - 1);
    mem_region_t *region = (mem_region_t *)page_addr;
    // 这里可以添加块合并逻辑
    // （为了简化，先不实现合并）
    block_header_t * next_header=(block_header_t*)((char*)header+sizeof(block_header_t)+header->size);
    if((void*)next_header<region->end && !next_header->used)
    {
        header->size+=sizeof(block_header_t)+next_header->size;
        block_header_t *next_next_header = (block_header_t *)((char *)header + sizeof(block_header_t) + header->size);
        if((void*)next_next_header < region->end)
        {
            next_next_header->presize = header->size;
        }
    }

    
    if(header->presize>0)
    {
        block_header_t *prev_header = (block_header_t *)((char *)header - sizeof(block_header_t) - header->presize);
        if (!prev_header->used)
        {
            prev_header->size += sizeof(block_header_t) + header->size;
            
            // 更新下一个块的 presize（用 prev_header）
            block_header_t *new_next_header = (block_header_t *)((char *)prev_header + sizeof(block_header_t) + prev_header->size);
            if((void*)new_next_header < region->end)
            {
                new_next_header->presize = prev_header->size;
            }
        }
    }
    block_header_t *page_first_header = (block_header_t *)region->start;
    if(page_first_header->used == 0 && page_first_header->size == (unsigned long)region->end - (unsigned long)region->start - sizeof(block_header_t))
    {
        mem_region_t **prev_ptr = &mem_regions;
        mem_region_t *curr = mem_regions;
        while(curr)
        {
            if(curr == region)
            {
                *prev_ptr = curr->next;
                break;
            }
            prev_ptr = &curr->next;
            curr = curr->next;
        }
        free_page((void *)page_addr);
    }
}

