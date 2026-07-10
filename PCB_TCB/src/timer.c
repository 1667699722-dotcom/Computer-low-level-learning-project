#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
// 通用信号处理函数
static void common_handler(int sig) {
    switch (sig) {
        case SIGALRM:
            printf("定时器中断!\n");
            break;
        case SIGUSR1:
            printf("用户信号1触发!\n");
            break;
        default:
            printf("未知信号: %d\n", sig);
    }
    fflush(stdout);
}
// 启动定时器
void timerstart(void)
{
    struct sigaction sa;
    sa.sa_handler = common_handler;
    sa.sa_flags = SA_RESTART;  // 自动重启被中断的系统调用
    sigemptyset(&sa.sa_mask);  // 不屏蔽其他信号
    sigaction(SIGALRM, &sa, NULL);
    struct itimerval timer;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 10000;  // 10ms
    timer.it_value = timer.it_interval;
    setitimer(ITIMER_REAL, &timer, NULL);
}
// 关闭定时器
void timerstop(void)
{
    struct itimerval timer;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
}
// 注册用户信号1
void reg_usr1(void)
{
    struct sigaction sa;
    sa.sa_handler = common_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
}
// 空忙等待函数
void busy_sleep(int ms) {
    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    while (1) {
        clock_gettime(CLOCK_MONOTONIC, &now);
        long elapsed_ms = (now.tv_sec - start.tv_sec) * 1000 + 
                          (now.tv_nsec - start.tv_nsec) / 1000000;
        if (elapsed_ms >= ms) {
            break;
        }
    }
}




