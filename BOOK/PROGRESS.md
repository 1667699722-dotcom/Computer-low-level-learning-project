# BOOK 进度状态 — 底层学习笔记

> **最后更新**: 2026-06-17
> **总页数**: 62 页（5 章已纳入编译）
> **编译状态**: ✅ 干净无警告无错误
> **PDF 路径**: `BOOK/book.pdf`
> **主文件**: `BOOK/book.tex`

---

## 一、编译命令

在 `BOOK/` 目录下执行：

```bash
cd BOOK
xelatex -interaction=nonstopmode book.tex
```

检查是否有警告/错误：

```bash
xelatex -interaction=nonstopmode book.tex 2>&1 | grep -iE "error|undefined|overfull|missing"
```

> 如果 grep 没有任何输出（返回码 1），就是干净的。

---

## 二、书籍结构

### 主文件层级

```
book.tex                           ← 入口，定义文档类、代码风格、标题
├── intro/main.tex                 ← 第 1 章：项目介绍
│   └── intro/sections/02_structure.tex
│   └── intro/sections/03_path.tex
├── isa/main.tex                   ← 第 2 章：用户态 AArch64 汇编
│   ├── isa/sections/01_syscall.tex       ← sys_write / sys_read
│   ├── isa/sections/02_memory.tex        ← 4KB 堆分配器（首次适配 + 合并）
│   ├── isa/sections/03_strcopy.tex       ← copy_string：用户态 strcpy
│   ├── isa/sections/04_string_array.tex  ← 平行数组存指针+长度
│   ├── isa/sections/05_parse.tex         ← find_two_spaces：命令解析
│   ├── isa/sections/06_main.tex          ← 主程序循环流程图
│   └── isa/sections/07_build.tex         ← clang 编译 + lldb 调试
├── qemu/main.tex                  ← 第 3 章：QEMU 裸机 AArch64
│   ├── qemu/sections/01_start.tex        ← start.s：UART 输出
│   ├── qemu/sections/02_run.tex          ← run.sh：交叉编译脚本
│   └── qemu/sections/03_uart.tex         ← 地址空间与 PL011 UART
├── chip/main.tex                  ← 第 4 章：Verilog 芯片设计
│   ├── chip/sections/01_concepts.tex     ← Verilog RTL 基本概念
│   ├── chip/sections/02_adder.tex        ← 64 位字节加法器
│   ├── chip/sections/03_uart_tx.tex      ← UART 发送器
│   ├── chip/sections/04_uart_rx.tex      ← UART 接收器
│   └── chip/sections/05_fpga.tex         ← 综合到 FPGA
└── algo/main.tex                  ← 第 5 章：C 语言算法与数据结构（*本日新增*）
    ├── algo/sections/02_insertion_sort.tex
    ├── algo/sections/03_merge_sort.tex
    ├── algo/sections/04_max_subarray.tex
    ├── algo/sections/05_matrix.tex
    └── algo/sections/06_relation.tex
```

### 已存在但尚未在 book.tex 引用的章节

| 目录 | 状态 | 说明 |
|------|------|------|
| `cuda/main.tex` | 🔲 骨架（3 个 section 文件） | 第 6 章：CUDA 并行计算，框架已搭，无实际内容 |

要把某章加入书籍，只需在 `book.tex` 的 `\end{document}` 之前加一行：

```tex
\input{cuda/main}
```

---

## 三、章节完成度明细

| 章号 | 标题 | 完成度 | 页数 | 备注 |
|------|------|--------|------|------|
| 1 | 项目介绍 | ✅ 完成 | ~3 页 | 目录结构 + 阅读路径建议 |
| 2 | 用户态 AArch64 汇编 | ✅ 完成 | ~15 页 | 系统调用/内存分配器/字符串数组/命令解析/主程序/编译调试 |
| 3 | QEMU 裸机 AArch64 | ✅ 完成 | ~10 页 | start.s + run.sh + UART 地址空间 |
| 4 | Verilog 芯片设计 | ✅ 完成 | ~12 页 | adder + uart_tx + uart_rx + FPGA 综合 |
| 5 | C 语言算法与数据结构 | ✅ 完成 | ~12 页 | 插入排序/归并排序+逆序对/最大子数组/矩阵乘法 |
| 6 | CUDA 并行计算 | 🔲 骨架 | 未编译 | 需要补全 cuda/sections/ 内容 |

### 已删除的旧 section 文件（清理记录）

以下过时文件已在本次编辑中删除，避免它们的路径出现在文档中被误以为仍在使用：

- `BOOK/algo/sections/01_intro.tex` （旧版）
- `BOOK/algo/sections/02_completed.tex` （旧版）
- `BOOK/algo/sections/03_relation.tex` （旧版）

其他章节（chip/qemu/isa）下仍保留有一些以 `01_intro / 02_completed / 03_wip` 命名的旧 section 骨架文件，它们**不被任何 main.tex 引用**，不影响编译。如果需要彻底清理，可 grep 出未被引用的文件删除。

---

## 四、外部源码文件引用

书籍中的代码清单通过 `\lstinputlisting` 直接引用项目根目录的源码，这样代码更新后重新编译 PDF 就能自动同步。

| 章节 | 源码文件 | 引用方式 |
|------|----------|----------|
| 第 5 章 — 归并排序 | `src/sort/merge_sort.c` | `\lstinputlisting` |
| 第 5 章 — 最大子数组 | `src/findmaximumsubarray/1.c`、`2.c` | `\lstinputlisting` |
| 第 5 章 — 矩阵乘法 | `src/matrix/1.c` | `\lstinputlisting` |

> 注意：相对路径是从 `BOOK/` 目录向上一层，即 `../src/...`。如果移动了 BOOK 目录位置，需要同步修改路径。

### 空的源码文件（等待实现）

| 文件 | 说明 |
|------|------|
| `src/sort/heapsort.c` | heapsort 存根（git commit `b07815f` 写的） |

---

## 五、LaTeX 已知要点

### 1. 文档类与宏包

- `\documentclass[12pt,a4paper,openany,UTF8]{book}`
- `\usepackage{ctex}` — 中文支持
- `\usepackage{listings}` + `\lstset{breaklines=true}` — 代码块，*且会自动换行*
- `\usepackage{amsmath,amssymb}` — 数学公式
- `\usepackage{titlesec}` — 章节标题美化
- `\usepackage{geometry}` — 页面边距（上下左右 2.5cm）

### 2. 代码风格定义

`book.tex` 顶部预定义了 4 种 `\lstdefinestyle`：

| Style 名称 | 用途 | 示例 |
|------------|------|------|
| `asm` | AArch64 汇编 | `\begin{lstlisting}[style=asm]` |
| `verilog` | Verilog RTL | `\begin{lstlisting}[style=verilog]` |
| `cstyle` | C 代码 | `\begin{lstlisting}[style=cstyle]` |
| `bashstyle` | Shell 脚本 | `\begin{lstlisting}[style=bashstyle]` |

### 3. 编译中常见需要注意的字符

| 问题 | 解决 |
|------|------|
| 行内 `\texttt{}` 中写 `#` | 必须转义：`\texttt{\#}` |
| 行内 `\texttt{}` 中写 `{ }` | 转义：`\texttt{\{ \}}` |
| 地址中的下划线 `0x4000_0000` | `\texttt{0x4000\_0000}` |
| 超长的 `\texttt{}` 不换行导致 overfull | 拆成多个短的 `\texttt{}`，或放进 lstlisting 环境 |
| 数学公式中写中文符号 ｜ 不要包在 `$...$` 里，单独拆成中文段落 |
| Unicode 方框字符 ├─└ | LaTeX 等宽字体没有，换用 ASCII 的 `+--` 等 |

### 4. 调试流程

当编译出警告/错误时，按以下顺序排查：

1. 运行 `xelatex -interaction=nonstopmode book.tex`，关注最底部的 `! Undefined control sequence` / `Overfull \hbox`
2. 看 `book.log` 文件定位行号
3. 用 `grep -n "关键词" *.tex` 找到具体文件
4. 修改后再次编译，直到 grep 无输出

---

## 六、Git 仓库状态（2026-06-17）

```
分支: main（与 origin/main 同步）
最近提交: b07815f  "Add heapsort stub, update README and run.sh"
BOOK/ 目录状态: 尚未 git add，是 untracked files
```

### 把 BOOK 纳入版本控制（下次要做）

```bash
cd /Users/liuhuachao/Documents/Cline/408
echo "book.aux"        >> .gitignore
echo "book.log"         >> .gitignore
echo "book.out"         >> .gitignore
echo "book.toc"         >> .gitignore
echo "book.pdf"         >> .gitignore   # 可选：PDF 不入库，需要时重新编译
git add BOOK/
git commit -m "Add LaTeX book: 5 chapters, 62 pages, clean compile"
```

> **关于 git 信息**: 本助手可以用 `git log --oneline`、`git diff`、`git status` 查看当日完成的任务。但如果 BOOK/ 本身没被提交，git 就无法追踪到「今天写了哪些 LaTeX 文件」。建议每完成一大章就 commit 一次，下次打开就能用 `git show HEAD --stat` 快速看到上次写到哪。

---

## 七、下阶段可做的事（优先级从高到低）

### 🔴 高优先级 — 对完善现有内容最有帮助

1. **补第 5 章缺失的 heapsort 节**：`src/sort/heapsort.c` 现在是空文件，写完源码后，在 `algo/main.tex` 里加一个 `\input{algo/sections/07_heapsort}`
2. **第 6 章 CUDA 填入实内容**：`cuda/main.tex` 已经有框架，`cuda/sections/01_intro.tex` 等是极简骨架，需要用实际 CUDA 代码和讲解替换
3. **清理未被引用的旧 section 骨架**：`grep -l "未被 main.tex 引用"` 或逐个检查 `find BOOK -name "*.tex" -type f` 然后对照各 main.tex 的 `\input` 行

### 🟡 中优先级 — 体验优化

4. **封面设计美化**：当前封面是简洁文字版，可以加图标、副标题排版、二维码等
5. **加目录之后的"本书总览图"**：画一张 4 个模块（Chip/ISA/QEMU/Algo/CUDA）怎么彼此关联的示意图
6. **章节内加一些 TODO 标记**：在你打算继续扩展的位置（例如 "未来补：图搜索算法"）留好占位，下次打开就能接着写
7. **交叉引用**：在第 5 章提到汇编时加 `\ref{...}` 指向第 2 章对应 section，让读者可以跳转

### 🟢 低优先级 — 长期优化

8. **章节编号一致性**：当前 section 文件名编号从 `01` 开始，但如果中间增删 section，编号就不再连续。可以接受不连续编号，也可以考虑换成语义命名（`memory_allocator.tex` 而非 `02_memory.tex`）
9. **统一代码风格宏**：在某些章节用了 `\lstinputlisting[firstline=X,lastline=Y]` 来只截取代码片段，建议统一这种引用方式
10. **加入索引**：最后章节写完后加 `\usepackage{makeidx}` + `\makeindex` + `\printindex`，让读者可以关键词搜索

---

## 八、快速开始新章节的模板

每次写新章节时，复制以下最小可用模板：

```latex
% 第 N 章 — XXX 模块
\chapter{XXX 模块}

对应项目目录：\texttt{xxx/}。

\section{小节标题}

内容...

\input{xxx/sections/01_something}
\input{xxx/sections/02_something}
```

然后在 `book.tex` 末尾 `\end{document}` 之前加：

```latex
\input{xxx/main}
```

编译测试后，在本文件更新章节完成度表格。

---

## 九、编译最后一次验证（2026-06-17）

```
Output written on book.pdf (62 pages).
grep "error|undefined|overfull|missing"  →  无输出 ✅
```

下次从这里继续。
