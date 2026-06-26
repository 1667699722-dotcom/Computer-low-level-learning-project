// 参数: esr - 异常状态寄存器, elr - 异常返回地址, far - 故障地址寄存器
// 返回值: 非0表示需要跳过触发异常的指令（同步异常）
int c_exception_handler(unsigned long esr, unsigned long elr, unsigned long far);