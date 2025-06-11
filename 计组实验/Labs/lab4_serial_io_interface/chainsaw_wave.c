#include "xintc_l.h"
#include "xtmrctr_l.h"
#include "xgpio_l.h"
#include "xil_io.h"
#include "xil_exception.h"
#include "xspi_l.h"

// 周期范围（微秒）
#define MIN_PERIOD (6000000 - 2)   // 60ms
#define MAX_PERIOD (100000000 - 2) // 1s

void interrupt_hub(void)__attribute__((interrupt_handler));
void swHandler(void);
void tmrHandler(void);

int tcsr;
volatile u16 volt = 0;

int main()
{
	// GPIO配置
	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI_OFFSET, 0xffff);
	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_ISR_OFFSET, XGPIO_IR_CH1_MASK);
	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_IER_OFFSET, XGPIO_IR_CH1_MASK);
	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_GIE_OFFSET, XGPIO_GIE_GINTR_ENABLE_MASK);

	// SPI 配置
	Xil_Out32(XPAR_AXI_QUAD_SPI_0_BASEADDR + XSP_CR_OFFSET,
			  XSP_CR_ENABLE_MASK|XSP_CR_MASTER_MODE_MASK|XSP_CR_CLK_POLARITY_MASK);
	Xil_Out32(XPAR_AXI_QUAD_SPI_0_BASEADDR + XSP_SSR_OFFSET, 0xfffffffe);

	// 中断控制器配置
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IER_OFFSET,
			XPAR_AXI_GPIO_0_IP2INTC_IRPT_MASK | XPAR_AXI_TIMER_0_INTERRUPT_MASK);
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_MER_OFFSET,
			  XIN_INT_MASTER_ENABLE_MASK|XIN_INT_HARDWARE_ENABLE_MASK);

	// 定时器配置
	Xil_Out32(XPAR_TMRCTR_0_BASEADDR + XTC_TCSR_OFFSET, XTC_CSR_INT_OCCURED_MASK); // 清除中断状态
	u32 initial_load = MAX_PERIOD;
	Xil_Out32(XPAR_TMRCTR_0_BASEADDR + XTC_TLR_OFFSET, initial_load); // 设置预置值
	Xil_Out32(XPAR_TMRCTR_0_BASEADDR + XTC_TCSR_OFFSET, XTC_CSR_LOAD_MASK);
	Xil_Out32(XPAR_TMRCTR_0_BASEADDR + XTC_TCSR_OFFSET,
			  XTC_CSR_ENABLE_TMR_MASK | XTC_CSR_AUTO_RELOAD_MASK |
			  XTC_CSR_DOWN_COUNT_MASK | XTC_CSR_ENABLE_INT_MASK); // 设置工作模式

	microblaze_enable_interrupts();
	Xil_Out16(XPAR_AXI_QUAD_SPI_0_BASEADDR + XSP_DTR_OFFSET, 0); // 发送数据0
	return 0;
}

void interrupt_hub()
{
	u32 int_status = Xil_In32(XPAR_AXI_INTC_0_BASEADDR + XIN_ISR_OFFSET);

	// 处理开关中断
	if (int_status & 0x1) {
		swHandler();
	}

	// 处理定时器中断
	if (int_status & 0x4) {
		tmrHandler();
	}

	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IAR_OFFSET, 0xffffffff);
}

void swHandler()
{
	// 读取开关值（假设使用低8位）
	u8 switch_num = Xil_In32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET) & 0xff;
	// 计算新周期（微秒）
	u32 period = MIN_PERIOD + (u32)(((u64)(MAX_PERIOD - MIN_PERIOD) * (255 - switch_num)) / 255);
	 // 调试用：输出period
	 xil_printf("period = %u\n", period);
	// 更新定时器周期
	Xil_Out32(XPAR_TMRCTR_0_BASEADDR + XTC_TLR_OFFSET, period); // 设置预置值

	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_ISR_OFFSET, XGPIO_IR_CH1_MASK);
}

void tmrHandler()
{
	// 生成锯齿波
	xil_printf("cc\n"); // 调试
	volt = (volt + 1) & 0xfff;
	Xil_Out16(XPAR_AXI_QUAD_SPI_0_BASEADDR + XSP_DTR_OFFSET, volt);
	Xil_Out32(XPAR_TMRCTR_0_BASEADDR + XTC_TCSR_OFFSET, XTC_CSR_INT_OCCURED_MASK);
}
