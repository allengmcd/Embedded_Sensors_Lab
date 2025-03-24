#include "bsp_ssi.h"


//****************************************************************************
//
// The variable g_ui32SysClock contains the system clock frequency in Hz.
//
//****************************************************************************
uint32_t g_ui32SysClock_ssi;

//*****************************************************************************
//
// Global flag to indicate data was received
//
//*****************************************************************************
volatile uint32_t g_bReceiveFlag = 0;

//*****************************************************************************
//
// Number of bytes to send and receive.
//
//*****************************************************************************
#define NUM_SSI_DATA            4


void BSP_SSI_Init(void)
{ 
    // Enable the GPIO ports for PN2 and PL3
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);

    // Wait for the peripherals to be ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)) {}
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI2)) {}

    // Configure PD0 (MISO), PD1 (MOSI), and PD3 (CLK) for SSI2
    GPIOPinConfigure(GPIO_PD0_SSI2XDAT1);
    GPIOPinConfigure(GPIO_PD1_SSI2XDAT0);
    // GPIOPinConfigure(GPIO_PD2_SSI2FSS);
    GPIOPinConfigure(GPIO_PD3_SSI2CLK);

    // // Configure PN2 and PL3 as output pins
    // GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
    // GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_1);
    // GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_2);
    // GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_3);

    GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3);

    //
    // Configure and enable the SSI1 port for SPI slave mode.
    //
    SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 8);
    //SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 2000000, 8);

    //
    // Enable the SSI2 modules.
    //
    SSIEnable(SSI2_BASE);
}


void BSP_SSI_Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{ 
    uint32_t ui32Index;

    for(ui32Index = 0; ui32Index < ui32Count; ui32Index++)
    {
        SSIDataPut(SSI2_BASE, pui8Buffer[ui32Index]);
    }

    // UARTprintf("SSIBusy...\n  ");
    // while(SSIBusy(SSI2_BASE))
    // {
    // }
}
