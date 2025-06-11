#include "stdio.h"
#include "xil_io.h"
#include "xgpio_l.h"
#include "xintc_l.h"
#include "sleep.h"

void btnHandler(void)__attribute__((fast_interrupt));

u8 seg_code[17] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90,
			 	 	   0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e, 0xff};
u16 sw_status;
u32 sw_status_de;
u8 btn_status;
int digits_index[8] = {0}; // 数码管各位对应的数字在段码表中对应的索引

int main()
{
	Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI_OFFSET, 0xffff); // 读取开关状态
	Xil_Out16(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_TRI_OFFSET, 0x1f); // 读取按钮状态
	Xil_Out16(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_TRI_OFFSET, 0x0); // 输出数码管位选信号
	Xil_Out16(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_TRI2_OFFSET, 0x0); // 输出数码管段选信号

	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_ISR_OFFSET, 0x1); // 清除GPIO_2的通道1（按钮）的中断状态
	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_IER_OFFSET, 0x1); // 使能通道1中断
	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_GIE_OFFSET, 0x80000000); // 使能中断输出

	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IAR_OFFSET, 0xffffffff); // 清除所有通道的中断状态
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IMR_OFFSET, 0x2); // 设置intr[1]中断模式为快速中断
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IER_OFFSET, 0x2); // 使能intr[1]中断
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_MER_OFFSET, 0x3); // 使能硬件中断irq输出
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IVAR_OFFSET + 4 * 1, (int)btnHandler);

	microblaze_enable_interrupts(); // 使能中断控制器

	while (1)
	{
		for (int i = 0; i < 8; i++)
		{
			Xil_Out8(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_DATA_OFFSET, ~(0x80 >> i));
			Xil_Out8(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_DATA2_OFFSET, seg_code[digits_index[i]]);
			usleep(1000);
		}
	}

	return 0;
}

void btnHandler()
{
	btn_status = Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f;

	if (btn_status == 0x1) // 显示开关状态的低8位在数码管上
	{
		sw_status = Xil_In16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET) & 0xffff;

		for (int i = 0; i < 8; i++)
		{
			digits_index[i] = ((sw_status & (0x80 >> i)) == 0x0)? 0:1; // 确定每一位是0还是1
		}
	}

	if (btn_status == 0x2) // 显示开关状态对应的16进制数到数码管的低4位（高4位不显示）
	{
		sw_status = Xil_In16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET) & 0xffff;

		digits_index[0] = 16;
		digits_index[1] = 16;
		digits_index[2] = 16;
		digits_index[3] = 16;
		digits_index[4] = (sw_status & 0xf000) >> 12;
		digits_index[5] = (sw_status & 0xf00) >> 8;
		digits_index[6] = (sw_status & 0xf0) >> 4;
		digits_index[7] = (sw_status & 0xf);
	}

	if (btn_status == 0x10) // 显示开关状态对应的10进制数到数码管的低5位
	{
		sw_status = Xil_In16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET) & 0xffff;
		sw_status_de = sw_status;

		digits_index[0] = 16;
		digits_index[1] = 16;
		digits_index[2] = 16;
		digits_index[3] = sw_status_de / 10000;
		digits_index[4] = (sw_status_de / 1000) % 10;
		digits_index[5] = (sw_status_de / 100) % 10;
		digits_index[6] = (sw_status_de / 10) % 10;
		digits_index[7] = sw_status_de % 10;
	}

	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_ISR_OFFSET, 0x1);
}