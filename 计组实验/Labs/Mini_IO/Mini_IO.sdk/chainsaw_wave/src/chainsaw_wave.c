//DAC:SPI0
# include "math.h"
# include "xgpio_l.h"
# include "stdio.h"
# include "xtmrctr_l.h"
# include "xspi_l.h"
# include "xintc_l.h"
# include "xil_io.h"
# include "xil_exception.h"

# define VALUE_switch 1000000 - 2
# define spi_T   46875-2
# define change_T 48958 //����60ms

void timer_handle()  __attribute__ ((fast_interrupt));
void timer0_handle(); // �������
void timer1_handle(); // ��ʱ��ȡ���ص�ֵ

int samples[128]; // �����㣬�����γɾ�ݲ�
int int_times;
int T; // ����ֵ

int main()
{
    int status;
    Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI_OFFSET, 0xffff);//sw

    //��ʱ��T0��ʼ������������
    Xil_Out32(XPAR_TMRCTR_0_BASEADDR + XTC_TLR_OFFSET, spi_T);
    Xil_Out32(XPAR_TMRCTR_0_BASEADDR + XTC_TCSR_OFFSET, XTC_CSR_INT_OCCURED_MASK | XTC_CSR_AUTO_RELOAD_MASK |
        XTC_CSR_DOWN_COUNT_MASK | XTC_CSR_LOAD_MASK | XTC_CSR_ENABLE_INT_MASK);
    status = Xil_In32(XPAR_TMRCTR_0_BASEADDR + XTC_TCSR_OFFSET);
    status = (status & (~XTC_CSR_LOAD_MASK)) | XTC_CSR_ENABLE_TMR_MASK;
    Xil_Out32(XPAR_TMRCTR_0_BASEADDR + XTC_TCSR_OFFSET, status);

    //��ʼ��T1����ȡ����ֵ
    Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TIMER_COUNTER_OFFSET + XTC_TCSR_OFFSET,
        Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TIMER_COUNTER_OFFSET + XTC_TCSR_OFFSET) & ~XTC_CSR_ENABLE_TMR_MASK);
    Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TIMER_COUNTER_OFFSET + XTC_TLR_OFFSET, VALUE_switch);
    Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TIMER_COUNTER_OFFSET + XTC_TCSR_OFFSET,
        Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TIMER_COUNTER_OFFSET + XTC_TCSR_OFFSET) | XTC_CSR_LOAD_MASK);
    Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TIMER_COUNTER_OFFSET + XTC_TCSR_OFFSET,
        (Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TIMER_COUNTER_OFFSET + XTC_TCSR_OFFSET)
        & ~XTC_CSR_LOAD_MASK) | XTC_CSR_ENABLE_TMR_MASK | XTC_CSR_AUTO_RELOAD_MASK | XTC_CSR_DOWN_COUNT_MASK);

    //spi��ʼ��
    Xil_Out32(XPAR_SPI_0_BASEADDR + XSP_SRR_OFFSET, XSP_SRR_RESET_MASK);
    Xil_Out32(XPAR_SPI_0_BASEADDR + XSP_CR_OFFSET, XSP_CR_ENABLE_MASK | XSP_CR_MASTER_MODE_MASK |
        XSP_CR_CLK_POLARITY_MASK | XSP_CR_TXFIFO_RESET_MASK | XSP_CR_RXFIFO_RESET_MASK);
    Xil_Out32(XPAR_SPI_0_BASEADDR + XSP_SSR_OFFSET, 0x0);
    Xil_Out32(XPAR_SPI_0_BASEADDR + XSP_DTR_OFFSET, 0x0);


    //�жϿ�������ʼ��
    Xil_Out32(XPAR_INTC_0_BASEADDR + XIN_IAR_OFFSET,Xil_In32(XPAR_INTC_0_BASEADDR + XIN_ISR_OFFSET));
    Xil_Out32(XPAR_INTC_0_BASEADDR + XIN_IER_OFFSET,XPAR_AXI_TIMER_0_INTERRUPT_MASK);
    Xil_Out32(XPAR_INTC_0_BASEADDR + XIN_IMR_OFFSET,XPAR_AXI_TIMER_0_INTERRUPT_MASK);
    Xil_Out32(XPAR_INTC_0_BASEADDR + XIN_MER_OFFSET,XIN_INT_MASTER_ENABLE_MASK|XIN_INT_HARDWARE_ENABLE_MASK);
    Xil_Out32(XPAR_INTC_0_BASEADDR + XIN_IVAR_OFFSET + 4 * XPAR_INTC_0_TMRCTR_0_VEC_ID, (int)timer_handle);

    microblaze_enable_interrupts();
    for (int i = 0 ; i < 128; i++)
    {
        samples[i] = 32 * i;
    }
    return 0;
}

void timer_handle() {
    int status;
    // �ж� T0 �ж�
    status = Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET);
    if ((status & XTC_CSR_INT_OCCURED_MASK) == XTC_CSR_INT_OCCURED_MASK) {
        timer0_handle();
    }
    Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET, status);

    // �ж� T1 �ж�
    status = Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TIMER_COUNTER_OFFSET + XTC_TCSR_OFFSET);
    if ((status & XTC_CSR_INT_OCCURED_MASK) == XTC_CSR_INT_OCCURED_MASK) {
        timer1_handle();
    }
    Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TIMER_COUNTER_OFFSET + XTC_TCSR_OFFSET, status);
}

// T0 �ж�������
void timer0_handle() {
    int_times++;
    if(int_times == 128)
       int_times = 0;
    Xil_Out32(XPAR_SPI_0_BASEADDR + XSP_DTR_OFFSET, samples[int_times]);
}
// T1 �ж�������
void timer1_handle() {
    T = Xil_In16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET) & 0xf;
    Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TLR_OFFSET, spi_T + T * change_T);
}
