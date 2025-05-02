#include "bsp_accel.h"

void BSP_Accel_Init(void)
{
    // Enable ADC0 and GPIOE
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    
    // Wait for the peripherals to be ready
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

    // Configure PE4 and PE3 for ADC function
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);

    // Disable the ADC sequencer before configuring
    // ADCSequenceDisable(ADC0_BASE, 2);

    // Configure ADC sequencer 1 for PE0 (AIN9) and PE1 (AIN0)
    ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 3);

    // Step 1: Sample PE4 (AIN9)
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH3);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);

    // Enable ADC sequence 1
    ADCSequenceEnable(ADC0_BASE, 2);
    
    // Clear any existing ADC interrupt flags
    // ADCIntClear(ADC0_BASE, 2);
}

void BSP_Accel_Read(uint32_t *x, uint32_t *y, uint32_t *z)
{
    uint32_t adcValues[3];

    // Clear interrupt flag before triggering
    ADCIntClear(ADC0_BASE, 2);

    // Trigger the ADC conversion
    ADCProcessorTrigger(ADC0_BASE, 2);

    // Wait for the conversion to complete
    while (!ADCIntStatus(ADC0_BASE, 2, false));

    // Clear the ADC interrupt flag
    ADCIntClear(ADC0_BASE, 2);

    // Read ADC values
    ADCSequenceDataGet(ADC0_BASE, 2, adcValues);

    // Store results
    *x = adcValues[0];  // PE4 (AIN9)
    *y = adcValues[1];  // PE3 (AIN0)
    *z = adcValues[2];  // PE3 (AIN0)
}