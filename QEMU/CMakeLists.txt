cmake_minimum_required(VERSION 3.18)

# 禁用 macOS 特有的自动参数
set(CMAKE_OSX_SYSROOT "")
set(CMAKE_OSX_ARCHITECTURES "")
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# 强制用 ld.lld 链接
set(CMAKE_C_LINK_EXECUTABLE "/opt/homebrew/opt/lld/bin/ld.lld <OBJECTS> -o <TARGET> <LINK_FLAGS>")
set(CMAKE_ASM_LINK_EXECUTABLE "/opt/homebrew/opt/lld/bin/ld.lld <OBJECTS> -o <TARGET> <LINK_FLAGS>")

project(arm64_kernel LANGUAGES C ASM)

# ========== 工具链配置 ==========
set(TARGET_TRIPLE aarch64-none-elf)
set(CLANG /opt/homebrew/opt/llvm/bin/clang)
set(LLD /opt/homebrew/opt/lld/bin/ld.lld)

set(CMAKE_C_COMPILER ${CLANG})
set(CMAKE_ASM_COMPILER ${CLANG})
set(CMAKE_LINKER ${LLD})

# ========== 源文件列表 ==========
set(SOURCES
    start.s
    src/ASM/exceptions.s
    src/ASM/memory.s
    src/uart.c
    src/exceptionhandler.c
    src/lib.c
    src/exit.c
    kernel.c
)

# ========== 生成内核 elf ==========
add_executable(kernel.elf ${SOURCES})

# C文件编译参数（对应 run.sh）
target_compile_options(kernel.elf PRIVATE
    $<$<COMPILE_LANGUAGE:C>:
        --target=${TARGET_TRIPLE}
        -ffreestanding
        -fno-builtin
        -Wall -Wextra -g
        -Isrc/include
    >
    $<$<COMPILE_LANGUAGE:ASM>:
        --target=${TARGET_TRIPLE}
        -Wall -Wextra -g
    >
)

# 链接参数（对应 run.sh）
target_link_options(kernel.elf PRIVATE
    -T ${CMAKE_SOURCE_DIR}/linker.ld
    -nostdlib
)

# 输出目录
set_target_properties(kernel.elf PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

# ========== 自定义目标 ==========
# 运行 QEMU
add_custom_target(run
    COMMAND qemu-system-aarch64
        -machine virt,virtualization=on
        -cpu cortex-a72
        -m 64M
        -nographic
        -semihosting
        -kernel bin/kernel.elf
    DEPENDS kernel.elf
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
