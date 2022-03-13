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
        // Enable the GPIOD peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    //
    // Wait for the GPIOD module to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
    }

    //
    // Initialize the GPIO pin configuration.
    //
    // Set pins 6, 7 as input, SW controlled.
    //
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);

    GPIOPadConfigSet(
        GPIO_PORTE_BASE,
        GPIO_PIN_4,
        GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

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

void BSP_Joystick_Select(uint32_t *isPressed)
{
    //
    // Read some pins.
    //
    *isPressed = (uint32_t)GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4);
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
    //HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    //HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= GPIO_PIN_7;
    //HWREG(GPIO_PORTD_BASE + GPIO_O_AFSEL) &= ~GPIO_PIN_7;  
    //HWREG(GPIO_PORTD_BASE + GPIO_O_DEN) |= GPIO_PIN_7;
    //HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;       

    // Enable the GPIOD peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    //
    // Wait for the GPIOD module to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
    {
    }


    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= GPIO_PIN_7;
    HWREG(GPIO_PORTD_BASE + GPIO_O_AFSEL) &= ~GPIO_PIN_7;  
    HWREG(GPIO_PORTD_BASE + GPIO_O_DEN) |= GPIO_PIN_7;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;       


    //
    // Initialize the GPIO pin configuration.
    //
    // Set pins 6, 7 as input, SW controlled.
    //
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
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



// ------------BSP_LightSensor_Init------------
// Initialize a GPIO pin for input, which corresponds
// with BoosterPack pins J1.8 (Light Sensor interrupt).
// Initialize two I2C pins, which correspond with
// BoosterPack pins J1.9 (SCL) and J1.10 (SDA).
// Input: none
// Output: none
void BSP_LightSensor_Init(void)
{
    //
    // Enable the I2C0 peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    //
    // Wait for the I2C0 module to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1))
    {
    }

    //
    // Initialize Master and Slave
    //
    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), true);
    //
    // Specify slave address
    //
    I2CMasterSlaveAddrSet(I2C1_BASE, SHT21_I2C_ADDRESS, false);


    // Enable the GPIOA peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Wait for the GPIOA module to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }

    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
}


int16_t lsb;
int16_t msb;
int16_t three;

void BSP_LightSensor_Send()
{
    //specify that we are writing (a register address) to the
    //slave device
    I2CMasterSlaveAddrSet(I2C1_BASE, OPT3001_I2C_ADDRESS, false);

    //specify register to be read
    I2CMasterDataPut(I2C1_BASE, MANUFACTUREID_REG);

    //send control byte and register address byte to slave device
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    // Wait until the slave has received and acknowledged the data.
    SysCtlDelay(100);
    while(I2CMasterBusy(I2C1_BASE));
}

void BSP_LightSensor_Receive(int16_t *result)
{
    //specify that we are reading (a register address) to the
    //slave device
    I2CMasterSlaveAddrSet(I2C1_BASE, OPT3001_I2C_ADDRESS, true);

    //send control byte and read from the register we
    //specified
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);

    //wait for MCU to finish transaction
    SysCtlDelay(100);
    while(I2CMasterBusy(I2C1_BASE));

    msb = (I2CMasterDataGet(I2C1_BASE) & 0xFF);

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH); //Receive

    SysCtlDelay(100);
    while(I2CMasterBusy(I2C1_BASE));

    lsb = (I2CMasterDataGet(I2C1_BASE) & 0xFF );

    *result = (msb << 8) | lsb;
}