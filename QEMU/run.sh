set -e
mkdir -p bin
/opt/homebrew/opt/llvm/bin/clang --target=aarch64-none-elf -Wall -Wextra -g -c start.s -o bin/start.o
/opt/homebrew/opt/llvm/bin/clang --target=aarch64-none-elf -ffreestanding -fno-builtin -Wall -Wextra -g -c kernel.c -o bin/kernel.o
/opt/homebrew/opt/lld/bin/ld.lld -T linker.ld -nostdlib -o bin/kernel.elf bin/start.o bin/kernel.o
qemu-system-aarch64 -machine virt -cpu cortex-a72 -m 64M -nographic -semihosting -kernel bin/kernel.elf
