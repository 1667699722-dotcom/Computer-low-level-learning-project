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

> 计算机底层学习项目：汇编系统编程、芯片设计、算法数据结构
>
> Computer low-level learning project: Assembly system programming, chip design, algorithms & data structures

***

## 项目结构 / Project Structure

```
408/
├── CUDA/        # CUDA 并行计算 / CUDA Parallel Computing
├── Chip/        # Verilog 芯片设计 / Verilog Chip Design
├── ISA/         # ARM64 用户态汇编系统编程（进行中）/ ARM64 User-mode Assembly (In Progress)
├── QEMU/        # QEMU 裸机 AArch64（进行中）/ QEMU Bare-metal AArch64 (In Progress)
├── algo/        # C 语言算法与数据结构 / Algorithms &amp; Data Structures in C
├── ctos/        # 其他 / Others
└── bin/         # 编译后的二进制 / Compiled Binaries
```

***

## 1. 汇编系统编程 (ISA/) / Assembly System Programming (ISA/)

**中文：**
在 macOS 上用 ARM64 汇编实现系统级编程，包括内存管理、系统调用、自定义指令解析等。

**English:**
System-level programming in ARM64 assembly on macOS, including memory management, system calls, custom instruction parser, etc.

**已完成 / Completed:**

- ✅ macOS 用户态基础：程序入口、主循环、条件跳转、系统调用、LLDB 调试、多文件拆分
- ✅ 内存管理：堆结构、块头格式、分配（带分割）、释放（带前后合并）、边界检查
- ✅ 系统调用与 I/O：write/read 封装、单字符/字符串读写、输入/输出缓冲区
- ✅ 字符串运行时：堆字符串复制、平行指针/长度数组（string_ptrs / string_lens）、按序号存取、按序号打印
- ✅ 解析器前端：`find_two_spaces` 扫描双空格位置、空格记录、失败路径
- ✅ Token 拆解：指令存储区、堆块分配、store_instr_addrs/get_instr_addrs
- ✅ 数字输出：uint_to_dec 反向写数字、print_uint 完整实现
- ✅ 字符串转数字：atoi_scan 基础实现（数字字符判断、十进制累积）
- ✅ 指令执行：execute_add 加法指令实现

**进行中 / In Progress:**

- 🔧 Token 边界检查与连续空格支持 / Token boundary &amp; multi-space
- 🔧 print_int 负数输出 / Signed integer output
- 🔧 atoi_scan 完善（非数字终止、溢出检测）/ atoi_scan improvements
- 🔧 命令解析（ADD/SUB/MUL/EXIT 识别）/ Command parsing
- 🔧 指令执行扩展（execute_sub/execute_mul）/ More instructions
- 🔧 解释器主循环 / Interpreter main loop

详细计划 / Detailed Schedule: [ISA/spec/schedule.md](ISA/spec/schedule.md)

***

## 2. QEMU 裸机 AArch64 (QEMU/) / QEMU Bare-metal AArch64 (QEMU/)

**中文：**
在 QEMU 模拟的 virt 机器上运行裸机程序，学习裸机启动、UART 驱动、异常处理、内存管理、线程/进程调度等底层机制。

**English:**
Bare-metal programming on QEMU-emulated virt machine, learning boot sequence, UART driver, exception handling, memory management, thread/process scheduling, etc.

**已完成 / Completed:**

- ✅ 工具链：clang + ld.lld 裸机编译、ELF 镜像生成、QEMU 参数配置、CMake 构建系统
- ✅ 启动层：`_start` 裸机入口、初始栈设置、BSS 清零、跳转到 kernel_main
- ✅ 链接脚本：linker.ld 段布局、定义内核加载地址和堆区域
- ✅ UART 驱动：uart_putc/uart_puts/uart_getc/uart_gets/uart_put_hex 完整输入输出
- ✅ 内存管理：链表内存分配器（带块分割/合并）、4字节对齐、内存初始化
- ✅ 验证：能在 QEMU 中启动、输出信息、接收用户输入并回显

**进行中 / In Progress:**

- 🔧 异常向量表与 VBAR_EL2 设置 / Exception vector table &amp; VBAR_EL2
- 🔧 同步异常与 IRQ 处理 / Sync exception &amp; IRQ handling
- 🔧 GIC 初始化与定时器中断 / GIC init &amp; timer IRQ

**后续 / Next:**

- 物理内存分配器 / Page allocator
- 协作式内核线程 / Cooperative kernel threads
- 定时器中断与抢占式调度 / Timer IRQ & preemptive scheduling
- MMU 与虚拟内存 / MMU & virtual memory
- 系统调用与进程 / Syscall & processes
- virtio-net 网络栈 / virtio-net network stack

***

## 3. 芯片设计 (Chip/) / Chip Design (Chip/)

**中文：**
用 Verilog 实现简单硬件模块（加法器、UART 等），学习数字电路与自制 CPU。

**English:**
Simple hardware modules in Verilog (adder, UART, etc.) for digital circuit & custom CPU learning.

**已完成 / Completed:**

- ✅ 基础模块：加法器 / adder
- ✅ UART 串口：接收、发送 / uart_rx, uart_tx
- 🔧 自制 CPU 设计（进行中）/ Custom CPU design (In progress)

***

## 4. 算法与数据结构 (src/) / Algorithms & Data Structures (src/)

**中文：**
经典算法的 C 语言实现。

**English:**
Classic algorithms implemented in C.

**已完成 / Completed:**

- ✅ 插入排序、归并排序、快速排序、堆排序 / Insertion, Merge, Quick, Heap Sort
- ✅ 最大子数组 / Maximum Subarray
- ✅ 矩阵运算 / Matrix Operations
- ✅ 栈和队列 / Stack &amp; Queue
- ✅ LeetCode 题目：两数之和、有效的括号、设计循环队列

***

## 5. CUDA 并行计算 (CUDA/) / CUDA Parallel Computing (CUDA/)

**中文：**
学习 CUDA 并行编程。

**English:**
Learning CUDA parallel programming.




