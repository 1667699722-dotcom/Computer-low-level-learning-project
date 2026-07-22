#!/bin/bash
#
# Linux 0.12 风格 QEMU 启动脚本
# 模拟 80386/486 时代的 PC 硬件
#

set -e

# ============ 目录与文件路径 ============
SRC_DIR="src"
BUILD_DIR="build"
FLOPPY_IMG="$BUILD_DIR/boot.img"     # 软盘镜像 (1.44MB)
HDD_IMG="$BUILD_DIR/hd.img"            # 硬盘镜像
BOOT_BIN="$BUILD_DIR/boot.bin"         # 编译后的 bootloader
BOOT_SRC="$SRC_DIR/boot.s"             # bootloader 源码

# ============ 工具链配置 ============
# macOS 上可以用 brew install nasm 安装 NASM
NASM=${NASM:-nasm}

# ============ 创建目录 ============
mkdir -p $SRC_DIR $BUILD_DIR

# ============ 创建软盘镜像 (1.44MB) ============
# Linux 0.12 使用 3.5" 1.44MB 软盘启动
if [ ! -f "$FLOPPY_IMG" ]; then
    echo "[+] 创建软盘镜像: $FLOPPY_IMG (1.44MB)"
    dd if=/dev/zero of="$FLOPPY_IMG" bs=512 count=2880 2>/dev/null
fi

# ============ 创建硬盘镜像 (可选) ============
# Linux 0.12 支持 IDE 硬盘，这里创建一个 10MB 的小硬盘
if [ ! -f "$HDD_IMG" ]; then
    echo "[+] 创建硬盘镜像: $HDD_IMG (10MB)"
    dd if=/dev/zero of="$HDD_IMG" bs=1M count=10 2>/dev/null
fi

# ============ 编译 Bootloader ============
if [ -f "$BOOT_SRC" ]; then
    echo "[+] 编译 bootloader: $BOOT_SRC -> $BOOT_BIN"
    $NASM -f bin "$BOOT_SRC" -o "$BOOT_BIN"

    # 检查是否正好 512 字节
    BOOT_SIZE=$(wc -c < "$BOOT_BIN")
    if [ "$BOOT_SIZE" -ne 512 ]; then
        echo "[!] 警告: bootloader 大小为 $BOOT_SIZE 字节，不是 512 字节"
    fi

    # 写入软盘第 1 扇区
    echo "[+] 写入 bootloader 到软盘第 1 扇区"
    dd if="$BOOT_BIN" of="$FLOPPY_IMG" bs=512 count=1 conv=notrunc 2>/dev/null
fi

# ============ QEMU 启动 ============
# 使用 qemu-system-i386 模拟 32 位 x86
#
# 硬件配置对应 Linux 0.12 时代的典型配置：
#   - CPU: 80486 (32-bit x86)
#   - 内存: 16MB (Linux 0.12 支持最多 64MB)
#   - 软盘控制器: 8272A (标准 PC 软盘控制器)
#   - 硬盘控制器: IDE/ATA (PC/AT)
#   - 中断控制器: 8259A PIC
#   - 定时器: 8253 PIT
#   - 串口: 16550 UART (COM1)
#   - 键盘: 8042 键盘控制器 + PS/2 键盘
#   - 显示: VGA
#   - RTC: MC146818

echo "[*] 启动 QEMU..."

qemu-system-i386 \
    -M pc \
    -cpu 486 \
    -m 16M \
    -drive file="$FLOPPY_IMG",format=raw,if=floppy \
    -drive file="$HDD_IMG",format=raw,if=ide \
    -boot a \
    -serial stdio \
    -vga std \
    -no-reboot \
    -d guest_errors,unimp

# 退出时提示
echo "[*] QEMU 已退出"

# ============ 参数说明 ============
# -M pc           : 标准 PC 机器类型（包含所有传统 PC 设备）
# -cpu 486        : 模拟 Intel 80486 CPU
# -m 16M          : 16MB 内存（Linux 0.12 时代典型配置）
# -fda            : 第一个软盘驱动器 A
# -hda            : 第一个 IDE 硬盘
# -boot a         : 从软盘启动
# -serial stdio   : 串口重定向到标准输入输出（方便调试）
# -vga std        : 标准 VGA 显卡
# -no-reboot      : 内核崩溃时不自动重启
# -d guest_errors,unimp : 显示客户机错误和未实现的操作
#
# ============ 调试模式（取消注释启用）============
# 如果你需要用 GDB 调试内核，使用下面的命令：
#
# qemu-system-i386 \
#     -M pc \
#     -cpu 486 \
#     -m 16M \
#     -fda "$FLOPPY_IMG" \
#     -hda "$HDD_IMG" \
#     -boot a \
#     -serial stdio \
#     -vga std \
#     -s -S \
#     -no-reboot
#
# 然后在另一个终端运行：
#   gdb -ex "target remote :1234"
#
# -s  : QEMU 监听 1234 端口等待 GDB 连接
# -S  : 启动后暂停，等待 GDB 连接

# ============ 切换到图形界面（取消注释启用）============
# 如果你想用图形界面而不是串口：
#
# qemu-system-i386 \
#     -M pc \
#     -cpu 486 \
#     -m 16M \
#     -fda "$FLOPPY_IMG" \
#     -hda "$HDD_IMG" \
#     -boot a \
#     -vga std \
#     -display cocoa \
#     -no-reboot