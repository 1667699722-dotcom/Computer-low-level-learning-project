void *my_malloc(int size);
void my_free(void *ptr);
void *my_memset(void *s,int c ,unsigned int n);
void *my_memcpy(void *desk,const void* src,unsigned int n);
int my_strcmp(const char *s1, const char *s2);
char *my_strncpy(char *dest, const char *src, unsigned int n);
unsigned int my_strlen(const char *s);
void uart_put_dec(unsigned int val);