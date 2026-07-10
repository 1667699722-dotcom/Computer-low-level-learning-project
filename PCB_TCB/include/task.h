#ifndef TASK_H
#define TASK_H

#define _XOPEN_SOURCE 700
#include <ucontext.h>
#include <stdlib.h>

typedef enum
{
    running,
    ready,
    blocked
}Taskstate;

typedef struct Task
{
    ucontext_t ctx;
    char* stack;
    int id;
    Taskstate state;
    int time_slice;
    struct Task *next;
} Task;

void task_create(Task *t, void (*func)(void), int id);
void task_swap(Task *old_t, Task *new_t);
void task_enqueue(Task **queue,Task *task);
Task* task_dequeue(Task **queue);

#endif


