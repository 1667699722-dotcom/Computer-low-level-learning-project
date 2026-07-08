set -e
mkdir -p bin

# 创建磁盘镜像（如果不存在）
if [ ! -f disk.img ]; then
    dd if=/dev/zero of=disk.img bs=1M count=1
fi

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
# 编译 src_1/ 下所有 C 文件
for file in src_1/*.c; do
    filename=$(basename "$file" .c)
    /opt/homebrew/opt/llvm/bin/clang --target=aarch64-none-elf -ffreestanding -fno-builtin -Wall -Wextra -g -Isrc/include -c "$file" -o "bin/$filename.o"
done
# 编译 kernel.c
/opt/homebrew/opt/llvm/bin/clang --target=aarch64-none-elf -ffreestanding -fno-builtin -Wall -Wextra -g -Isrc/include -c kernel.c -o bin/kernel.o

# 链接所有目标文件
/opt/homebrew/opt/lld/bin/ld.lld -T linker.ld -nostdlib -o bin/kernel.elf bin/*.o

# 运行：-kernel 方式，virtualization=on 让内核在 EL2 启动
qemu-system-aarch64 -machine virt,virtualization=on,gic-version=2 -cpu cortex-a72  -smp 4 -m 512M -nographic -semihosting -kernel bin/kernel.elf -drive file=disk.img,if=none,format=raw,id=hd -device virtio-blk-device,drive=hd -global virtio-mmio.force-legacy=false

#qemu-system-aarch64 -machine virt,virtualization=on -cpu cortex-a72 -m 512M -serial mon:stdio -display cocoa -kernel bin/kernel.elf -drive file=disk.img,if=none,format=raw,id=hd -device virtio-blk-device,drive=hd -device virtio-gpu-device -global virtio-mmio.force-legacy=false

#qemu-system-aarch64 -accel hvf -machine virt -cpu host -m 512M -serial mon:stdio -display cocoa -kernel bin/kernel.elf -drive file=disk.img,if=none,format=raw,id=hd -device virtio-blk-device,drive=hd -device virtio-gpu-device -global virtio-mmio.force-legacy=false