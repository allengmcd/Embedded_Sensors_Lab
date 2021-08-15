#include "BSP2.h"


void PortAIntHandler(void);

// ------------BSP_Joystick_Init------------
// Initialize a GPIO pin for input, which corresponds
// with BoosterPack pin J1.5 (Select button).
// Initialize two ADC pins, which correspond with
// BoosterPack pins J1.2 (X) and J3.26 (Y).
// Input: none
// Output: none
void BSP_Joystick_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_5);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // initialize ADC peripherals

    // initialize ADC sampling sequence
    //ADCSequenceDisable(ADC0_BASE, 0);
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 1);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH4);                           
    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH11 | ADC_CTL_IE | ADC_CTL_END); 
    ADCSequenceEnable(ADC0_BASE, 0);
}

// ------------BSP_Joystick_Input------------
// Read and return the immediate status of the
// joystick.  Button de-bouncing for the Select
// button is not considered.  The joystick X- and
// Y-positions are returned as 10-bit numbers,
// even if the ADC on the LaunchPad is more precise.
// Input: x is pointer to store X-position (0 to 1023)
//        y is pointer to store Y-position (0 to 1023)
//        select is pointer to store Select status (0 if pressed)
// Output: none
// Assumes: BSP_Joystick_Init() has been called
//void BSP_Joystick_Input(uint16_t *x, uint16_t *y, uint8_t *select){
uint32_t gJoystick[2] = {0};
void BSP_Joystick_Input(uint32_t *x, uint32_t *y, uint32_t *z)
{
    ADCProcessorTrigger(ADC0_BASE, 0);          // trigger the ADC sample sequence for Joystick X and Y
    while(!ADCIntStatus(ADC0_BASE, 0, false));  // wait until the sample sequence has completed
    ADCSequenceDataGet(ADC0_BASE, 0, gJoystick);// retrieve joystick data
    ADCIntClear(ADC0_BASE, 0);                  // clear ADC sequence interrupt flag
    
    *x = gJoystick[0];
    *y = gJoystick[1];
}


void BSP_Microphone_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_5);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // initialize ADC peripherals

    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 2);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH8 | ADC_CTL_IE | ADC_CTL_END);  // Joystick VER(Y)
    ADCSequenceEnable(ADC0_BASE, 1);
}


void BSP_Microphone_Input(uint32_t *x)
{
    ADCProcessorTrigger(ADC0_BASE, 1);          // trigger the ADC sample sequence for Joystick X and Y
    while(!ADCIntStatus(ADC0_BASE, 1, false));  // wait until the sample sequence has completed
    ADCSequenceDataGet(ADC0_BASE, 1, x);// retrieve joystick data
    ADCIntClear(ADC0_BASE, 1);                  // clear ADC sequence interrupt flag
}


void BSP_Accelerometer_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // initialize ADC peripherals

    // initialize ADC sampling sequences
    //ADCSequenceDisable(ADC0_BASE, 0);
    ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 3);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH7);                             // Joystick HOR(X)
    ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_CH6);  // Joystick VER(Y)
    ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_CH5 | ADC_CTL_IE | ADC_CTL_END);  // Joystick VER(Y)
    ADCSequenceEnable(ADC0_BASE, 2);  
}

uint32_t gAccel[3] = {0};
void BSP_Accelerometer_Input(uint32_t *x, uint32_t *y, uint32_t *z)
{
    ADCProcessorTrigger(ADC0_BASE, 2);          // trigger the ADC sample sequence for Joystick X and Y
    while(!ADCIntStatus(ADC0_BASE, 2, false));  // wait until the sample sequence has completed
    ADCSequenceDataGet(ADC0_BASE, 2, gAccel);// retrieve joystick data
    ADCIntClear(ADC0_BASE, 2);                  // clear ADC sequence interrupt flag
    
    *x = gAccel[0];
    *y = gAccel[1];
    *z = gAccel[2];
}

// PD6, PD7
//
void BSP_Button_Init(void)
{
    // Enable the GPIOD peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    //
    // Wait for the GPIOD module to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
    {
    }

    //
    // Initialize the GPIO pin configuration.
    //
    // Set pins 6, 7 as input, SW controlled.
    //
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    GPIOPadConfigSet(
        GPIO_PORTD_BASE,
        GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}


void BSP_Button_Input(uint32_t *isPressed, uint8_t button)
{
    //
    // Read some pins.
    //
    *isPressed = (uint32_t)GPIOPinRead(GPIO_PORTD_BASE, button);
}

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void BSP_UART_Init(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    MAP_UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}
