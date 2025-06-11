#include "stdio.h"
#include "xil_io.h"
#include "xgpio_l.h"
#include "xintc_l.h"
#include "sleep.h"

void interruptHub(void)__attribute__((interrupt_handler));
void btnHandler(void);
void swHandler(void);

u8 seg_code[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90}; // ����0-9��Ӧ�Ķ���
u8 digits_index[8] = {0}; // ����ܸ�λ��Ӧ�������ڶ�����ж�Ӧ������
u32 btn_status = 0x0;
u32 sw_status;

int main()
{
	Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI_OFFSET, 0xffff); // ��ȡ����״̬
	Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI2_OFFSET, 0x0); // ������״̬�����LED��
	Xil_Out16(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_TRI_OFFSET, 0x1f); // ��ȡ��ť״̬
	Xil_Out16(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_TRI_OFFSET, 0x0); // ��������λѡ�ź�
	Xil_Out16(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_TRI2_OFFSET, 0x0); // �������ܶ�ѡ�ź�

	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_ISR_OFFSET, 0x1); // ���GPIO_2��ͨ��1����ť�����ж�״̬
	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_IER_OFFSET, 0x1); // ʹ��ͨ��1�ж�
	Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_GIE_OFFSET, 0x80000000); // ʹ���ж����

	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_ISR_OFFSET, 0x1); // ���GPIO_0��ͨ��1�����أ����ж�״̬
	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_IER_OFFSET, 0x1); // ʹ��ͨ��1�ж�
	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_GIE_OFFSET, 0x80000000); // ʹ���ж����

	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IAR_OFFSET, 0xffffffff); // �������ͨ�����ж�״̬
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IER_OFFSET, 0x1 | 0x2); // ʹ��intr[0]��intr[1]�ж�
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_MER_OFFSET, 0x3); // ʹ��Ӳ���ж�irq���

	microblaze_enable_interrupts(); // ʹ���жϿ�����

	while(1)
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

void interruptHub()
{
	int intr_status;
	intr_status = Xil_In32(XPAR_INTC_0_BASEADDR + XIN_ISR_OFFSET);

	if (intr_status == 0x1) // �ж��������Կ���
	{
		swHandler();
	}

	if (intr_status == 0x2) // �ж��������԰�ť
	{
		btnHandler();
	}

	// ����ж�״̬
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IAR_OFFSET, 0xffffffff);
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
