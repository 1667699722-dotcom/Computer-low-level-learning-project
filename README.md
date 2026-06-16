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
├── src/         # C 语言算法与数据结构 / Algorithms & Data Structures in C
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

**进行中 / In Progress:**

- 🔧 Token 拆解与边界检查 / Token boundary parsing
- 🔧 数字输出（print_uint/print_int）/ Decimal number output
- 🔧 字符串转数字（atoi_scan）/ String to integer
- 🔧 命令解析与执行 / Command parse & execute
- 🔧 解释器主循环 / Interpreter main loop

详细计划 / Detailed Schedule: [ISA/spec/schedule.md](ISA/spec/schedule.md)

***

## 2. QEMU 裸机 AArch64 (QEMU/) / QEMU Bare-metal AArch64 (QEMU/)

**中文：**
在 QEMU 模拟的 virt 机器上运行裸机程序，学习裸机启动、UART 驱动、异常处理、内存管理、线程/进程调度等底层机制。

**English:**
Bare-metal programming on QEMU-emulated virt machine, learning boot sequence, UART driver, exception handling, memory management, thread/process scheduling, etc.

**已完成 / Completed:**

- ✅ 工具链：clang + ld.lld 裸机编译、ELF 镜像生成、QEMU 参数配置
- ✅ 启动层：`_start` 裸机入口、初始栈设置、PL011 UART MMIO 输出
- ✅ 验证：能在 QEMU 中启动并输出字符串

**进行中 / In Progress:**

- 🔧 EL2 → EL1 异常级别切换 / EL2 to EL1 switch
- 🔧 linker.ld 段布局 / Linker script & section layout
- 🔧 BSS 清零与 kernel_main 跳转 / BSS zeroing & kernel_main
- 🔧 UART 驱动完善（输入/输出完整）/ UART driver (full input/output)
- 🔧 异常向量表与 IRQ 处理 / Exception vector table & IRQ

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

- ✅ 插入排序、归并排序 / Insertion Sort, Merge Sort
- ✅ 最大子数组 / Maximum Subarray
- ✅ 矩阵运算 / Matrix Operations

***

## 5. CUDA 并行计算 (CUDA/) / CUDA Parallel Computing (CUDA/)

**中文：**
学习 CUDA 并行编程。

**English:**
Learning CUDA parallel programming.


***

## 相关文档 / Related Docs

- [ARM64 指令集参考 / ARM64 ISA Reference](ISA/spec/ISA.md)
- [开发进度计划 / Development Schedule](ISA/spec/schedule.md)
- [芯片设计文档 / Chip Design Docs](Chip/design.md)
- [QEMU 裸机启动代码 / QEMU Bare-metal Boot](QEMU/start.s)
- [QEMU 编译运行脚本 / QEMU Build & Run Script](QEMU/run.sh)

