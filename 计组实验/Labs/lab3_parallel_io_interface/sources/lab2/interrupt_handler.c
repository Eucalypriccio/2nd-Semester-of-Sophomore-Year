#include "stdio.h"
#include "xil_io.h"
#include "xgpio_l.h"
#include "xintc_l.h"

void btnHandler(void)__attribute__((interrupt_handler));
void swHandler();

unsigned short switch_num1, switch_num2; // 开关状态对应的二进制数据
unsigned short add_result, multiply_result; // 两数相加与相乘的结果
unsigned short btn_status; // 按钮状态
volatile int read_flag1 = 0;
volatile int read_flag2 = 0;

int main()
{
	Xil_Out8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_TRI_OFFSET, 0x1f); // 读取按钮状态
	Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI_OFFSET, 0xffff); // 读取开关状态
	Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI2_OFFSET, 0x0); // 将开关状态输出到LED中

	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_ISR_OFFSET, 0x1); // 清除GPIO_2的通道1（按钮）的中断状态
	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_IER_OFFSET, 0x1); // 使能通道1中断
	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_GIE_OFFSET, 0x80000000); // 使能中断输出

	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IAR_OFFSET, 0xffffffff); // 清除所有通道的中断状态
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IER_OFFSET, 0x2); // 使能intr[1]中断
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_MER_OFFSET, 0x3); // 使能硬件中断irq输出

	microblaze_enable_interrupts(); // 使能中断控制器
	while(1);
	return 0;
}

void btnHandler(void)
{
	btn_status = Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f;
	if (btn_status == 0x1) // 按下BTNC
	{
		read_flag1 = 1;
		swHandler();
	}
	if (btn_status == 0x8) // 按下BTNR
	{
		read_flag2 = 1;
		swHandler();
	}
	if ((read_flag1 != 0) && (read_flag2 != 0))
	{
		if (btn_status == 0x2) // 按下BTNU
		{
			Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, 0x0);
			add_result = switch_num1 + switch_num2;
			while ((Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f) != 0);
			Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, add_result);
		}
		if (btn_status == 0x10) // 按下BTND
		{
			Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, 0x0);
			multiply_result = switch_num1 * switch_num2;
			while ((Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f) != 0);
			Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, multiply_result);
		}
	}

	// 清除中断状态
	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_ISR_OFFSET, 0x1);
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IAR_OFFSET, 0xffffffff);
}

void swHandler()
{
	Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, 0x0);
	if (btn_status == 0x1)
	{
		switch_num1 = Xil_In16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET) & 0xffff;
		Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, switch_num1);
	}
	if (btn_status == 0x8)
	{
		switch_num2 = Xil_In16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET) & 0xffff;
		Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, switch_num2);
	}
}