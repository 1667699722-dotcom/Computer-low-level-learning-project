
#define NULL ((void *)0)
#define UART0_BASE 0x09000000

#define UART_DR     (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UART_FR     (*(volatile unsigned int *)(UART0_BASE + 0x18))
#define UART_CR     (*(volatile unsigned int *)(UART0_BASE + 0x30))

void uart_init()
{
    UART_CR = (1 << 0) | (1 << 8) | (1 << 9);
}

void uart_putc(char c) 
{
    while ((UART_FR & (1 << 5)) != 0);
    UART_DR = c;
}

void uart_puts(const char *s) 
{
    while (*s) {
        uart_putc(*s++);
    }
}

char uart_getc()
{
    while ((UART_FR & (1 << 4)) != 0);
    return (char)UART_DR;
}

void uart_gets(char *buf, int len)
{
    if (buf == NULL || len <= 1)
    {
        if (buf != NULL)
            buf[0] = '\0';
        return;
    }
    int i = 0;
    char ch;
    while (i < len - 1)
    {
        ch = uart_getc();
        if (ch == '\r' || ch == '\n')
            break;
        uart_putc(ch);
        buf[i++] = ch;
    }
    uart_puts("\n");
    buf[i] = '\0';
}

