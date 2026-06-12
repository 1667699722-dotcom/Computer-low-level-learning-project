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
├── ISA/         # ARM64 汇编系统编程（进行中）/ ARM64 Assembly System Programming (In Progress)
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

- ✅ 堆结构定义与初始化 / Heap structure definition & init
- ✅ 内存分配（带分割）/ Memory allocation (with splitting)
- ✅ 内存释放（带相邻合并）/ Memory free (with coalescing)
- ✅ Read/Write 系统调用封装 / Read/Write syscall wrappers
- ✅ 字符/字符串 I/O / Character & string I/O
- ✅ 模块化设计 / Modular design
- 🔧 自定义指令解析（进行中）/ Custom instruction parser (In progress)

详细计划 / Detailed Schedule: [ISA/spec/schedule.md](ISA/spec/schedule.md)

***

## 2. 芯片设计 (Chip/) / Chip Design (Chip/)

**中文：**
用 Verilog 实现简单硬件模块（加法器、UART 等），学习数字电路。

**English:**
Simple hardware modules in Verilog (adder, UART, etc.) for digital circuit learning.

***

## 3. 算法与数据结构 (src/) / Algorithms & Data Structures (src/)

**中文：**
经典算法的 C 语言实现。

**English:**
Classic algorithms implemented in C.

**已完成 / Completed:**

- ✅ 插入排序、归并排序 / Insertion Sort, Merge Sort
- ✅ 最大子数组 / Maximum Subarray
- ✅ 矩阵运算 / Matrix Operations

***

## 4. CUDA 并行计算 (CUDA/) / CUDA Parallel Computing (CUDA/)

**中文：**
学习 CUDA 并行编程。

**English:**
Learning CUDA parallel programming.

***

## 快速开始 / Quick Start

```bash
# 汇编系统编程 / Assembly System Programming
cd ISA && ./run.sh

# 算法数据结构 / Algorithms & Data Structures
./run.sh
```

***

## 相关文档 / Related Docs

- [ARM64 指令集参考 / ARM64 ISA Reference](ISA/spec/ISA.md)
- [开发进度计划 / Development Schedule](ISA/spec/schedule.md)
- [芯片设计文档 / Chip Design Docs](Chip/design.md)

