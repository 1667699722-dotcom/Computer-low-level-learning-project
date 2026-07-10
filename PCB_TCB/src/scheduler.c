#include "./../include/task.h"
#include <stdio.h>

Task *current_task = NULL;
Task *ready_queue = NULL;
volatile int need_resched = 0;

void scheduler_init()
{
    current_task=NULL;
    ready_queue=NULL;
}
void schedule()
{
    Task *old_task=current_task;
    if(old_task!=NULL && old_task->state==running)
    {
        old_task->state=ready;
        task_enqueue(&ready_queue, old_task);
    }
    Task *next_task=task_dequeue(&ready_queue);
    if(next_task==NULL)
    {
        printf("error:no ready task to schedule\n");
        return;
    }
    next_task->state=running;
    current_task=next_task;
    if(old_task!=NULL)
    {
        task_swap(old_task, next_task);
    }
    printf("%s",(char *)old_task);
}

void task_yield() {
    schedule();
}

void check_preempt() {
    if (need_resched) {
        
        need_resched = 0;
        schedule();
    }
}

