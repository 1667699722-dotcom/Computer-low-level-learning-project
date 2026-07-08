#include "include/uart.h"
#include "include/exceptionhandler.h"
#include "include/lib.h"
#include "include/GIC.h"
#include "include/time.h"
// ESR_EL2 的 EC 字段（bit[31:26]）表示异常类别
#define ESR_EC_SHIFT        26
#define ESR_EC_MASK         (0x3F << ESR_EC_SHIFT)
#define EC_UNKNOWN          0x00
#define EC_WFI_WFE          0x01
#define EC_MCR_MRC_CP15     0x03
#define EC_MCRR_MRRC_CP15   0x04
#define EC_MCR_MRC_CP14     0x0C
#define EC_LDC_STC_CP14     0x0D
#define EC_SIMD_FP          0x07
#define EC_VMRS             0x0A
#define EC_PAUTH            0x09
#define EC_LD64B_ST64B     0x13
#define EC_ILLEGAL_EXEC     0x0E  // 执行状态非法
#define EC_SVC_64           0x15  // SVC 指令（AArch64）
#define EC_HVC_64           0x16  // HVC 指令（AArch64）
#define EC_SMC_64           0x17  // SMC 指令（AArch64）
#define EC_SVC_32           0x11  // SVC 指令（AArch32）
#define EC_HVC_32           0x12  // HVC 指令（AArch32）
#define EC_SMC_32           0x13  // SMC 指令（AArch32）
#define EC_SYSTEM_REG       0x18  // MSR/MRS 系统寄存器访问
#define EC_IABT_LOW         0x20  // 指令中止（来自低EL）
#define EC_IABT_CUR         0x21  // 指令中止（当前EL）
#define EC_PC_ALIGN         0x22  // PC 对齐错误
#define EC_DABT_LOW         0x24  // 数据中止（来自低EL）
#define EC_DABT_CUR         0x25  // 数据中止（当前EL）
#define EC_SP_ALIGN         0x26  // SP 对齐错误
#define EC_FLOAT_EXC        0x28  // SIMD/FP 异常
#define EC_SERROR           0x2F  // SError
#define EC_BRK              0x3C  // 软件断点（BRK指令）
#define EC_STEP             0x33  // 单步
#define EC_WATCHPOINT_LOW   0x34  // 观察点（来自低EL）
#define EC_WATCHPOINT_CUR   0x35  // 观察点（当前EL）
#define EC_BKPT_32          0x38  // 软件断点（AArch32 BKPT）
#define EC_VECTOR_CATCH     0x0B  // Vector catch

static const char *get_ec_name(unsigned int ec) {
    switch (ec) {
        case EC_UNKNOWN:        return "Unknown reason";
        case EC_WFI_WFE:        return "WFI/WFE";
        case EC_MCR_MRC_CP15:   return "MCR/MRC CP15";
        case EC_MCRR_MRRC_CP15: return "MCRR/MRRC CP15";
        case EC_MCR_MRC_CP14:   return "MCR/MRC CP14";
        case EC_LDC_STC_CP14:   return "LDC/STC CP14";
        case EC_SIMD_FP:        return "SIMD/FP";
        case EC_VMRS:           return "VMRS";
        case EC_PAUTH:          return "Pointer Authentication";
        case EC_ILLEGAL_EXEC:   return "Illegal Execution State";
        case EC_SVC_64:         return "SVC (AArch64)";
        case EC_HVC_64:         return "HVC (AArch64)";
        case EC_SMC_64:         return "SMC (AArch64)";
        case EC_SVC_32:         return "SVC (AArch32)";
        case EC_HVC_32:         return "HVC (AArch32)";
        case EC_SMC_32:         return "SMC (AArch32)";
        case EC_SYSTEM_REG:     return "System Register Access";
        case EC_IABT_LOW:       return "Instruction Abort (Lower EL)";
        case EC_IABT_CUR:       return "Instruction Abort (Current EL)";
        case EC_PC_ALIGN:       return "PC Alignment Fault";
        case EC_DABT_LOW:       return "Data Abort (Lower EL)";
        case EC_DABT_CUR:       return "Data Abort (Current EL)";
        case EC_SP_ALIGN:       return "SP Alignment Fault";
        case EC_FLOAT_EXC:      return "SIMD/FP Exception";
        case EC_SERROR:         return "SError (System Error)";
        case EC_BRK:            return "Breakpoint (BRK)";
        case EC_STEP:           return "Software Step";
        case EC_WATCHPOINT_LOW: return "Watchpoint (Lower EL)";
        case EC_WATCHPOINT_CUR: return "Watchpoint (Current EL)";
        case EC_BKPT_32:        return "Breakpoint (AArch32 BKPT)";
        default:                return "Unknown EC";
    }
}


int c_exception_handler(unsigned long esr, unsigned long elr, unsigned long far) {

    unsigned int ec = (unsigned int)((esr & ESR_EC_MASK) >> ESR_EC_SHIFT);
    
    uart_puts("\n=== Exception ===\n");
    uart_puts("ESR_EL2: 0x");
    uart_put_hex((unsigned int)esr);
    uart_puts("\n");
    uart_puts("ELR_EL2: 0x");
    uart_put_hex((unsigned int)elr);
    uart_puts("\n");
    uart_puts("FAR_EL2: 0x");
    uart_put_hex((unsigned int)far);
    uart_puts("\n");
    uart_puts("EC (Exception Class): ");
    uart_puts(get_ec_name(ec));
    uart_puts(" (0x");
    uart_put_hex(ec);
    uart_puts(")\n");
    uart_puts("=================\n\n");



    
    // 同步异常需要跳过触发指令（返回1表示需要跳过）
    // 包括：未定义指令、数据中止、指令中止、SVC、HVC、SMC、系统寄存器访问等
    // IRQ/FIQ/SError 等异步异常不需要跳过
    // 这里简化处理：大部分同步异常都需要跳过
    switch (ec) {
        case EC_UNKNOWN:
        case EC_MCR_MRC_CP15:
        case EC_MCRR_MRRC_CP15:
        case EC_MCR_MRC_CP14:
        case EC_LDC_STC_CP14:
        case EC_SIMD_FP:
        case EC_VMRS:
        case EC_PAUTH:
        case EC_ILLEGAL_EXEC:
        case EC_SVC_64:
        case EC_HVC_64:
        case EC_SMC_64:
        case EC_SVC_32:
        case EC_HVC_32:
        case EC_SMC_32:
        case EC_SYSTEM_REG:
        case EC_IABT_LOW:
        case EC_IABT_CUR:
        case EC_DABT_LOW:
        case EC_DABT_CUR:
        case EC_FLOAT_EXC:
        case EC_BRK:
        case EC_BKPT_32:
            return 1; // 需要跳过指令
        default:
            return 0; // 不需要跳过
    }
}