#!/bin/zsh
#cat << 'PASS'

#╔══════════════════════════════════════════════════════╗
#║                                                      ║
#║   ██████      ████     ██████   ██████               ║
#║   ██    ██  ██    ██  ██       ██                    ║
#║   ██ ██ ██  █████████  ██████   ██████               ║
#║   ██        ██    ██        ██       ██              ║
#║   ██        ██    ██  ██████   ██████                ║
#║                                                      ║
#║   ────────────────────────────────────────────────   ║
#║                                                      ║
#║   BUILD SUCCESSFUL - launching lldb...               ║
#║                                                      ║
#╚══════════════════════════════════════════════════════╝

#PASS    
set -e
mkdir -p bin
/opt/homebrew/opt/llvm/bin/clang   --target=aarch64-none-elf  -c start.s -o bin/start.o
/opt/homebrew/opt/lld/bin/ld.lld  -Ttext=0x40000000  -e _start  -o bin/kernel.elf  bin/start.o
qemu-system-aarch64   -machine virt   -cpu cortex-a72  -nographic  -kernel bin/kernel.elf
