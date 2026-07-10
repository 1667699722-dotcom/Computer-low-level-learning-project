<div align="center">

# 408 Low-Level Learning Project / 408 底层学习项目

<p align="center">
  <img src="https://img.shields.io/badge/Assembly-ARM64-0091BD?style=for-the-badge&logo=arm&logoColor=white" alt="ARM64">
  <img src="https://img.shields.io/badge/Verilog-HDL-2C2255?style=for-the-badge&logoColor=white" alt="Verilog">
  <img src="https://img.shields.io/badge/C-Language-A8B9CC?style=for-the-badge&logo=c&logoColor=white" alt="C">
  <img src="https://img.shields.io/badge/CUDA-Parallel-76B900?style=for-the-badge&logo=nvidia&logoColor=white" alt="CUDA">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/macOS-000000?style=flat-square&logo=apple&logoColor=white" alt="macOS">
  <img src="https://img.shields.io/badge/Platform-ARM64-5C2D91?style=flat-square" alt="Platform">
  <img src="https://img.shields.io/badge/status-active-brightgreen?style=flat-square" alt="Status">
</p>

</div>

> ARM64 汇编、自制操作系统、算法导论、芯片设计 - 全方位底层学习
>
> ARM64 Assembly, Custom OS, Introduction to Algorithms, Chip Design - Comprehensive low-level learning

***

## 项目结构 / Project Structure

```
408/
├── CUDA/        # CUDA 并行计算 / CUDA Parallel Computing
├── Chip/        # Verilog 芯片设计 / Verilog Chip Design
├── UISIntr/     # ARM64 汇编系统编程 / ARM64 Assembly System Programming
├── QEMU/        # AArch64 裸机操作系统 / AArch64 Bare-metal OS
├── PCB_TCB/     # 用户态多任务模拟器 / User-space Multi-task Simulator
├── Algorithms/  # 算法导论与数据结构 / Introduction to Algorithms & Data Structures
├── journal/     # 每日开发日志 / Daily Development Logs
└── bin/         # 编译后的二进制 / Compiled Binaries
```

***

## 1. ARM64 汇编系统编程 (UISIntr/) / ARM64 Assembly System Programming (UISIntr/)

**中文：**
在 macOS 上用 ARM64 汇编实现系统级编程，包括内存管理、系统调用、自定义指令解析器等。

**English:**
System-level programming in ARM64 assembly on macOS, including memory management, system calls, custom instruction parser, etc.

**已完成 / Completed:**

- ✅ macOS 用户态基础：程序入口、主循环、条件跳转、系统调用、LLDB 调试
- ✅ 内存管理：堆结构、块头格式、分配（带分割）、释放（带合并）、边界检查
- ✅ 字符串运行时：堆字符串复制、平行指针/长度数组
- ✅ 指令解析器：Token 拆解、数字输出、指令执行
- ✅ 简单解释器：能执行 ADD/SUB/MUL 等基本指令

***

## 2. AArch64 裸机操作系统 (QEMU/) / AArch64 Bare-metal OS (QEMU/)

**中文：**
在 QEMU 模拟的 virt 机器上开发完整的 AArch64 裸机操作系统，学习从启动、驱动开发、文件系统到显示输出的全栈底层技术。

**English:**
Develop a full AArch64 bare-metal OS on QEMU-emulated virt machine, learning full-stack low-level technologies from boot sequence, driver development, file system to display output.

**状态 / Status:**
⏸️ 挂起 - 定时器中断卡住，暂时转向用户态模拟器

**已完成 / Completed:**

- ✅ 工具链与启动：clang+ld.lld 裸机编译、`_start` 入口、BSS 清零、ELF 镜像生成
- ✅ UART 驱动：完整的输入输出（`uart_puts`/`uart_gets`/`uart_getc`）
- ✅ 内存管理：链表内存分配器（cmemory.c）、支持块分割/合并、4字节对齐、页分配器（page.c）
- ✅ 异常处理：异常向量表、异常处理框架（exceptionhandler.c）、ESR 解析
- ✅ Virtio 设备驱动：virtio-blk（块设备）、virtio-gpu（GPU）（virtio.c）
- ✅ 文件系统：简单文件系统实现、文件创建/读写（fs.c）
- ✅ GPU 显示：B8G8R8A8_UNORM 像素格式、清屏/画点、全屏刷新（gpu.c）

**卡住的问题 / Blocked:**
- CNTHP_EL2 定时器中断无法触发
- GIC 驱动：GICv2 中断控制器初始化（GIC.c）
**后续 / Next:**
- 调度器与多线程
- MMU 与虚拟内存

**详细进度 / Detailed Progress:** [QEMU/schedule.md](./QEMU/schedule.md)

***

## 3. 用户态多任务模拟器 (PCB_TCB/) / User-space Multi-task Simulator (PCB_TCB/)

**中文：**
基于 Linux 用户态标准 C 库，使用 ucontext 实现独立栈与上下文切换，搭配 timerfd 模拟硬件时钟中断，手动模拟 PCB/TCB 任务控制块、时间片轮转调度；结合 fork 区分进程与线程资源隔离模型，先吃透操作系统多任务软件模型，再扩展到裸机底层实现。

**English:**
Based on Linux user-space standard C library, use ucontext for independent stack and context switching, timerfd for simulated hardware clock interrupts, manually implement PCB/TCB task control blocks and time-slice round-robin scheduling; combine fork to distinguish process and thread resource isolation models. Master OS multi-tasking software model first, then expand to bare-metal implementation.

**状态 / Status:**
🚀 进行中 - 项目规划完成，准备开始实现

**计划 / Plan:**
- 任务控制块模块（模拟 PCB/TCB）
- 任务创建模块（独立栈、ucontext）
- 时钟中断模拟模块（timerfd）
- 轮转调度器模块
- 任务状态管理
- 进程对比拓展模块（fork）

**详细规划 / Detailed Plan:** [PCB_TCB/spc.md](./PCB_TCB/spc.md)

***

## 4. 芯片设计 (Chip/) / Chip Design (Chip/)

**中文：**
用 Verilog 实现数字电路模块，学习数字逻辑与自制 CPU 设计。

**English:**
Implement digital circuit modules in Verilog, learning digital logic and custom CPU design.

**已完成 / Completed:**

- ✅ 基础模块：加法器 / Adder
- ✅ UART 串口：接收、发送模块 / UART RX/TX

**进行中 / In Progress:**
- 🔧 自制 AArch64 兼容 CPU 设计

***

## 5. 算法导论与数据结构 (Algorithms/) / Introduction to Algorithms & Data Structures (Algorithms/)

**中文：**
《算法导论》的 C 语言完整实现，以及经典数据结构与算法题解。

**English:**
Complete C implementation of "Introduction to Algorithms", and classic data structures & algorithm problem solutions.

**已完成 / Completed:**

- ✅ 排序算法：插入排序、归并排序、快速排序、堆排序
- ✅ 分治算法：最大子数组、矩阵乘法
- ✅ 数据结构：栈、队列
- ✅ LeetCode 题目：两数之和、有效的括号、设计循环队列

***

## 6. CUDA 并行计算 (CUDA/) / CUDA Parallel Computing (CUDA/)

**中文：**
学习 CUDA 并行编程，包括 GPU 架构、核函数编写、内存管理等。

**English:**
Learning CUDA parallel programming, including GPU architecture, kernel writing, memory management, etc.

***

## 7. 每日开发日志 (journal/) / Daily Development Logs (journal/)

**中文：**
记录每日开发进度、问题和思考，按日期组织。

**English:**
Record daily development progress, issues and thoughts, organized by date.

**查看日志 / View Logs:** [journal/README.md](./journal/README.md)




