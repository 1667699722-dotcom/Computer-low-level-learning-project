#include "include/time.h"
#include "include/fs.h"
#include "include/uart.h"

//使用CNTHP_TVAL_EL2倒计时

// 1. 读取并打印频率
static inline uint64_t read_cntfrq(void)
{
    uint64_t val;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(val));
    return val;
}
// 2. 循环读取计数值，观察变化
static inline uint64_t read_cntpct(void)
{
    uint64_t val;
    asm volatile("mrs %0, cntpct_el0" : "=r"(val));
    return val;
}

void test_timer() {
    // 1. 读取并打印频率
    uint64_t freq = read_cntfrq();
    uart_put_dec(freq);
    // 2. 循环读取计数值，观察变化
    for (int i = 0; i < 10; i++) {
        uint64_t cnt = read_cntpct();
        uart_put_dec(cnt);
        uart_puts("\n");
        // 打印 cnt
        // 延时一会儿
    }
}

static inline void write_cnthp_tval(uint64_t val)
{
    asm volatile("msr cnthp_tval_el2, %0" : : "r"(val));
}

// 设置 HP 定时器控制寄存器（使能、中断使能等）
static inline void write_cnthp_ctl(uint64_t val)
{
    asm volatile("msr cnthp_ctl_el2, %0" : : "r"(val));
}
// 使能、开中断
void cnthp_start_irq(void)
{
    write_cnthp_ctl(1);
}
// 使能、屏蔽中断
 void cnthp_start_noirq(void)
{
    write_cnthp_ctl((1U << 2) | 1);
}
// 关闭定时器
 void cnthp_stop(void)
{
    write_cnthp_ctl(0);
}

void cnthp_set_period(uint64_t ticks)
{
    write_cnthp_tval(ticks);
}

static inline uint64_t read_cnthp_tval(void)
{
    uint64_t val;
    asm volatile("mrs %0, cnthp_tval_el2" : "=r"(val));
    return val;
}
//读取 CNTHP_CTL_EL2 寄存器
static inline uint64_t read_cnthp_ctl(void)
{
    uint64_t val;
    asm volatile("mrs %0, cnthp_ctl_el2" : "=r"(val));
    return val;
}
void readCNTHP_CTL_EL2()
{
    uint64_t cnt1 = read_cnthp_ctl();
    uart_puts("CNTHP_CTL_EL2:");
    uart_put_dec(cnt1);
    uart_puts("\n");
}

void testcnthp() {
    
    // 2. 循环读取计数值，观察变化
    for (int i = 0; i < 10; i++) {
        uint64_t cnt = read_cnthp_tval();
        uart_put_dec(cnt);
        uart_puts("\n");
        uint64_t cnt1 = read_cnthp_ctl();
        uart_put_dec(cnt1);
        uart_puts("\n");
        // 打印 cnt
        // 延时一会儿
    }
}