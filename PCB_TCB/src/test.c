#include <stdio.h>
#include <unistd.h>
#include "./../include/timer.h"
#include <time.h>
#include <signal.h>
#include "./../include/task.h"
void test(void)
{
    printf("定时器启动\n");
    reg_usr1();
    timerstart();
    busy_sleep(50);
    printf("定时器关闭\n");
    timerstop();
    kill(getpid(), SIGUSR1);
    // 再等1秒，看看有没有中断
    busy_sleep(2500);
    kill(getpid(), SIGUSR1);
    printf("程序结束\n");
}