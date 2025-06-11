/*
 * seg_display.c
 *
 *  Created on: 2025Äê5ÔÂ13ÈÕ
 *      Author: Eureka is exploring
 */

#include "stdio.h"
#include "xil_io.h"
#include "xgpio_l.h"

int main()
{
	char segcode[8] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8};
	short pos = 0xff7f;
	int i, j;
	Xil_Out8(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_TRI_OFFSET, 0x0);
	Xil_Out8(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_TRI2_OFFSET, 0x0);

	while (1)
	{
		for (i = 0; i < 8; i++)
		{
			Xil_Out16(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_DATA2_OFFSET, segcode[i]);
			Xil_Out16(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_DATA_OFFSET, pos);
			for (j = 0; j < 1000; j++);
			pos = pos >> 1;
		}
		pos = 0xff7f;
	}

	return 0;
}
