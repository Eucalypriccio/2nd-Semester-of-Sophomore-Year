#include "stdio.h"
#include "xil_io.h"
#include "xgpio_l.h"
#include "xintc_l.h"

void btnHandler(void)__attribute__((fast_interrupt));
void swHandler(); // ���뿪��״̬����ʾ��led��

unsigned short switch_num1, switch_num2; // ����״̬��Ӧ�Ķ���������
unsigned short add_result, multiply_result; // �����������˵Ľ��
unsigned short btn_status; // ��ť״̬
volatile int read_flag1 = 0;
volatile int read_flag2 = 0;

int main()
{
	Xil_Out8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_TRI_OFFSET, 0x1f); // ��ȡ��ť״̬
	Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI_OFFSET, 0xffff); // ��ȡ����״̬
	Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI2_OFFSET, 0x0); // ������״̬�����LED��

	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_ISR_OFFSET, 0x1); // ���GPIO_2��ͨ��1����ť�����ж�״̬
	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_IER_OFFSET, 0x1); // ʹ��ͨ��1�ж�
	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_GIE_OFFSET, 0x80000000); // ʹ���ж����

	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IAR_OFFSET, 0xffffffff); // �������ͨ�����ж�״̬
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IER_OFFSET, 0x2); // ʹ��intr[1]�ж�
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_MER_OFFSET, 0x3); // ʹ��Ӳ���ж�irq���

	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IMR_OFFSET, 0x2); //  ����intr[1]Ϊ�����ж�ģʽ
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IVAR_OFFSET + 4*1, (int)btnHandler); // ע���ж�����

	microblaze_enable_interrupts();
	while(1);
	return 0;
}

void btnHandler(void)
{
	btn_status = Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f;
	if (btn_status == 0x1) // ����BTNC
	{
		read_flag1 = 1;
		swHandler();
	}
	if (btn_status == 0x8) // ����BTNR
	{
		read_flag2 = 1;
		swHandler();
	}
	if ((read_flag1 != 0) && (read_flag2 != 0))
	{
		if (btn_status == 0x2) // ����BTNU
		{
			Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, 0x0);
			add_result = switch_num1 + switch_num2;
			while ((Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f) != 0);
			Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, add_result);
		}
		if (btn_status == 0x10) // ����BTND
		{
			Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, 0x0);
			multiply_result = switch_num1 * switch_num2;
			while ((Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f) != 0);
			Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, multiply_result);
		}
	}

	// ����ж�״̬
	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_ISR_OFFSET, 0x1);
//	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IAR_OFFSET, 0xffffffff); �����ж�ģʽ�Զ�����ж�״̬
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
