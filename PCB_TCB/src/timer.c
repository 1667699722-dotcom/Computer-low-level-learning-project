#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include "./../include/task.h"
#include "./../include/scheduler.h"
#include <unistd.h>    // STDIN_FILENO, read()
#include <fcntl.h>     // fcntl(), O_NONBLOCK
#include "./../include/io_stdin.h"

void set_stdin_nonblock() {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);  // 设置非阻塞标志
}

// 新增：配置信号驱动 I/O
void setup_signal_io(void) {
    int flags;
    // 1. 设置 O_ASYNC 标志
    flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_ASYNC | O_NONBLOCK);
    // 2. 设置接收信号的进程
    fcntl(STDIN_FILENO, F_SETOWN, getpid());
}
//
extern volatile int need_resched;
extern Task *current_task;
#define DEFAULT_TIME_SLICE 10
char c;
static void common_handler(int sig) {
    switch (sig) {
        case SIGALRM:
            // 先设置标志，再检查并调度
            need_resched = 1;
            check_preempt();
            break;
        case SIGUSR1:
            printf("用户信号1触发!\n");
            break;
        case SIGIO:
            if(read(STDIN_FILENO, &c, 1) == 1)
            {
                buf_put(c);
            }
            need_resched = 1;
            check_preempt();
            break;
        default:
            printf("未知信号: %d\n", sig);
    }
    //fflush(stdout);
}
// 启动定时器
void timerstart(void)
{
    struct sigaction sa;
    sa.sa_handler = common_handler;
    sa.sa_flags = SA_RESTART;  // 自动重启被中断的系统调用
    sigemptyset(&sa.sa_mask);  // 不屏蔽其他信号
    sigaction(SIGALRM, &sa, NULL);

    sigaction(SIGIO, &sa, NULL);

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
