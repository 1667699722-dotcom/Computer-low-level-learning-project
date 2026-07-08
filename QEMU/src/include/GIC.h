// ==================== 基本类型定义 ====================
typedef unsigned long      uint64_t;   // 64位无符号整数
typedef unsigned int       uint32_t;   // 32位无符号整数
typedef unsigned short     uint16_t;   // 16位无符号整数
typedef unsigned char      uint8_t;    // 8位无符号整数
// GICD 寄存器
#define GICD_CTLR       0x000   // Distributor Control Register
#define GICD_ISENABLER  0x100   // Interrupt Set-Enable Registers
#define GICD_ICENABLER  0x180   // Interrupt Clear-Enable Registers
#define GICD_IPRIORITYR 0x400   // Interrupt Priority Registers
#define GICD_ITARGETSR  0x800   // Interrupt Targets Registers
#define GICD_ICFGR      0xC00   // Interrupt Configuration Registers

// GICC 寄存器
#define GICC_CTLR       0x000   // CPU Interface Control Register
#define GICC_PMR        0x004   // Interrupt Priority Mask Register
#define GICC_IAR        0x00C   // Interrupt Acknowledge Register
#define GICC_EOIR       0x010   // End of Interrupt Register

void gic_init(void);