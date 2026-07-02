#include "include/memory.h"
#include "include/uart.h"
void *my_malloc(int size) 
{
    int aligned_size = (size + 7) & ~7;
    return memory_alloc(aligned_size);
}

void my_free(void *ptr) {
    memory_free(ptr);
}

void *my_memset(void *s,int c ,unsigned int n)
{
    unsigned char * p=(unsigned char *)s;
    while(n--)
    {
        *p++=(unsigned char)c; 
    }
    return s;
}

void *my_memcpy(void *desk,const void* src,unsigned int n)
{
    unsigned char *d=(unsigned char*)desk;
    const unsigned char *s=(const unsigned char*)src;
    while(n--)
    {
        *d++=*s++;
    }
    return desk;
}

int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2 && *(unsigned char *)s1 == *(unsigned char *)s2) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

char *my_strncpy(char *dest, const char *src, unsigned int n) 
{
    unsigned int i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

unsigned int my_strlen(const char *s) {
    unsigned int len = 0;
    while (*s++) {
        len++;
    }
    return len;
}

void uart_put_dec(unsigned int val) {
    char buf[32];
    int i = 30;
    buf[31] = '\0';
    
    if (val == 0) {
        uart_putc('0');
        return;
    }
    
    while (val > 0) {
        buf[i--] = '0' + (val % 10);
        val /= 10;
    }
    
    uart_puts(&buf[i + 1]);
}
