
#define _XOPEN_SOURCE 700
#include <ucontext.h>
#include <stdlib.h>

typedef struct Task {
    ucontext_t ctx;
    char* stack;
    int id;
} Task;

void task_create(Task *t, void (*func)(void), int id);
void task_swap(Task *old_t, Task *new_t);



