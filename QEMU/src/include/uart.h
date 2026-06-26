void uart_puts(const char *s);
void uart_putc(char c) ;
void uart_gets(char *buf, int len);
void uart_init();
void uart_put_hex(unsigned long val);