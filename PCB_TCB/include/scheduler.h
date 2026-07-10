
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

extern Task *current_task;
extern Task *ready_queue;
extern volatile int need_resched;

void scheduler_init();
void schedule();
void task_yield();
void check_preempt();

#endif
