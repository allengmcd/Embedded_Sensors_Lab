#include "bsp_mic.h"

void BSP_Mic_Init(void)
{
// Enable ADC0 and GPIOE
SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

// Wait for the peripherals to be ready
while (!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));
while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

// Configure PE4 and PE3 for ADC function
GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_5);

// Disable the ADC sequencer before configuring
// ADCSequenceDisable(ADC0_BASE, 2);

// Configure ADC sequencer 1 for PE0 (AIN9) and PE1 (AIN0)
ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 3);

// Step 1: Sample PE4 (AIN9)
ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH8 | ADC_CTL_IE | ADC_CTL_END);

// Enable ADC sequence 1
ADCSequenceEnable(ADC0_BASE, 3);

// Clear any existing ADC interrupt flags
// ADCIntClear(ADC0_BASE, 2);
}


void BSP_Mic_Read(uint32_t *signal)
{
    ADCProcessorTrigger(ADC0_BASE, 3);          // trigger the ADC sample sequence for Joystick X and Y
    while(!ADCIntStatus(ADC0_BASE, 3, false));  // wait until the sample sequence has completed
    ADCSequenceDataGet(ADC0_BASE, 3, signal);// retrieve joystick data
    ADCIntClear(ADC0_BASE, 3);                  // clear ADC sequence interrupt flag
}