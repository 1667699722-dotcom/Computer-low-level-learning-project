# 指令集
## 简易系统
一系列指令的组合构成操作系统

### 所需指令集

以下是构建简易操作系统所需的核心指令集（最小化子集）：

#### 基础数据传送
| 指令格式 | 功能说明 | 用途 |
| :------: | -------- | ---- |
| MOV Xd,Xn | 寄存器间传送 | 数据复制、初始化 |
| MOV Xd,#imm | 加载立即数 | 常量初始化 |
| MOVZ Xd,#imm,LSL #shift | 加载16位立即数 | 大常量构造 |
| MOVK Xd,#imm,LSL #shift | 拼接立即数 | 与MOVZ配合使用构造64位常量 |

#### 算术运算
| 指令格式 | 功能说明 | 用途 |
| :------: | -------- | ---- |
| ADD Xd,Xn,Xm | 64位加法 | 地址计算、数值运算 |
| ADD Xd,Xn,#imm | 加立即数 | 地址偏移、计数 |
| SUB Xd,Xn,Xm | 64位减法 | 地址计算、数值运算 |
| SUB Xd,Xn,#imm | 减立即数 | 地址偏移、计数 |
| ADD Xd,Xn,Xm,LSL #shift | 移位后加 | 数组索引计算 |

#### 逻辑运算
| 指令格式 | 功能说明 | 用途 |
| :------: | -------- | ---- |
| AND Xd,Xn,Xm | 按位与 | 位掩码、清除位 |
| AND Xd,Xn,#imm | 按位与立即数 | 提取特定位 |
| ORR Xd,Xn,Xm | 按位或 | 设置位、合并位 |
| ORR Xd,Xn,#imm | 按位或立即数 | 设置特定位 |
| EOR Xd,Xn,Xm | 按位异或 | 位翻转、临时值 |
| LSL Xd,Xn,#shift | 逻辑左移 | 乘2^n、位操作 |
| LSR Xd,Xn,#shift | 逻辑右移 | 除2^n、位操作 |

#### 比较与条件码
| 指令格式 | 功能说明 | 用途 |
| :------: | -------- | ---- |
| CMP Xn,Xm | 比较寄存器 | 条件判断基础 |
| CMP Xn,#imm | 比较立即数 | 与常量比较 |
| SUBS Xd,Xn,Xm | 减法并设标志 | 比较并保存结果 |

#### 跳转分支
| 指令格式 | 功能说明 | 用途 |
| :------: | -------- | ---- |
| B label | 无条件跳转 | 循环、程序流控制 |
| B.cond label | 条件跳转 | if/else、循环判断 |
| BL label | 调用子程序 | 函数调用 |
| RET | 函数返回 | 从函数返回 |
| CBZ Xn,label | 为零跳转 | 简化零判断 |
| CBNZ Xn,label | 非零跳转 | 简化非零判断 |

#### 加载存储
| 指令格式 | 功能说明 | 用途 |
| :------: | -------- | ---- |
| LDR Xd,[Xn] | 加载64位 | 从内存读数据 |
| LDR Xd,[Xn,#imm] | 基址加偏移加载 | 结构体/数组访问 |
| STR Xd,[Xn] | 存储64位 | 写数据到内存 |
| STR Xd,[Xn,#imm] | 基址加偏移存储 | 结构体/数组写入 |
| LDR Wd,[Xn] | 加载32位 | 读取32位数据 |
| STR Wd,[Xn] | 存储32位 | 写入32位数据 |
| LDRB Wd,[Xn] | 加载8位 | 读取字节 |
| STRB Wd,[Xn] | 存储8位 | 写入字节 |
| LDP Xd1,Xd2,[Xn] | 加载双字对 | 一次读两个寄存器 |
| STP Xd1,Xd2,[Xn] | 存储双字对 | 一次写两个寄存器 |

#### 栈操作
| 指令格式 | 功能说明 | 用途 |
| :------: | -------- | ---- |
| STP X29,X30,[SP,#-16]! | 保存帧指针和LR | 函数序言 |
| MOV X29,SP | 设置帧指针 | 建立栈帧 |
| LDP X29,X30,[SP],#16 | 恢复帧指针和LR | 函数尾声 |
| SUB SP,SP,#imm | 分配栈空间 | 局部变量空间 |
| ADD SP,SP,#imm | 释放栈空间 | 回收栈空间 |

#### 地址计算
| 指令格式 | 功能说明 | 用途 |
| :------: | -------- | ---- |
| ADR Xd,label | PC相对地址 | 小范围地址加载 |
| ADRP Xd,label | 页相对地址 | 全局变量/函数地址 |

#### 系统相关
| 指令格式 | 功能说明 | 用途 |
| :------: | -------- | ---- |
| NOP | 空操作 | 延迟、对齐 |
| HLT #imm | 停止 | 停机、调试 |
| DSB SY | 数据同步屏障 | 确保内存操作完成 |
| ISB | 指令同步屏障 | 刷新流水线 |

#### 简易OS各模块所需指令
| OS模块 | 核心指令 |
| :----: | -------- |
| **启动代码** | MOV, ADRP, LDR, BL, B |
| **内存管理** | ADD, SUB, LDR, STR, CMP, B.cond |
| **进程切换** | STP, LDP, MOV, B |
| **中断处理** | MRS, MSR, BL, RET |
| **系统调用** | SVC, MOV, LDR, STR |
| **简单驱动** | LDR, STR, AND, ORR, CMP, B.cond |
| **任务调度** | CMP, B.cond, MOV, LDR, STR |

#### 最小指令子集（约50条）
- **数据传送**: MOV, MOVZ, MOVK
- **算术**: ADD, SUB
- **逻辑**: AND, ORR, EOR, LSL, LSR
- **比较**: CMP, SUBS
- **分支**: B, B.cond, BL, RET, CBZ, CBNZ
- **加载存储**: LDR, STR, LDP, STP, LDRB, STRB, LDRW, STRW
- **栈**: STP, LDP, SUB, ADD
- **地址**: ADR, ADRP
- **系统**: NOP, HLT, DSB, ISB

> 注：以上约50条指令足够构建一个支持内存管理、任务调度、简单驱动的简易操作系统！

## APPLE M4（ARMv9.2‑A / AArch64）指令集

### 算术运算指令
| 指令格式 | 功能说明 | 代码示例 | 备注 |
| :------: | -------- | -------- | ---- |
| ADD Xd,Xn,Xm | 64位寄存器加法 | ADD X2,X0,X1 | X2 = X0 + X1 |
| ADD Xd,Xn,#imm | 寄存器加立即数 | ADD X0,X0,#10 | 立即数范围受限 |
| ADDS Xd,Xn,Xm | 64位加法，影响标志位 | ADDS X2,X0,X1 | 影响 N,Z,C,V |
| ADC Xd,Xn,Xm | 带进位加法 | ADC X3,X1,X2 | X3 = X1 + X2 + C |
| ADCS Xd,Xn,Xm | 带进位加法，影响标志位 | ADCS X3,X1,X2 | 影响 N,Z,C,V |
| SUB Xd,Xn,Xm | 64位寄存器减法 | SUB X3,X2,X1 | X3 = X2 - X1 |
| SUB Xd,Xn,#imm | 寄存器减立即数 | SUB X0,X0,#5 | X0 = X0 - 5 |
| SUBS Xd,Xn,Xm | 64位减法，影响标志位 | SUBS X3,X2,X1 | 影响 N,Z,C,V |
| SBC Xd,Xn,Xm | 带借位减法 | SBC X4,X3,X2 | X4 = X3 - X2 - !C |
| SBCS Xd,Xn,Xm | 带借位减法，影响标志位 | SBCS X4,X3,X2 | 影响 N,Z,C,V |
| MUL Xd,Xn,Xm | 64位整数乘法 | MUL X4,X0,X1 | 普通整数乘指令 |
| MUL Wd,Wn,Wm | 32位整数乘法 | MUL W4,W0,W1 | 32位乘法 |
| SMULL Xd,Wn,Wm | 有符号长乘法 | SMULL X2,W0,W1 | X2 = W0 * W1 (64位结果) |
| UMULL Xd,Wn,Wm | 无符号长乘法 | UMULL X2,W0,W1 | X2 = W0 * W1 (64位结果) |
| SDIV Xd,Xn,Xm | 有符号除法 | SDIV X5,X4,X2 | X5 = X4 / X2 |
| UDIV Xd,Xn,Xm | 无符号除法 | UDIV X5,X4,X2 | X5 = X4 / X2 (无符号) |
| NEG Xd,Xn | 取负 | NEG X3,X1 | X3 = -X1 |
| NEGS Xd,Xn | 取负，影响标志位 | NEGS X3,X1 | 影响 N,Z,C,V |
| CMN Xn,Xm | 比较负数 | CMN X0,X1 | 相当于 ADDS ZR,X0,X1 |
| CMN Xn,#imm | 比较负立即数 | CMN X0,#10 | 相当于 ADDS ZR,X0,#10 |

### 逻辑传送/位操作指令
| 指令格式 | 功能说明 | 代码示例 | 备注 |
| :------: | -------- | -------- | ---- |
| MOV Xd,#imm | 加载立即数到寄存器 | MOV X0,#20 | 常用初始化寄存器 |
| MOV Xd,Xn | 寄存器间传送 | MOV X2,X0 | X2 = X0 |
| MOVZ Xd,#imm,LSL #shift | 加载立即数（零扩展） | MOVZ X0,#0x1234,LSL #16 | 加载16位立即数 |
| MOVK Xd,#imm,LSL #shift | 加载立即数（保持高位） | MOVK X0,#0x5678,LSL #0 | 与MOVZ配合使用 |
| MOVN Xd,#imm,LSL #shift | 加载立即数（按位取反） | MOVN X0,#0xFFFF | X0 = ~0xFFFF |
| AND Xd,Xn,Xm | 按位与 | AND X1,X0,X2 | 逻辑运算，影响状态位 |
| AND Xd,Xn,#imm | 按位与立即数 | AND X0,X0,#0xFF | 按位与立即数 |
| ANDS Xd,Xn,Xm | 按位与，影响标志位 | ANDS X1,X0,X2 | 影响 N,Z |
| ORR Xd,Xn,Xm | 按位或 | ORR X1,X0,X2 | AArch64 或指令为 ORR |
| ORR Xd,Xn,#imm | 按位或立即数 | ORR X0,X0,#0xFF | 按位或立即数 |
| EOR Xd,Xn,Xm | 按位异或 | EOR X1,X0,X2 | 常用于位翻转 |
| EOR Xd,Xn,#imm | 按位异或立即数 | EOR X0,X0,#0xFF | 按位异或立即数 |
| BIC Xd,Xn,Xm | 位清除 | BIC X1,X0,X2 | X1 = X0 & ~X2 |
| BICS Xd,Xn,Xm | 位清除，影响标志位 | BICS X1,X0,X2 | 影响 N,Z |
| LSL Xd,Xn,#shift | 逻辑左移 | LSL X1,X0,#4 | X1 = X0 << 4 |
| LSL Xd,Xn,Xm | 逻辑左移（寄存器） | LSL X1,X0,X2 | X1 = X0 << X2 |
| LSR Xd,Xn,#shift | 逻辑右移 | LSR X1,X0,#4 | X1 = X0 >> 4 (无符号) |
| LSR Xd,Xn,Xm | 逻辑右移（寄存器） | LSR X1,X0,X2 | X1 = X0 >> X2 |
| ASR Xd,Xn,#shift | 算术右移 | ASR X1,X0,#4 | X1 = X0 >> 4 (有符号) |
| ASR Xd,Xn,Xm | 算术右移（寄存器） | ASR X1,X0,X2 | X1 = X0 >> X2 |
| ROR Xd,Xn,#shift | 循环右移 | ROR X1,X0,#4 | X1 = X0 循环右移4位 |
| ROR Xd,Xn,Xm | 循环右移（寄存器） | ROR X1,X0,X2 | X1 = X0 循环右移X2位 |
| CLZ Xd,Xn | 前导零计数 | CLZ X1,X0 | 统计X0前导零的个数 |
| CLS Xd,Xn | 前导符号位计数 | CLS X1,X0 | 统计X0前导符号位的个数 |
| RBIT Xd,Xn | 位反转 | RBIT X1,X0 | X1 = X0 位反转 |
| REV Xd,Xn | 字节序反转 | REV X1,X0 | X1 = X0 字节序反转 |
| REV16 Xd,Xn | 半字字节序反转 | REV16 X1,X0 | 每半字内部字节反转 |
| REV32 Xd,Xn | 字字节序反转 | REV32 X1,X0 | 每字内部字节反转 |
| BFI Xd,Xn,#lsb,#width | 位域插入 | BFI X1,X0,#0,#8 | 将X0的[7:0]插入X1的[7:0] |
| BFXIL Xd,Xn,#lsb,#width | 位域提取并插入 | BFXIL X1,X0,#4,#8 | 提取X0[11:4]插入X1[7:0] |
| UBFX Xd,Xn,#lsb,#width | 无符号位域提取 | UBFX X1,X0,#4,#8 | 提取X0[11:4]，零扩展 |
| SBFX Xd,Xn,#lsb,#width | 有符号位域提取 | SBFX X1,X0,#4,#8 | 提取X0[11:4]，符号扩展 |
| BFC Xd,#lsb,#width | 位域清零 | BFC X1,#4,#8 | 将X1[11:4]清零 |

### 跳转分支指令
| 指令格式 | 功能说明 | 代码示例 | 备注 |
| :------: | -------- | -------- | ---- |
| B label | 无条件跳转 | B loop | 短跳转指令 |
| B.cond label | 条件跳转 | B.EQ label | 根据标志位跳转 |
| BL label | 调用子程序 | BL func | 返回地址存入 X30 |
| BLR Xn | 调用寄存器指向的子程序 | BLR X3 | 返回地址存入 X30 |
| RET | 函数返回 | RET | 从 X30 取地址跳转 |
| RET Xn | 从指定寄存器返回 | RET X2 | 从 X2 取地址跳转 |
| BR Xn | 无条件跳转到寄存器 | BR X3 | 跳转到 X3 指向的地址 |
| CBZ Xn,label | 比较为零跳转 | CBZ X0,label | X0 == 0 时跳转 |
| CBZ Wn,label | 32位比较为零跳转 | CBZ W0,label | W0 == 0 时跳转 |
| CBNZ Xn,label | 比较非零跳转 | CBNZ X0,label | X0 != 0 时跳转 |
| CBNZ Wn,label | 32位比较非零跳转 | CBNZ W0,label | W0 != 0 时跳转 |
| TBZ Xn,#bit,label | 测试位为零跳转 | TBZ X0,#0,label | X0[0] == 0 时跳转 |
| TBNZ Xn,#bit,label | 测试位非零跳转 | TBNZ X0,#0,label | X0[0] != 0 时跳转 |
| SVC #0x80 | macOS 系统调用 | SVC #0x80 | M4/macOS 专属调用指令 |
| HVC #imm | Hypervisor调用 | HVC #0 | EL2调用 |
| SMC #imm | 安全监控调用 | SMC #0 | EL3调用 |
| BRK #imm | 断点指令 | BRK #0 | 触发调试异常 |
| HLT #imm | 停止指令 | HLT #0 | 停止执行 |

### 比较指令
| 指令格式 | 功能说明 | 代码示例 | 备注 |
| :------: | -------- | -------- | ---- |
| CMP Xn,Xm | 比较寄存器 | CMP X0,X1 | X0 - X1，影响标志位 |
| CMP Xn,#imm | 比较立即数 | CMP X0,#10 | X0 - 10，影响标志位 |
| CMP Wn,Wm | 32位比较 | CMP W0,W1 | W0 - W1，影响标志位 |
| CMP Wn,#imm | 32位比较立即数 | CMP W0,#10 | W0 - 10，影响标志位 |
| CMN Xn,Xm | 比较负数 | CMN X0,X1 | X0 + X1，影响标志位 |
| CMN Xn,#imm | 比较负立即数 | CMN X0,#10 | X0 + 10，影响标志位 |
| CCMP Xn,Xm,#nzcv,#cond | 条件比较 | CCMP X0,X1,#0,EQ | 条件满足时比较 |
| CCMP Xn,#imm,#nzcv,#cond | 条件比较立即数 | CCMP X0,#5,#0,EQ | 条件满足时比较 |
| CCMN Xn,Xm,#nzcv,#cond | 条件比较负数 | CCMN X0,X1,#0,EQ | 条件满足时比较 |
| CCMN Xn,#imm,#nzcv,#cond | 条件比较负立即数 | CCMN X0,#5,#0,EQ | 条件满足时比较 |
| TST Xn,Xm | 测试位 | TST X0,X1 | X0 & X1，影响标志位 |
| TST Xn,#imm | 测试位立即数 | TST X0,#1 | X0 & 1，影响标志位 |

### 加载存储指令
| 指令格式 | 功能说明 | 代码示例 | 备注 |
| :------: | -------- | -------- | ---- |
| LDR Xd,[Xn] | 从内存读8字节 | LDR X0,[SP] | 加载64位数据 |
| LDR Xd,[Xn,#imm] | 基址加偏移加载 | LDR X0,[SP,#16] | SP+16 地址加载 |
| LDR Xd,[Xn,Xm] | 基址加变址加载 | LDR X0,[SP,X1] | SP+X1 地址加载 |
| LDR Xd,[Xn,Xm,LSL #shift] | 变址移位加载 | LDR X0,[SP,X1,LSL #3] | SP+X1*8 地址 |
| LDR Xd,[Xn,#imm]! | 前变基加载 | LDR X0,[SP,#16]! | 加载后SP += 16 |
| LDR Xd,[Xn],#imm | 后变基加载 | LDR X0,[SP],#16 | 加载后SP += 16 |
| LDR Wd,[Xn] | 加载32位数据 | LDR W0,[SP] | 零扩展到64位 |
| LDRH Wd,[Xn] | 加载16位数据 | LDRH W0,[SP] | 零扩展到64位 |
| LDRB Wd,[Xn] | 读取1字节 | LDRB W1,[X0] | 字节加载，零扩展 |
| LDRSB Xd,[Xn] | 加载有符号字节 | LDRSB X0,[SP] | 符号扩展到64位 |
| LDRSH Xd,[Xn] | 加载有符号半字 | LDRSH X0,[SP] | 符号扩展到64位 |
| LDRSW Xd,[Xn] | 加载有符号字 | LDRSW X0,[SP] | 符号扩展到64位 |
| STR Xd,[Xn] | 向内存存8字节 | STR X0,[SP] | 存储64位数据 |
| STR Xd,[Xn,#imm] | 基址加偏移存储 | STR X0,[SP,#16] | 存储到SP+16 |
| STR Xd,[Xn,Xm] | 基址加变址存储 | STR X0,[SP,X1] | 存储到SP+X1 |
| STR Xd,[Xn,#imm]! | 前变基存储 | STR X0,[SP,#16]! | 存储后SP += 16 |
| STR Xd,[Xn],#imm | 后变基存储 | STR X0,[SP],#16 | 存储后SP += 16 |
| STR Wd,[Xn] | 存储32位数据 | STR W0,[SP] | 存储低32位 |
| STRH Wd,[Xn] | 存储16位数据 | STRH W0,[SP] | 存储低16位 |
| STRB Wd,[Xn] | 存储1字节 | STRB W1,[X0] | 存储低8位 |
| LDUR Xd,[Xn,#imm] | 非特权加载 | LDUR X0,[SP,#8] | 非特权模式加载 |
| STUR Xd,[Xn,#imm] | 非特权存储 | STUR X0,[SP,#8] | 非特权模式存储 |
| LDP Xd1,Xd2,[Xn] | 加载双字对 | LDP X0,X1,[SP] | 加载两个64位寄存器 |
| STP Xd1,Xd2,[Xn] | 存储双字对 | STP X0,X1,[SP] | 存储两个64位寄存器 |
| LDP Xd1,Xd2,[Xn,#imm]! | 前变基加载双字对 | LDP X0,X1,[SP,#16]! | 加载后SP += 16 |
| STP Xd1,Xd2,[Xn,#imm]! | 前变基存储双字对 | STP X0,X1,[SP,#16]! | 存储后SP += 16 |
| LDXR Xd,[Xn] | 独占加载 | LDXR X0,[X1] | 独占读取用于原子操作 |
| STXR Wd,Xd,[Xn] | 独占存储 | STXR W0,X1,[X2] | 尝试存储，W0=0成功 |
| LDAXR Xd,[Xn] | 获取独占加载 | LDAXR X0,[X1] | 带获取语义的独占加载 |
| STLXR Wd,Xd,[Xn] | 释放独占存储 | STLXR W0,X1,[X2] | 带释放语义的独占存储 |
| LDAR Xd,[Xn] | 获取加载 | LDAR X0,[X1] | 带获取语义的加载 |
| STLR Xd,[Xn] | 释放存储 | STLR X0,[X1] | 带释放语义的存储 |
| CAS Xd,Xt,[Xn] | 比较并交换 | CAS X0,X1,[X2] | 原子比较交换 |
| SWP Xd,Xt,[Xn] | 交换 | SWP X0,X1,[X2] | 原子交换内存与寄存器 |

### 栈操作/函数调用指令
| 指令格式 | 功能说明 | 代码示例 | 备注 |
| :------: | -------- | -------- | ---- |
| STP X29,X30,[SP,#-16]! | 保存帧指针和返回地址 | STP X29,X30,[SP,#-16]! | 函数序言 |
| MOV X29,SP | 设置帧指针 | MOV X29,SP | 设置当前栈指针为帧指针 |
| LDP X29,X30,[SP],#16 | 恢复帧指针和返回地址 | LDP X29,X30,[SP],#16 | 函数尾声 |
| SUB SP,SP,#imm | 分配栈空间 | SUB SP,SP,#32 | 分配32字节栈空间 |
| ADD SP,SP,#imm | 释放栈空间 | ADD SP,SP,#32 | 释放32字节栈空间 |
| STR Xn,[SP,#imm] | 保存寄存器到栈 | STR X0,[SP,#8] | 保存X0到栈 |
| LDR Xn,[SP,#imm] | 从栈恢复寄存器 | LDR X0,[SP,#8] | 从栈恢复X0 |

### 地址计算指令
| 指令格式 | 功能说明 | 代码示例 | 备注 |
| :------: | -------- | -------- | ---- |
| ADR Xd,label | PC相对地址 | ADR X0,label | 小范围地址加载 |
| ADRP Xd,label | 页相对地址 | ADRP X0,label | 4KB页相对地址 |
| ADD Xd,Xn,Xm,LSL #shift | 地址计算 | ADD X0,X1,X2,LSL #3 | X0 = X1 + X2 * 8 |

### 条件选择/数据处理指令
| 指令格式 | 功能说明 | 代码示例 | 备注 |
| :------: | -------- | -------- | ---- |
| CSEL Xd,Xn,Xm,cond | 条件选择 | CSEL X0,X1,X2,EQ | EQ时选X1，否则X2 |
| CSINC Xd,Xn,Xm,cond | 条件选择并递增 | CSINC X0,X1,X2,EQ | EQ时X1，否则X2+1 |
| CSINV Xd,Xn,Xm,cond | 条件选择并取反 | CSINV X0,X1,X2,EQ | EQ时X1，否则~X2 |
| CSNEG Xd,Xn,Xm,cond | 条件选择并取负 | CSNEG X0,X1,X2,EQ | EQ时X1，否则-X2 |
| CSET Xd,cond | 条件设置 | CSET X0,EQ | EQ时X0=1，否则0 |
| CSETM Xd,cond | 条件设置掩码 | CSETM X0,EQ | EQ时X0=-1，否则0 |
| CINC Xd,Xn,cond | 条件递增 | CINC X0,X1,EQ | EQ时X0=X1+1，否则X1 |
| CINV Xd,Xn,cond | 条件取反 | CINV X0,X1,EQ | EQ时X0=~X1，否则X1 |
| CNEG Xd,Xn,cond | 条件取负 | CNEG X0,X1,EQ | EQ时X0=-X1，否则X1 |

### 系统指令/特殊寄存器指令
| 指令格式 | 功能说明 | 代码示例 | 备注 |
| :------: | -------- | -------- | ---- |
| MRS Xd,reg | 读取系统寄存器 | MRS X0,CNTFRQ_EL0 | 读取计数器频率 |
| MSR reg,Xn | 写入系统寄存器 | MSR DAIFSET,#0xF | 设置中断屏蔽 |
| NOP | 空操作 | NOP | 不做任何操作 |
| DMB option | 数据内存屏障 | DMB ISH | 数据内存屏障 |
| DSB option | 数据同步屏障 | DSB ISH | 数据同步屏障 |
| ISB | 指令同步屏障 | ISB | 刷新流水线 |
| CLREX | 清除独占标记 | CLREX | 清除LDXR/STXR的独占标记 |
| SEV | 发送事件 | SEV | 向所有CPU发送事件 |
| SEVL | 发送本地事件 | SEVL | 向本地CPU发送事件 |
| WFE | 等待事件 | WFE | 进入低功耗等待事件 |
| WFI | 等待中断 | WFI | 进入低功耗等待中断 |
| YIELD | 让出处理器 | YIELD | 提示可以切换线程 |
| MRS Xd,TPIDR_EL0 | 读取线程ID寄存器 | MRS X0,TPIDR_EL0 | 读取线程本地存储指针 |
| MSR TPIDR_EL0,Xn | 写入线程ID寄存器 | MSR TPIDR_EL0,X0 | 写入线程本地存储指针 |

### 条件码说明
| 助记符 | 含义 | 标志位条件 |
| :----: | ---- | ---------- |
| EQ | 相等/为零 | Z == 1 |
| NE | 不相等/非零 | Z == 0 |
| CS/HS | 进位/无符号大于等于 | C == 1 |
| CC/LO | 无进位/无符号小于 | C == 0 |
| MI | 负数 | N == 1 |
| PL | 正数或零 | N == 0 |
| VS | 溢出 | V == 1 |
| VC | 未溢出 | V == 0 |
| HI | 无符号大于 | C == 1 && Z == 0 |
| LS | 无符号小于等于 | C == 0 || Z == 1 |
| GE | 有符号大于等于 | N == V |
| LT | 有符号小于 | N != V |
| GT | 有符号大于 | Z == 0 && N == V |
| LE | 有符号小于等于 | Z == 1 || N != V |
| AL | 总是 | 任意 |
| NV | 从不（保留） | 无 |

### 内存屏障选项
| 选项 | 说明 |
| :--: | ---- |
| OSHLD | Outer Shareable, load |
| OSHST | Outer Shareable, store |
| OSH | Outer Shareable, any |
| NSHLD | Non-Shareable, load |
| NSHST | Non-Shareable, store |
| NSH | Non-Shareable, any |
| ISHLD | Inner Shareable, load |
| ISHST | Inner Shareable, store |
| ISH | Inner Shareable, any |
| LD | Any, load |
| ST | Any, store |
| SY | Any, any (full barrier) |
