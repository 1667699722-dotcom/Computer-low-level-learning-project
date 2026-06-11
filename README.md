
<div align="center">

# 408 Low-Level Learning Project

<p align="center">
  <img src="https://img.shields.io/badge/Assembly-ARM64-0091BD?style=for-the-badge&logo=arm&logoColor=white" alt="ARM64">
  <img src="https://img.shields.io/badge/Verilog-HDL-2C2255?style=for-the-badge&logo=esri&logoColor=white" alt="Verilog">
  <img src="https://img.shields.io/badge/C-Language-A8B9CC?style=for-the-badge&logo=c&logoColor=white" alt="C">
  <img src="https://img.shields.io/badge/CUDA-Parallel-76B900?style=for-the-badge&logo=nvidia&logoColor=white" alt="CUDA">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/macOS-000000?style=flat-square&logo=apple&logoColor=white" alt="macOS">
  <img src="https://img.shields.io/badge/Platform-ARM64-5C2D91?style=flat-square" alt="Platform">
  <img src="https://img.shields.io/badge/status-active-brightgreen?style=flat-square" alt="Status">
</p>

---

### 📌 Project Overview

A comprehensive learning project covering three core areas:

<p align="center">
  <table>
    <tr>
      <th align="center">🏗️ Assembly</th>
      <th align="center">🔧 Chip Design</th>
      <th align="center">🧮 Algorithms</th>
    </tr>
    <tr>
      <td align="center">ARM64 System Programming</td>
      <td align="center">Verilog HDL</td>
      <td align="center">Classic DS &amp; Algo</td>
    </tr>
    <tr>
      <td align="center">🟢 In Progress</td>
      <td align="center">🟡 Planned</td>
      <td align="center">🟢 Core Done</td>
    </tr>
  </table>
</p>

</div>

---

## 📂 Project Structure

```
408/
├── CUDA/              🟦 CUDA Parallel Computing Learning
├── Chip/              🟪 Chip Design (Verilog)
├── ISA/               🟧 ARM64 Assembly System Programming
├── src/               🟩 Algorithms & Data Structures (C)
├── ctos/              ⬜ Others
└── bin/               📦 Compiled Binaries
```

---

## 📦 1. Assembly System Programming (ISA/) 🟧

### 📌 Overview
System-level programming in ARM64 assembly on macOS, including memory management, system calls, custom instruction parser, etc.

### 📂 ISA/ Substructure
```
ISA/
├── spec/              📜 Development Docs & Schedule
│   ├── ISA.md         ARM64 ISA Reference
│   └── schedule.md    Development Schedule
├── src/               💻 Core Source Code
│   ├── memory.s       Memory Allocation
│   ├── memoryfree.s   Memory Free (Coalescing)
│   ├── exit.s         Exit System Call
│   └── ...
├── hello.s            Main Program
└── run.sh             Compile & Run Script
```

### 📌 Feature Progress

| Module | Status | Progress |
| :--- | :---: | :--- |
| Heap Structure Definition | ✅ | ██████████ 100% |
| Memory Management - Init | ✅ | ██████████ 100% |
| Memory Management - Alloc (Splitting) | ✅ | ██████████ 100% |
| Memory Management - Free (Coalescing) | ✅ | ██████████ 100% |
| Write System Call Wrapper | ✅ | ██████████ 100% |
| Read System Call Wrapper | ✅ | ██████████ 100% |
| Single Character/String I/O | ✅ | ██████████ 100% |
| Modular Design | ✅ | ██████████ 100% |
| LLDB Debugging | ✅ | ██████████ 100% |
| Custom Instruction Parser | 🔧 | ████░░░░░░ 40% |

### 📌 Development Schedule
See [ISA/spec/schedule.md](ISA/spec/schedule.md)

---

## 📦 2. Chip Design (Chip/) 🟪

### 📌 Overview
Simple hardware modules in Verilog for digital circuit learning.

### 📂 Chip/ Substructure
```
Chip/
├── src/               🔧 Verilog Source Code
│   ├── adder.v       Adder
│   ├── uart_tx.v     UART Transmit
│   ├── uart_rx.v     UART Receive
│   └── uart_test.v   UART Test
├── PROGRESS.md        Progress Tracker
└── design.md          Design Documentation
```

### 🗺️ Architecture (Mermaid Diagram)

```mermaid
flowchart LR
    A[Input] --> B[UART_RX]
    B --> C[FIFO Buffer]
    C --> D[Processing Core]
    D --> E[ALU]
    D --> F[Register File]
    E --> G[UART_TX]
    G --> H[Output]

    style A fill:#e1f5fe,stroke:#01579b
    style B fill:#fff9c4,stroke:#f57f17
    style D fill:#c8e6c9,stroke:#1b5e20
    style H fill:#ffebee,stroke:#b71c1c
```

---

## 📦 3. Algorithms & Data Structures (src/) 🟩

### 📌 Overview
Classic algorithms & data structures in C.

### 📂 src/ Substructure
```
src/
├── sort/              🔄 Sorting Algorithms
│   ├── merge_sort.c   Merge Sort
│   └── inserton_sort.c Insertion Sort
├── matrix/            📐 Matrix Operations
└── findmaximumsubarray/ 📊 Maximum Subarray
```

### 📌 Completed Algorithms

| Algorithm | Status | Complexity |
| :--- | :---: | :--- |
| Insertion Sort | ✅ | O(n²) |
| Merge Sort | ✅ | O(n log n) |
| Maximum Subarray | ✅ | O(n) |
| Matrix Operations | ✅ | O(n³) |

---

## 📦 4. CUDA Parallel Computing (CUDA/) 🟦

### 📌 Overview
CUDA parallel programming learning.

---

## 🚀 Quick Start

### 1. Assembly System Programming (ISA/)
```bash
cd ISA
./run.sh
```

### 2. Algorithms & Data Structures (src/)
```bash
./run.sh
```

---

## � Project Statistics

```mermaid
pie title Code Distribution (Estimated)
    "Assembly (ISA/)" : 40
    "C (src/)" : 30
    "Verilog (Chip/)" : 20
    "CUDA" : 10
```

```mermaid
gantt
    title Development Timeline
    dateFormat  YYYY-MM-DD
    section Assembly
    Memory Management     :done, 2025-01-01, 30d
    System Calls          :done, 2025-02-01, 15d
    Custom Instructions   :active, 2025-02-15, 30d
    section Chip Design
    Basic Modules         :2025-03-01, 60d
    section Algorithms
    Core Algo             :done, 2025-01-01, 60d
```

---

## 🛠️ Tech Stack

<p align="left">
  <img src="https://img.shields.io/badge/Apple_MacBook_Pro-000000?style=for-the-badge&logo=apple&logoColor=white" alt="Apple">
  <img src="https://img.shields.io/badge/ARM64_Assembly-0091BD?style=for-the-badge&logo=arm&logoColor=white" alt="ARM64">
  <img src="https://img.shields.io/badge/C_Programming-A8B9CC?style=for-the-badge&logo=c&logoColor=white" alt="C">
  <img src="https://img.shields.io/badge/Verilog-2C2255?style=for-the-badge&logoColor=white" alt="Verilog">
  <img src="https://img.shields.io/badge/CUDA-76B900?style=for-the-badge&logo=nvidia&logoColor=white" alt="CUDA">
  <img src="https://img.shields.io/badge/LLDB-4D4D4D?style=for-the-badge&logoColor=white" alt="LLDB">
  <img src="https://img.shields.io/badge/Git-F05032?style=for-the-badge&logo=git&logoColor=white" alt="Git">
</p>

---

## �📚 Related Docs
- 📖 [ARM64 ISA Reference](ISA/spec/ISA.md)
- 📅 [Development Schedule](ISA/spec/schedule.md)
- 📐 [Chip Design Docs](Chip/design.md)

---

<div align="center">

---

## 📄 License

See root directory LICENSE file

</div>
