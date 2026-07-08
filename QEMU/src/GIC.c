#include "include/GIC.h"

void gic_init(void) {
    // 1. 初始化 GICD (Distributor)
    uint32_t *gicd = (uint32_t *)0x08000000;
    
    // 关闭 GICD
    gicd[GICD_CTLR / 4] = 0;
    
    // 使能所有中断的优先级（可选）
    for (int i = 0; i < 256; i++) {
        ((uint8_t *)gicd)[GICD_IPRIORITYR + i] = 0xA0;
    }
    
    // 路由所有中断到 CPU0
    for (int i = 0; i < 256; i++) {
        ((uint8_t *)gicd)[GICD_ITARGETSR + i] = 0x01;
    }
    
    // 配置为电平触发（大部分外设）
    for (int i = 0; i < 64; i++) {
        gicd[GICD_ICFGR / 4 + i] = 0;
    }
    
    // 使能 GICD (Group0 + Group1)
    gicd[GICD_CTLR / 4] = 0x03;
    
    // 2. 初始化 GICC (CPU Interface)
    uint32_t *gicc1 = (uint32_t *)0x08010000;
    
    // 设置优先级掩码（0xFF 表示所有优先级都接受）
    gicc1[GICC_PMR / 4] = 0xFF;
    
    // 使能 GICC
    gicc1[GICC_CTLR / 4] = 0x03;

    // 2. 初始化 GICC (CPU Interface)
    uint32_t *gicc2 = (uint32_t *)0x08020000;
    
    // 设置优先级掩码（0xFF 表示所有优先级都接受）
    gicc2[GICC_PMR / 4] = 0xFF;
    
    // 使能 GICC
    gicc2[GICC_CTLR / 4] = 0x03;
}