#include "./../include/task.h"
#include <stdlib.h>
#define STACK_SIZE 32768
void task_create(Task *t, void (*func)(void), int id)
{
    t->id=id;
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



