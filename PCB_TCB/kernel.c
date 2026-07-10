#include <stdio.h>
#include <unistd.h>
#include "include/timer.h"
#include "include/task.h"
#include "include/scheduler.h"


Task taskD;
Task taskE;
ucontext_t main_ctx;

void taskD_func(){
    for (int i = 0; i < 10; i++) {
        printf("任务D运行 - 第 %d 次\n", i+1);
        busy_sleep(200);
    }
    printf("任务D结束\n");
    swapcontext(&taskD.ctx, &main_ctx);
}

void taskE_func() {
    for (int i = 0; i < 10; i++) {
        printf("任务E运行 - 第 %d 次\n", i+1);
        busy_sleep(200);
    }
    printf("任务E结束\n");
    swapcontext(&taskE.ctx, &main_ctx);
}

void test_preempt() {
    printf("=== 抢占式调度测试开始 ===\n");
    
    scheduler_init();
    need_resched = 0;
    
    task_create(&taskD, taskD_func, 4);
    task_create(&taskE, taskE_func, 5);
    
    task_enqueue(&ready_queue, &taskD);
    task_enqueue(&ready_queue, &taskE);
    
    reg_usr1();
    timerstart();
    
    current_task = task_dequeue(&ready_queue);
    current_task->state = running;
    
    printf("开始调度\n");
    swapcontext(&main_ctx, &current_task->ctx);
    
    timerstop();
    printf("=== 抢占式调度测试结束 ===\n");
}

int main() {
    test_preempt();
    return 0;
}
