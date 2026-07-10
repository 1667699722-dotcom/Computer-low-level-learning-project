#include <stdio.h>
#include <unistd.h>
#include "include/timer.h"
#include <time.h>
#include "include/task.h"

Task taskA, taskB;
static ucontext_t main_ctx; 

void taskA_func() {
        printf("任务A运行...\n");
        task_swap(&taskA, &taskB);
        printf("任务A运行结束\n");
        task_swap(&taskA, &taskB);
        swapcontext(&taskA.ctx,&main_ctx);
}

void taskB_func() {
        printf("任务B运行...\n");
        task_swap(&taskB, &taskA);
        printf("任务B运行结束\n");
        task_swap(&taskB, &taskA);
}
int main()
{
    printf("开始创建任务\n");
    
    task_create(&taskA, taskA_func, 1);
    task_create(&taskB, taskB_func, 2);
    
    printf("开始切换任务\n");
    swapcontext(&main_ctx, &taskA.ctx);
    printf("任务结束\n");
    
    return 0;
}
