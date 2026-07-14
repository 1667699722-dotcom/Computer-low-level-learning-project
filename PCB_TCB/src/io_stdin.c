#define BUF_SIZE 128

char keyboard_buffer[BUF_SIZE];
volatile int buf_head = 0;  // 写入位置
volatile int buf_tail = 0;  // 读取位置
volatile int buf_count = 0; // 缓冲区字符数

// 信号处理函数中调用：快速存入
void buf_put(char c) {
    if (buf_count < BUF_SIZE) {
        keyboard_buffer[buf_head] = c;
        buf_head = (buf_head + 1) % BUF_SIZE;
        buf_count++;
    }
}

// 任务中调用：读取字符
int buf_get(char *c) {
    if (buf_count > 0) {
        *c = keyboard_buffer[buf_tail];
        buf_tail = (buf_tail + 1) % BUF_SIZE;
        buf_count--;
        return 1;  // 成功
    }
    return 0;  // 缓冲区空
}