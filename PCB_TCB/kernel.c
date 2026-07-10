#include <stdio.h>
#include <unistd.h>
#include "include/timer.h"
#include <time.h>
#include "include/task.h"
#include "include/scheduler.h"


Task taskA;
Task taskB;
Task taskC;
ucontext_t main_ctx;

Task *current_task;
Task *ready_queue;

void taskA_func() {
    for (int i = 0; i < 5; i++) {
        printf("任务A运行 - 第 %d 次\n", i+1);
        task_yield();
    }
    printf("任务A结束\n");
    swapcontext(&taskA.ctx, &main_ctx);
}

void taskB_func() {
    for (int i = 0; i < 5; i++) {
        printf("任务B运行 - 第 %d 次\n", i+1);
        task_yield();
    }
    printf("任务B结束\n");
}

void taskC_func() {
    for (int i = 0; i < 5; i++) {
        printf("任务C运行 - 第 %d 次\n", i+1);
        task_yield();
    }
    printf("任务C结束\n");
}

void test_nonpreempt() {
    printf("=== 非抢占式调度测试开始 ===\n");
    scheduler_init();

    task_create(&taskA, taskA_func, 1);
    task_create(&taskB, taskB_func, 2);
    task_create(&taskC, taskC_func, 3);

    taskA.state=running;
    taskB.state=running;
    taskC.state=running;

    task_enqueue(&ready_queue, &taskA);
    task_enqueue(&ready_queue, &taskB);
    task_enqueue(&ready_queue, &taskC);

    ready_queue=&taskA;
    current_task = task_dequeue(&ready_queue);
    current_task->state = running;
    swapcontext(&main_ctx, &current_task->ctx);  
printf ("任务结束 \n");
}

int main()
{
    test_nonpreempt();
    return 0;
}
