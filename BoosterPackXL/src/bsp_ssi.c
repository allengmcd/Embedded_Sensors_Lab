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
    // uint32_t pui32DataTx[NUM_SSI_DATA];
    uint32_t pui32DataRx[NUM_SSI_DATA];
    // uint32_t ui32Index;

    //
    // Run from the PLL at 120 MHz.
    // Note: SYSCTL_CFG_VCO_240 is a new setting provided in TivaWare 2.2.x and
    // later to better reflect the actual VCO speed due to SYSCTL#22.
    //
    g_ui32SysClock_ssi = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN |
                                             SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_240), 120000000);




    //
    // Display the setup on the console.
    //
    UARTprintf("SSI ->\n");
    UARTprintf("Mode: Advanced Quad-SPI\n");
    UARTprintf("Data: 8-bit\n\n");

    //
    // The SSI0 and SSI1 peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);

    //
    // For this example SSI0 is used with PortA[7:2].  The SSI1 uses
    // PortB, PortD and PortE for the SSICLK, SSIFss and the 4 data pins.
    // GPIO ports need to be enabled so those pins can be used.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    //
    // Configure the pin muxing for SSI0 functions on PA[7:2].
    // Configure the pin muxing for SSI1 functions on PB, PD and PE.
    //
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0XDAT0);
    GPIOPinConfigure(GPIO_PA5_SSI0XDAT1);
    GPIOPinConfigure(GPIO_PA6_SSI0XDAT2);
    GPIOPinConfigure(GPIO_PA7_SSI0XDAT3);

    GPIOPinConfigure(GPIO_PB5_SSI1CLK);
    GPIOPinConfigure(GPIO_PB4_SSI1FSS);
    GPIOPinConfigure(GPIO_PE4_SSI1XDAT0);
    GPIOPinConfigure(GPIO_PE5_SSI1XDAT1);
    GPIOPinConfigure(GPIO_PD4_SSI1XDAT2);
    GPIOPinConfigure(GPIO_PD5_SSI1XDAT3);

    //
    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.  Consult the data sheet to
    // see which functions are allocated per pin.
    // The pins are assigned as follows:
    //      SSI0
    //      PA7 - SSI0XDAT3
    //      PA6 - SSI0XDAT2
    //      PA5 - SSI0XDAT1
    //      PA4 - SSI0XDAT0
    //      PA3 - SSI0Fss
    //      PA2 - SSI0CLK
    //      SSI1
    //      PD5 - SSI1XDAT3
    //      PD4 - SSI1XDAT2
    //      PE5 - SSI1XDAT1
    //      PE4 - SSI1XDAT0
    //      PB4 - SSI1Fss
    //      PB5 - SSI1CLK
    //
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 |
                       GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2);
    GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_4 );
    GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_5 | GPIO_PIN_4 );
    GPIOPinTypeSSI(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_4 );

    //
    // Configure and enable the SSI0 port for SPI master mode.  Use SSI0,
    // system clock supply, idle clock level low and active low clock in
    // freescale SPI mode, master mode, 2MHz SSI frequency, and 8-bit data.
    // For SPI mode, you can set the polarity of the SSI clock when the SSI
    // unit is idle.  You can also configure what clock edge you want to
    // capture data on.  Please reference the device datasheet for more
    // information on the different SPI modes.
    //
    SSIConfigSetExpClk(SSI0_BASE, g_ui32SysClock_ssi, SSI_FRF_MOTO_MODE_0,
                           SSI_MODE_MASTER, 2000000, 8);

    //
    // Configure and enable the SSI1 port for SPI slave mode.
    //
    SSIConfigSetExpClk(SSI1_BASE, g_ui32SysClock_ssi, SSI_FRF_MOTO_MODE_0,
                           SSI_MODE_SLAVE, 2000000, 8);

    //
    // Enable Quad-SSI mode for both SSI0 and SSI1.
    //
    SSIAdvModeSet(SSI0_BASE, SSI_ADV_MODE_QUAD_WRITE);
    SSIAdvModeSet(SSI1_BASE, SSI_ADV_MODE_QUAD_READ);

    //
    // Hold the Fss pin low during transfers.  The Fss will be controlled
    // directly by the SSIAdvDataPutFrameEnd().  If calling
    // SSIAdvDataPutFrameEnd to write data to the FIFO, the Fss is de-asserted
    // for the corresponding data.
    //
    SSIAdvFrameHoldEnable(SSI0_BASE);

    //
    // Enable processor interrupts.
    //
    IntMasterEnable();

    //
    // Enable SSI1 interrupt on RX FIFO full.
    //
    SSIIntEnable(SSI1_BASE, SSI_RXFF);

    // //
    // // Enable the SSI1 interrupts on the processor (NVIC).
    // //
    // IntEnable(INT_SSI1);

    //
    // Enable the SSI0 and SSI1 modules.
    //
    SSIEnable(SSI0_BASE);
    SSIEnable(SSI1_BASE);

      //
    // Read any residual data from the SSI port.  This makes sure the receive
    // FIFOs are empty, so we don't read any unwanted junk.  This is done here
    // because the SPI SSI mode is full-duplex, which allows you to send and
    // receive at the same time.  The SSIDataGetNonBlocking function returns
    // "true" when data was returned, and "false" when no data was returned.
    // The "non-blocking" function checks if there is any data in the receive
    // FIFO and does not "hang" if there isn't.
    //
    while(SSIDataGetNonBlocking(SSI0_BASE, &pui32DataRx[0]))
    {
    }
    while(SSIDataGetNonBlocking(SSI1_BASE, &pui32DataRx[0]))
    {
    }
}


void BSP_SSI_Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{ 
    uint32_t ui32Index;

    for(ui32Index = 0; ui32Index < ui32Count; ui32Index++)
    {

        //
        // Dummy write to start slave which is required for Quad-SSI
        // mode operation.
        //
        SSIDataPut(SSI1_BASE, 0);

        //
        // Check if the last data byte is queued up to be sent.
        //
        if (ui32Index == (NUM_SSI_DATA - 1))
        {
            //
            // Calling SSIAdvDataPutFrameEnd on the last data will put out the
            // data and de-assert the the Fss pin.
            //
            SSIAdvDataPutFrameEnd(SSI0_BASE, pui8Buffer[ui32Index]);
        }
        else
        {
            //
            // Send the data using the "blocking" put function.  This function
            // will wait until there is room in the send FIFO before returning.
            // This allows you to assure that all the data you send makes it
            // into the send FIFO.
            //
            SSIDataPut(SSI0_BASE, pui8Buffer[ui32Index]);
        }
    }

}
