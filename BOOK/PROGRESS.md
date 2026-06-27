# BOOK 进度状态 — 底层学习笔记

> **最后更新**: 2026-06-27
> **总页数**: 84 页（6 章全部完成）
> **编译状态**: ✅ 成功编译
> **PDF 路径**: `BOOK/book.pdf`
> **主文件**: `BOOK/book.tex`

---

## 一、编译命令

在 `BOOK/` 目录下执行：

```bash
cd BOOK
xelatex -interaction=nonstopmode book.tex
```

---

## 二、书籍结构

### 主文件层级

```
book.tex                           # 入口，封面使用 image.png
├── intro/main.tex                 # 第 1 章：项目介绍
├── isa/main.tex                   # 第 2 章：用户态 AArch64 汇编
├── qemu/main.tex                  # 第 3 章：QEMU 裸机 AArch64
│   ├── qemu/sections/01_start.tex
│   ├── qemu/sections/02_run.tex
│   ├── qemu/sections/03_uart.tex
│   ├── qemu/sections/04_hardware.tex
│   ├── qemu/sections/06_memory.tex     # 新增：内存管理
│   ├── qemu/sections/07_exceptions.tex # 新增：异常处理
│   └── qemu/sections/05_roadmap.tex
├── chip/main.tex                  # 第 4 章：Verilog 芯片设计
├── algo/main.tex                  # 第 5 章：C 语言算法与数据结构
└── cuda/main.tex                  # 第 6 章：CUDA 并行计算
```

---

## 三、章节完成度明细

| 章号 | 标题 | 完成度 | 页数 | 备注 |
|------|------|--------|------|------|
| 1 | 项目介绍 | ✅ 完成 | ~3 页 | 目录结构 + 阅读路径建议 |
| 2 | 用户态 AArch64 汇编 | ✅ 完成 | ~15 页 | 系统调用/内存分配器/字符串数组/命令解析/主程序/编译调试/指令解析 |
| 3 | QEMU 裸机 AArch64 | ✅ 完成 | ~22 页 | start.s + run.sh + UART 地址空间 + 硬件资源 + 内存管理 + 异常处理 + 后续路线图 |
| 4 | Verilog 芯片设计 | ✅ 完成 | ~14 页 | adder + uart_tx + uart_rx + FPGA 综合 + 与其他模块关系 |
| 5 | C 语言算法与数据结构 | ✅ 完成 | ~14 页 | 插入排序/归并排序+逆序对/堆排序/最大子数组/矩阵乘法 |
| 6 | CUDA 并行计算 | ✅ 完成 | ~4 页 | 简介/学习重点/与其他模块关系 |

---

## 四、本次更新（2026-06-27）

1. **更新封面**：使用 `image.png` 作为书籍封面
2. **完善第 3 章（QEMU）**：
   - 新增 `06_memory.tex`：内存管理（内存分配器、堆管理）
   - 新增 `07_exceptions.tex`：异常处理（异常向量表、ESR_EL2 解析）
3. **完善第 4 章（Chip）**：
   - 新增 `06_relation.tex`：与其他模块的关系
4. **更新第 1 章（Intro）**：修正项目目录结构描述
5. **编译成功**：生成 84 页 PDF

---

## 五、后续可扩展方向（可选）

- 在 CUDA 章节添加更多实际并行算法示例
- 在算法章节添加栈、队列、链表、树等数据结构
- 添加更多 Verilog 硬件模块示例
- 加入索引功能方便检索

---

## 六、LaTeX 已知要点

### 文档类与宏包
- `\documentclass[12pt,a4paper,openany,UTF8]{book}`
- `\usepackage{ctex}`：中文支持
- `\usepackage{listings}`：代码高亮
- `\usepackage{graphicx}`：图片支持
- `\usepackage{xcolor}` 和 `\usepackage{pagecolor}`：颜色和页面背景色

### 代码风格
预定义了 `asm`、`verilog`、`cstyle`、`bashstyle` 四种代码风格
