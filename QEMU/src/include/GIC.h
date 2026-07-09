// ==================== 基本类型定义 ====================
typedef unsigned long      uint64_t;   // 64位无符号整数
typedef unsigned int       uint32_t;   // 32位无符号整数
typedef unsigned short     uint16_t;   // 16位无符号整数
typedef unsigned char      uint8_t;    // 8位无符号整数

// GIC Distributor 寄存器
#define GICD_BASE            0x8000000
#define GICD_CTLR           (GICD_BASE + 0x000)
#define GICD_ISENABLER(n)   (GICD_BASE + 0x100 + (n) * 4)
#define GICD_IPRIORITYR(n)  (GICD_BASE + 0x400 + (n) * 4)

// GIC CPU Interface 寄存器
#define GICC_BASE            0x8010000
#define GICC_CTLR           (GICC_BASE + 0x000)
#define GICC_PMR            (GICC_BASE + 0x004)
#define GICC_IAR            (GICC_BASE + 0x00C)
#define GICC_EOIR           (GICC_BASE + 0x010)


