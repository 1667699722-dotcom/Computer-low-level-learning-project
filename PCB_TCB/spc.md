
# PCB/TCB 用户态多任务模拟器

## 项目概述

基于 Linux 用户态标准 C 库，使用 `ucontext` 实现独立栈与上下文切换，搭配 `timerfd` 模拟硬件时钟中断，手动模拟 PCB/TCB 任务控制块、时间片轮转调度；结合 `fork` 区分进程与线程资源隔离模型，先吃透操作系统多任务软件模型，后续再拓展 ARM64 裸机底层实现。

## 任务规划

| 序号 | 任务描述 | 涉及文件 |
|------|---------|---------|
| 1 | 运行现有基础任务切换测试 | kernel.c, include/task.h, src/task.c |
| 2 | 完善 TCB 结构体，添加状态、时间片、链表字段 | include/task.h |
| 3 | 实现 task_enqueue() 和 task_dequeue() 就绪队列管理 | include/task.h, src/task.c |
| 4 | 创建调度器模块（scheduler.h/scheduler.c） | include/scheduler.h, src/scheduler.c |
| 5 | 实现非抢占式调度器（schedule()、task_yield()） | include/scheduler.h, src/scheduler.c |
| 6 | 编写并测试非抢占式多任务切换 | kernel.c |
| 7 | 集成定时器与调度器，实现抢占标志 | include/timer.h, src/timer.c |
| 8 | 实现时间片管理与 check_preempt() 抢占检查 | include/scheduler.h, src/scheduler.c |
| 9 | 编写并测试抢占式时间片轮转调度 | kernel.c |
| 10 | 定义 PCB 结构体，实现进程/线程模型 | include/task.h |
| 11 | 实现 pcb_create() 和 pcb_create_thread() | src/task.c |
| 12 | 编写并测试单进程多线程调度 | kernel.c |
| 13 | 编写并测试多进程多线程调度 | kernel.c |
