#include "stdio.h"
#include "xil_io.h"
#include "xgpio_l.h"

int main ()
{
	unsigned short switch_num1, switch_num2; // ����״̬��Ӧ�Ķ���������
	unsigned short add_result, multiply_result; // �����������˵Ľ��
	int read_flag1, read_flag2; // �Ƿ��Ѷ�ȡ����״̬�ı�־
	read_flag1 = 0;
	read_flag2 = 0;

	Xil_Out8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_TRI_OFFSET, 0x1f); // ��ȡ��ť״̬
	Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI_OFFSET, 0xffff); // ��ȡ����״̬
	Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI2_OFFSET, 0x0); // ������״̬�����LED��

	while(1)
	{
		if ((Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f) == 0x1) // ����BTNC
		{
			Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, 0x0);
			switch_num1 = Xil_In16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET) & 0xffff;
			read_flag1 = 1;
			while ((Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f) != 0); // �ȴ������ɿ�
			Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, switch_num1);
		}
		if ((Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f) == 0x8) // ����BTNR
		{
			Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, 0x0);
			switch_num2 = Xil_In16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET) & 0xffff;
			read_flag2 = 1;
			while ((Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f) != 0); // �ȴ������ɿ�
			Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, switch_num2);
		}
		if ((read_flag1 != 0) && (read_flag2 != 0))
		{
			if ((Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f) == 0x2) // ����BTNU
			{
				Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, 0x0);
				add_result = switch_num1 + switch_num2;
				while ((Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f) != 0);
				Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, add_result);
			}
			if ((Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f) == 0x10) // ����BTND
			{
				Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, 0x0);
				multiply_result = switch_num1 * switch_num2;
				while ((Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f) != 0);
				Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, multiply_result);
			}
		}
	}
	return 0;
}
