set -e
mkdir -p bin

# 编译汇编
/opt/homebrew/opt/llvm/bin/clang --target=aarch64-none-elf -Wall -Wextra -g -c start.s -o bin/start.o

# 编译 src/ASM 下所有汇编文件
for file in src/ASM/*.s; do
    filename=$(basename "$file" .s)
    /opt/homebrew/opt/llvm/bin/clang --target=aarch64-none-elf -Wall -Wextra -g -c "$file" -o "bin/$filename.o"
done

# 编译 src/ 下所有 C 文件
for file in src/*.c; do
    filename=$(basename "$file" .c)
    /opt/homebrew/opt/llvm/bin/clang --target=aarch64-none-elf -ffreestanding -fno-builtin -Wall -Wextra -g -Isrc/include -c "$file" -o "bin/$filename.o"
done

for file in src_1/*.c; do
    filename=$(basename "$file" .c)
    /opt/homebrew/opt/llvm/bin/clang --target=aarch64-none-elf -ffreestanding -fno-builtin -Wall -Wextra -g -Isrc/include -c "$file" -o "bin/$filename.o"
done
# 编译 kernel.c
/opt/homebrew/opt/llvm/bin/clang --target=aarch64-none-elf -ffreestanding -fno-builtin -Wall -Wextra -g -Isrc/include -c kernel.c -o bin/kernel.o

# 链接所有目标文件
/opt/homebrew/opt/lld/bin/ld.lld -T linker.ld -nostdlib -o bin/kernel.elf bin/*.o

# 运行：-kernel 方式，virtualization=on 让内核在 EL2 启动
qemu-system-aarch64 -machine virt,virtualization=on -cpu cortex-a72 -m 64M -nographic -semihosting -kernel bin/kernel.elf
