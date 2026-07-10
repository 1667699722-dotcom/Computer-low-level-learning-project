#include <stdio.h>
#include <unistd.h>
#include "include/timer.h"
#include <time.h>
#include "include/task.h"
#include <signal.h>

static Task taskA, taskB;
static ucontext_t main_ctx; 

// 任务A运行函数
void taskA_func() {
        printf("任务A运行...\n");
        task_swap(&taskA, &taskB);
        printf("任务A运行结束\n");
        task_swap(&taskA, &taskB);
        swapcontext(&taskA.ctx,&main_ctx);
}
// 任务B运行函数
void taskB_func() {
        printf("任务B运行...\n");
        task_swap(&taskB, &taskA);
        printf("任务B运行结束\n");
        task_swap(&taskB, &taskA);
}
// 测试任务切换
void testtask(void)
{
    printf("开始创建任务\n");
    
    task_create(&taskA, taskA_func, 1);
    task_create(&taskB, taskB_func, 2);
    
    printf("开始切换任务\n");
    swapcontext(&main_ctx, &taskA.ctx);
    printf("任务结束\n");
}
// 测试定时器
void testtimer(void)
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
// 主函数
int main()
{
    testtask();
    return 0;
}
