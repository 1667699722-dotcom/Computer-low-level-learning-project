.section .text.exceptions
.balign 2048

.global exception_vector_table
exception_vector_table:

    // Current EL with SP_EL0: Synchronous
    b exception_handler
    .space 124
    // Current EL with SP_EL0: IRQ
    b exception_handler
    .space 124
    // Current EL with SP_EL0: FIQ
    b exception_handler
    .space 124
    // Current EL with SP_EL0: SError
    b exception_handler
    .space 124

    // Current EL with SP_ELx: Synchronous
    b exception_handler
    .space 124
    // Current EL with SP_ELx: IRQ
    b exception_handler
    .space 124
    // Current EL with SP_ELx: FIQ
    b exception_handler
    .space 124
    // Current EL with SP_ELx: SError
    b exception_handler
    .space 124

    // Lower EL using AArch64: Synchronous
    b exception_handler
    .space 124
    // Lower EL using AArch64: IRQ
    b exception_handler
    .space 124
    // Lower EL using AArch64: FIQ
    b exception_handler
    .space 124
    // Lower EL using AArch64: SError
    b exception_handler
    .space 124

    // Lower EL using AArch32: Synchronous
    b exception_handler
    .space 124
    // Lower EL using AArch32: IRQ
    b exception_handler
    .space 124
    // Lower EL using AArch32: FIQ
    b exception_handler
    .space 124
    // Lower EL using AArch32: SError
    b exception_handler
    .space 124

exception_handler:
    // 保存所有通用寄存器
    stp x0, x1, [sp, #-16]!
    stp x2, x3, [sp, #-16]!
    stp x4, x5, [sp, #-16]!
    stp x6, x7, [sp, #-16]!
    stp x8, x9, [sp, #-16]!
    stp x10, x11, [sp, #-16]!
    stp x12, x13, [sp, #-16]!
    stp x14, x15, [sp, #-16]!
    stp x16, x17, [sp, #-16]!
    stp x18, x19, [sp, #-16]!
    stp x20, x21, [sp, #-16]!
    stp x22, x23, [sp, #-16]!
    stp x24, x25, [sp, #-16]!
    stp x26, x27, [sp, #-16]!
    stp x28, x29, [sp, #-16]!
    stp x30, xzr, [sp, #-16]!

    // 保存栈帧指针，确保栈16字节对齐（AArch64调用约定要求）
    sub sp, sp, #16

    // 读取异常寄存器并传递给C处理函数
    mrs x0, esr_el2
    mrs x1, elr_el2
    mrs x2, far_el2
    bl c_exception_handler

    // 恢复栈
    add sp, sp, #16

    // 如果返回 != 0，是同步异常 - 跳过触发指令
    cmp x0, #0
    b.eq exception_return
    mrs x1, elr_el2
    add x1, x1, #4
    msr elr_el2, x1

exception_return:
    // 恢复寄存器
    ldp x30, xzr, [sp], #16
    ldp x28, x29, [sp], #16
    ldp x26, x27, [sp], #16
    ldp x24, x25, [sp], #16
    ldp x22, x23, [sp], #16
    ldp x20, x21, [sp], #16
    ldp x18, x19, [sp], #16
    ldp x16, x17, [sp], #16
    ldp x14, x15, [sp], #16
    ldp x12, x13, [sp], #16
    ldp x10, x11, [sp], #16
    ldp x8, x9, [sp], #16
    ldp x6, x7, [sp], #16
    ldp x4, x5, [sp], #16
    ldp x2, x3, [sp], #16
    ldp x0, x1, [sp], #16

    eret
    