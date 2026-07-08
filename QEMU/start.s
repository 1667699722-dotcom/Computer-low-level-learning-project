.section .text.startup
.global _start
.global __heap_start
.global __heap_end
.global cpu1_entry
.global smp_boot_secondary
_start:
    // 读取 CPU ID
    mrs x0, mpidr_el1
    and x0, x0, #0xFF
    cbz x0, cpu0_start

cpu1_entry:
    ldr x0, =__stack_top_cpu1
    mov sp, x0
    bl kernel_main_cpu1
1:  wfe
    b 1b

cpu0_start:
    ldr x0, =__stack_top
    mov sp, x0

    ldr x0, =__bss_start
    ldr x1, =__bss_end
    b clear_bss_loop

clear_bss:
    str xzr, [x0], #8

clear_bss_loop:
    cmp x0, x1
    b.lt clear_bss

    // 启动 CPU1
    bl smp_boot_secondary
    // 跳转到内核主函数
    bl kernel_main

halt:
    wfe
    b halt

smp_boot_secondary:
    // 使用 PSCI CPU_ON 启动 CPU1
    ldr x0, =0xC4000003    // PSCI CPU_ON function ID
    ldr x1, =1             // CPU ID 1
    ldr x2, =cpu1_entry    // 入口地址
    ldr x3, =0             // context ID
    smc #0
    ret