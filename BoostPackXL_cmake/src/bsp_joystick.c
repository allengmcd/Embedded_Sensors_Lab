// #include "bsp_joystick.h"

// // ------------BSP_Joystick_Init------------
// // Initialize a GPIO pin for input, which corresponds
// // with BoosterPack pin J1.5 (Select button).
// // Initialize two ADC pins, which correspond with
// // BoosterPack pins J1.2 (X) and J3.26 (Y).
// // Input: none
// // Output: none
// void BSP_Joystick_Init(void)
// {
//     // Enable ADC0 and GPIOE
//     SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
//     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    
//     // Wait for the peripherals to be ready
//     while (!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));
//     while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

//     // Configure PE4 and PE3 for ADC function
//     GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3 | GPIO_PIN_4);

//     // Disable the ADC sequencer before configuring
//     ADCSequenceDisable(ADC0_BASE, 1);

//     // Configure ADC sequencer 1 for PE4 (AIN9) and PE3 (AIN0)
//     ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);

//     // Step 1: Sample PE4 (AIN9)
//     ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH9);
//     // Step 2: Sample PE3 (AIN0) and mark as last step
//     ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);

//     // Enable ADC sequence 1
//     ADCSequenceEnable(ADC0_BASE, 1);
    
//     // Clear any existing ADC interrupt flags
//     ADCIntClear(ADC0_BASE, 1);
// }

// // ------------BSP_Joystick_Input------------
// // Read and return the immediate status of the
// // joystick.  Button de-bouncing for the Select
// // button is not considered.  The joystick X- and
// // Y-positions are returned as 10-bit numbers,
// // even if the ADC on the LaunchPad is more precise.
// // Input: x is pointer to store X-position (0 to 1023)
// //        y is pointer to store Y-position (0 to 1023)
// //        select is pointer to store Select status (0 if pressed)
// // Output: none
// // Assumes: BSP_Joystick_Init() has been called
// //void BSP_Joystick_Input(uint16_t *x, uint16_t *y, uint8_t *select){
// uint32_t gJoystick[2] = {0};
// void BSP_Joystick_Input(uint32_t *x, uint32_t *y, uint32_t *z)
// {
//     uint32_t adcValues[2];

//     // Clear interrupt flag before triggering
//     ADCIntClear(ADC0_BASE, 1);

//     // Trigger the ADC conversion
//     ADCProcessorTrigger(ADC0_BASE, 1);

//     // Wait for the conversion to complete
//     while (!ADCIntStatus(ADC0_BASE, 1, false));

//     // Clear the ADC interrupt flag
//     ADCIntClear(ADC0_BASE, 1);

//     // Read ADC values
//     ADCSequenceDataGet(ADC0_BASE, 1, adcValues);

//     // Store results
//     *x = adcValues[0];  // PE4 (AIN9)
//     *y = adcValues[1];  // PE3 (AIN0)
// }

// void BSP_Joystick_Select(uint32_t *isPressed)
// {
//     //
//     // Read some pins.
//     //
//     *isPressed = (uint32_t)GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4);
// }
