/*
 * button_in.c
 *
 *  Created on: 2025Äê5ÔÂ13ÈÕ
 *      Author: Eureka is exploring
 */

#include "stdio.h"
#include "xil_io.h"
#include "xgpio_l.h"

int main()
{
	char button;
	Xil_Out8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_TRI_OFFSET, 0x1f);
	while(1)
	{
		while ((Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f) != 0)
		{
			button = Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f;
			while ((Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f) != 0); // waiting for the button to be released
			xil_printf("the pushed button's code is 0x%x\n", button);
		}
	}

}
