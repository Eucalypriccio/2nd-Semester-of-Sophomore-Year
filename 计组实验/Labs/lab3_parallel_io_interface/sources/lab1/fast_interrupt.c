#include "stdio.h"
#include "xil_io.h"
#include "xgpio_l.h"
#include "xintc_l.h"
#include "sleep.h"

void btnHandler(void)__attribute__((fast_interrupt));
void swHandler(void)__attribute__((fast_interrupt));

u8 seg_code[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90}; // 数字0-9对应的段码
u8 digits_index[8] = {0}; // 数码管各位对应的数字在段码表中对应的索引
u32 btn_status = 0x0;
u32 sw_status;

int main()
{
	Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI_OFFSET, 0xffff); // 读取开关状态
	Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI2_OFFSET, 0x0); // 将开关状态输出到LED中
	Xil_Out16(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_TRI_OFFSET, 0x1f); // 读取按钮状态
	Xil_Out16(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_TRI_OFFSET, 0x0); // 输出数码管位选信号
	Xil_Out16(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_TRI2_OFFSET, 0x0); // 输出数码管段选信号

	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_ISR_OFFSET, 0x1); // 清除GPIO_2的通道1（按钮）的中断状态
	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_IER_OFFSET, 0x1); // 使能通道1中断
	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_GIE_OFFSET, 0x80000000); // 使能中断输出

	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_ISR_OFFSET, 0x1); // 清除GPIO_0的通道1（开关）的中断状态
	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_IER_OFFSET, 0x1); // 使能通道1中断
	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_GIE_OFFSET, 0x80000000); // 使能中断输出

	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IAR_OFFSET, 0xffffffff); // 清除所有通道的中断状态
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IMR_OFFSET, 0x1 | 0x2); // 设置intr[0]和intr[1]中断模式为快速中断
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IER_OFFSET, 0x1 | 0x2); // 使能intr[0]和intr[1]中断
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_MER_OFFSET, 0x3); // 使能硬件中断irq输出
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IVAR_OFFSET + 4 * 0, (int)swHandler);
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IVAR_OFFSET + 4 * 1, (int)btnHandler);

	microblaze_enable_interrupts();
	while (1)
	{
		for (int i = 0; i < 8; i++)
		{
			Xil_Out8(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_DATA_OFFSET, ~(0x80 >> i));
			Xil_Out8(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_DATA2_OFFSET, seg_code[digits_index[i]]);
			usleep(1000);
		}

		usleep(500);
	}

	return 0;
}

void btnHandler()
{
	btn_status = Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f;

	switch(btn_status)
	{
		case 0x1: {digits_index[6] = 0; digits_index[7] = 1; break;}
		case 0x2: {digits_index[6] = 0; digits_index[7] = 2; break;}
		case 0x4: {digits_index[6] = 0; digits_index[7] = 4; break;}
		case 0x8: {digits_index[6] = 0; digits_index[7] = 8; break;}
		case 0x10: {digits_index[6] = 1; digits_index[7] = 0; break;}
		default:;
	}

	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_ISR_OFFSET, 0x1);
}

void swHandler()
{
	sw_status = Xil_In16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET) & 0xffff;
	Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, sw_status);

	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_ISR_OFFSET, 0x1);
}