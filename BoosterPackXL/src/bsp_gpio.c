#include "bsp_gpio.h"


void BSP_GPIO_Init(void)
{ 
    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOH)) {}
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL)) {}
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)) {}

    //
    // Enable the GPIO pin for the LED (PN0).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_2);
}


void BSP_GPIO_Write(uint32_t ui32Port, uint8_t ui8Pins, uint8_t ui8Val)
{
    GPIOPinWrite(ui32Port, ui8Pins, ui8Val);
}