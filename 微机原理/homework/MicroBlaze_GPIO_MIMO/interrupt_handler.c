#include "xgpio.h"
#include "xintc.h"
#include "xparameters.h"

#define LED_CHANNEL 1
#define SWITCH_CHANNEL 2
#define BUTTON_CHANNEL 3
#define SEGMENT_CHANNEL 4

XGpio Gpio;
XIntc Intc;

volatile int speed = 100;

void update_leds(int speed)
{
    static int led_state = 0x01;
    XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, led_state);
    led_state = (led_state << 1)|((led_state >> 15) & 0x1);
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

void ButtonInterruptHandler(void *CallbackRef)
{
    int buttons = XGpio_DiscreteRead(&Gpio, BUTTON_CHANNEL);

    if (buttons & 0x01) speed += 10;
    if (buttons & 0x02) speed -= 10;
    if (speed < 10) speed = 10;

    XGpio_InterruptClear(&Gpio, 0xFFFFFFFF);
}

int main()
{
    XGpio_Initialize(&Gpio, XPAR_AXI_GPIO_0_DEVICE_ID);
    XIntc_Initialize(&Intc, XPAR_INTC_0_DEVICE_ID);

    XGpio_SelectDataDirection(&Gpio, LED_CHANNEL, 0x0);
    XGpio_SelectDataDirection(&Gpio, BUTTON_CHANNEL, 0xFFFF);
    
    XGpio_InterruptEnable(&Gpio, 0xFFFFFFFF);
    XGpio_InterruptGlobalEnable(&Gpio);

    XIntc_Connect(&Intc, XPAR_INTC_0_GPIO_0_VEC_ID, ButtonInterruptHandler, &Gpio);
    XIntc_Start(&Intc, XIN_REAL_MODE);

    while (1)
    {
        update_leds(speed);
        display_switches_on_segments();
    }

    return 0;
}