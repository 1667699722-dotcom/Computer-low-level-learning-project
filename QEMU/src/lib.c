#include "include/memory.h"

void *my_malloc(int size) 
{
    int aligned_size = (size + 7) & ~7;
    return memory_alloc(aligned_size);
}

void my_free(void *ptr) {
    memory_free(ptr);
}