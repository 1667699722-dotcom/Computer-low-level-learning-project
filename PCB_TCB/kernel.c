#include <stdio.h>
#include <unistd.h>
#include "include/timer.h"
#include "include/task.h"
#include "include/scheduler.h"


Task taskD;
Task taskE;
Task taskF;
ucontext_t main_ctx;

void taskD_func(){
    for (int i = 0; i < 12; i++) {
        printf("任务D运行 - 第 %d 次\n", i+1);
        busy_sleep(200);
    }
    printf("任务D结束\n");
    taskD.state=blocked;
    task_yield();
}

void taskE_func() {
    for (int i = 0; i < 6; i++) {
        printf("任务E运行 - 第 %d 次\n", i+1);
        busy_sleep(200);
    }
    printf("任务E结束\n");
    taskE.state=blocked;
    task_yield();
    //swapcontext(&taskE.ctx, &main_ctx);
}

void task_k_func() {
    while (1) {
        char cmd;
        //printf("任务F运行\n");
        if (buf_get(&cmd)) {  // 从缓冲区读字符
            // 翻译命令
            switch (cmd) {
                case 'q': 
                    printf("[Task K] 收到退出命令！\n");
                    exit(0);
                    break;
                case 'n':
                    printf("[Task K] 收到切换命令！\n");
                    need_resched = 1;  // 主动触发调度
                    schedule();
                    break;
                case 'p':
                    printf("[Task K] 打印系统状态...\n");
                    // 打印任务队列、当前任务等信息
                    break;
                default:
                    printf("[Task K] 收到未知字符：%c\n", cmd);
            }
        }

        // 处理完后，忙等待或让出CPU
        busy_sleep(10);
    }
}

void test_preempt() {
    printf("=== 抢占式调度测试开始 ===\n");
    
    scheduler_init();
    need_resched = 0;
    
    task_create(&taskD, taskD_func, 4);
    task_create(&taskE, taskE_func, 5);
    task_create(&taskF, task_k_func, 6);
    task_enqueue(&ready_queue, &taskD);
    task_enqueue(&ready_queue, &taskE);
    task_enqueue(&ready_queue, &taskF);

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
    set_stdin_nonblock();
    setup_signal_io();
    test_preempt();
    return 0;
}
