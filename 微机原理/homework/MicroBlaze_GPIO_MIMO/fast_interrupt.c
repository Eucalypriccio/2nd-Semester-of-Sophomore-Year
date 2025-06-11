#include "xgpio.h"
#include "xparameters.h"
#include "xintc.h"
#include "xil_exception.h"

#define LED_CHANNEL 1
#define SWITCH_CHANNEL 2
#define BUTTON_CHANNEL 3
#define SEGMENT_CHANNEL 4

XGpio Gpio;                  
XIntc Intc;                  

volatile int speed = 100;    
volatile int interrupt_flag = 0; 

void delay(int delay_ms) {
    for (volatile int i = 0; i < delay_ms * 1000; i++);
}

void update_leds(int speed) {
    static int led_state = 0x01;
    XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, led_state);
    led_state = (led_state << 1) | ((led_state >> 15) & 0x1);
    delay(speed);
}

void display_switches_on_segments()
{
    u16 switch_state = XGpio_DiscreteRead(&Gpio, SWITCH_CHANNEL);
    for (int i = 0; i < 4; i++)
    {
        u8 digit = (switch_state >> (i*4)) & 0xF;
        XGpio_DiscreteWrite(&Gpio, SEGMENT_CHANNEL, digit);
        delay(2);
    }
}

void ButtonInterruptHandler(void *CallbackRef) {
    int buttons = XGpio_DiscreteRead(&Gpio, BUTTON_CHANNEL);

    if (buttons & 0x01) speed += 10;
    if (buttons & 0x02) speed -= 10;
    if (speed < 10) speed = 10;

    XGpio_InterruptClear(&Gpio, 0xFFFFFFFF);
    interrupt_flag = 1;
}

int InterruptSystemSetup(XIntc *IntcInstancePtr) {
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
                                 (Xil_ExceptionHandler)XIntc_InterruptHandler,
                                 IntcInstancePtr);

    Xil_ExceptionEnable();
    return XST_SUCCESS;
}

int SetupInterrupts() {
    int Status;

    Status = XIntc_Initialize(&Intc, XPAR_INTC_0_DEVICE_ID);
    if (Status != XST_SUCCESS) return XST_FAILURE;

    Status = XIntc_Connect(&Intc, XPAR_INTC_0_GPIO_0_VEC_ID,
                           (XInterruptHandler)ButtonInterruptHandler, &Gpio);
    if (Status != XST_SUCCESS) return XST_FAILURE;

    Status = XIntc_Start(&Intc, XIN_REAL_MODE);
    if (Status != XST_SUCCESS) return XST_FAILURE;

    XGpio_InterruptEnable(&Gpio, 0xFFFFFFFF);
    XGpio_InterruptGlobalEnable(&Gpio);

    InterruptSystemSetup(&Intc);

    return XST_SUCCESS;
}

int main() {
    int Status;

    Status = XGpio_Initialize(&Gpio, XPAR_AXI_GPIO_0_DEVICE_ID);
    if (Status != XST_SUCCESS) return XST_FAILURE;

    XGpio_SetDataDirection(&Gpio, LED_CHANNEL, 0x0);
    XGpio_SetDataDirection(&Gpio, SWITCH_CHANNEL, 0xFFFF);
    XGpio_SetDataDirection(&Gpio, BUTTON_CHANNEL, 0xFFFF);

    Status = SetupInterrupts();
    if (Status != XST_SUCCESS) return XST_FAILURE;

    while (1) {
        if (interrupt_flag) {
            xil_printf("Speed updated: %d ms\n", speed);
            interrupt_flag = 0;
        }

        update_leds(speed);
        display_switches_on_segments();
    }

    return 0;
}