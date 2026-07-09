// ==================== 基本类型定义 ====================
typedef unsigned long      uint64_t;   // 64位无符号整数
typedef unsigned int       uint32_t;   // 32位无符号整数
typedef unsigned short     uint16_t;   // 16位无符号整数
typedef unsigned char      uint8_t;    // 8位无符号整数

void test_timer();


void cnthp_set_period(uint64_t ticks);
// 使能、开中断
void cnthp_start_irq(void);
// 使能、屏蔽中断
 void cnthp_start_noirq(void);
 // 关闭定时器
 void cnthp_stop(void);
 void testcnthp(void);
void readCNTHP_CTL_EL2();
