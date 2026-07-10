#include "./../include/task.h"
#include <stdlib.h>
#define STACK_SIZE 32768
#define DEFAULT_TIME_SLICE 10


void task_create(Task *t, void (*func)(void), int id)
{
    t->id=id;
    t->state=ready;
    t->time_slice=DEFAULT_TIME_SLICE;
    t->next=NULL;
    getcontext(&t->ctx);
    t->stack=(char*)malloc(STACK_SIZE );
    t->ctx.uc_stack.ss_sp=t->stack;
    t->ctx.uc_stack.ss_size=STACK_SIZE;
    t->ctx.uc_link = NULL; // 任务结束后退出
    makecontext(&t->ctx,func,0);
}

void task_swap(Task *old_t, Task *new_t) {
    swapcontext(&old_t->ctx, &new_t->ctx);
}

void task_enqueue(Task **queue, Task *task)
{
    task->next=NULL;
    if(*queue==NULL)
    {
        *queue=task;
    }
    else 
    {
        Task *current=*queue;
        while(current->next!=NULL)
        {
            current=current->next;
        }
        current->next=task;
    }
}

Task* task_dequeue(Task **queue)
{
    if(*queue==NULL)
    {
        return NULL;
    }
    Task *task=*queue;
    *queue=(*queue)->next;
    task->next=NULL;
    return task;
}







