# 写个最简单的测试文件
echo 'int main() { return 0; } > /tmp/test.c

# 试试加 -m32 参数编32位
x86_64-elf-gcc -m32 -c /tmp/test.c -o /tmp/test.o 2>&1

# 如果成功了，看看是不是32位
x86_64-elf-objdump -f /tmp/test.o