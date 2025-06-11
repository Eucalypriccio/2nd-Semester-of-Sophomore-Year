/*
 * switch_in.c
 *
 *  Created on: 2025Äê5ÔÂ13ÈÕ
 *      Author: Eureka is exploring
 */

#include "stdio.h"
#include "xil_io.h"
#include "xgpio_l.h"

int main()
{
	unsigned short last_sw, current_sw;
	Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI_OFFSET, 0xffff); // the 1st channel works in input direction

	while(1)
	{
		last_sw = current_sw;
		current_sw = Xil_In16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET) & 0xffff; // read the data in the 1st channel's data reg
		if (last_sw != current_sw)
		{
			xil_printf("the switches' code is 0x%4x\n", current_sw);
		}
	}

}
