void timerstart(void);//启动定时器
void timerstop(void);// 关闭定时器
void busy_sleep(int ms);// 空忙等待函数
void reg_usr1(void);// 注册用户信号1
void set_stdin_nonblock(void);
int buf_get(char *c);
void setup_signal_io(void);

