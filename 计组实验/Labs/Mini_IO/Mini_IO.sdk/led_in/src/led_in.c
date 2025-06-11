/*
 * led_in.c
 *
 *  Created on: 2025Äê5ÔÂ13ÈÕ
 *      Author: Eureka is exploring
 */

#include "stdio.h"
#include "xil_io.h"
#include "xgpio_l.h"

int main()
{
	unsigned short led;
	unsigned short byte;
	Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI2_OFFSET, 0x0);
	while (1)
	{
		xil_printf("input the 16 bits hexadecimal number to be displayed:\r\n");
		led = 0;
		while (1)
		{
			byte = inbyte();
			if (byte == 0x0d)
			{
				break;
			}
			else
			{
				if (byte >= 'a')
				{
					byte = byte - 0x57;
				}
				else if (byte >= 'A')
				{
					byte = byte - 0x37;
				}
				else
				{
					byte = byte - 0x30;
				}
				led = (led << 4) + byte;
			}
			Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, led);
		}
	}

}
