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

// DMA and SSI variables
static uint32_t g_ui32DMABufferSize;
#define MAX_DMA_BUFFER_SIZE 4096  // Adjust based on your needs
static uint8_t g_pui8DMABuffer[MAX_DMA_BUFFER_SIZE];
// Global DMA control table - must be aligned on a 1024-byte boundary
uint8_t g_pui8ControlTable[1024] __attribute__ ((aligned(1024)));




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
    SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1200000, 8);
    //SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 2000000, 8);

    //
    // Enable the SSI2 modules.
    //
    SSIEnable(SSI2_BASE);
}


// Initialize DMA for SSI
void BSP_SSI_Init_DMA(void)
{
    // Enable the uDMA peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UDMA)) {}
    
    // Enable the uDMA controller
    uDMAEnable();
    
    // Set the control table base address
    uDMAControlBaseSet(g_pui8ControlTable);
    
    // Configure channel for SSI2 TX (use appropriate channel for TM4C1294XL)
    // The TM4C1294XL assigns SSI2 TX to DMA channel 13
    uDMAChannelAssign(UDMA_CH13_SSI2TX);
    
    // Put the attributes in a known state
    uDMAChannelAttributeDisable(UDMA_CH13_SSI2TX, 
                               UDMA_ATTR_ALTSELECT | UDMA_ATTR_HIGH_PRIORITY |
                               UDMA_ATTR_REQMASK);
    
    // Configure the control parameters
    uDMAChannelControlSet(UDMA_CH13_SSI2TX | UDMA_PRI_SELECT,
                         UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE |
                         UDMA_ARB_4);
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

void BSP_SSI_Send_16(const uint16_t *pui8Buffer, uint32_t ui32Count)
{ 
    uint32_t ui32Index = 0;

    while (ui32Index < ui32Count)
    {
        // Wait if FIFO is full before writing more data
        while (SSIBusy(SSI2_BASE)) {}

        // Fill the FIFO with up to 8 bytes at a time
        for (int i = 0; i < 4 && ui32Index < ui32Count; i++, ui32Index++)
        {
            SSIDataPut(SSI2_BASE, (uint8_t)(pui8Buffer[ui32Index] >> 8));
            SSIDataPut(SSI2_BASE, (uint8_t)pui8Buffer[ui32Index]);
        }
    }

    // Ensure all data has been transmitted before returning
    while (SSIBusy(SSI2_BASE)) {}
}

// Convert uint16_t buffer to uint8_t buffer for DMA transfer
void ConvertBuffer(const uint16_t *pui16Buffer, uint8_t *pui8Buffer, uint32_t ui32Count)
{
    for (uint32_t i = 0; i < ui32Count; i++)
    {
        pui8Buffer[i*2] = (uint8_t)(pui16Buffer[i] >> 8);    // High byte
        pui8Buffer[i*2+1] = (uint8_t)(pui16Buffer[i]);       // Low byte
    }
}

// DMA-based SSI send function without interrupts (using polling)
void BSP_SSI_Send_16_DMA(const uint16_t *pui16Buffer, uint32_t ui32Count)
{
    // Calculate buffer size (each uint16_t expands to 2 bytes)
    uint32_t ui32ByteCount = ui32Count * 2;
    
    // Check if buffer is large enough
    if (ui32ByteCount > MAX_DMA_BUFFER_SIZE)
    {
        // Either handle in chunks or fall back to non-DMA method
        BSP_SSI_Send_16(pui16Buffer, ui32Count);
        return;
    }
    
    // Convert uint16_t buffer to uint8_t buffer
    for (uint32_t i = 0; i < ui32Count; i++)
    {
        g_pui8DMABuffer[i*2] = (uint8_t)(pui16Buffer[i] >> 8);    // High byte
        g_pui8DMABuffer[i*2+1] = (uint8_t)(pui16Buffer[i]);       // Low byte
    }

    
    // Convert uint16_t buffer to uint8_t buffer
    ConvertBuffer(pui16Buffer, g_pui8DMABuffer, ui32Count);
    
    // Enable DMA for SSI2 TX
    SSIDMAEnable(SSI2_BASE, SSI_DMA_TX);
    
    // Configure the DMA channel for the transfer
    uDMAChannelTransferSet(UDMA_CH13_SSI2TX | UDMA_PRI_SELECT,
                          UDMA_MODE_BASIC,
                          g_pui8DMABuffer,
                          (void *)(SSI2_BASE + SSI_O_DR),
                          g_ui32DMABufferSize);
    
    // Enable the DMA channel
    uDMAChannelEnable(UDMA_CH13_SSI2TX);
    
    // Poll until the DMA transfer is complete
    // For basic mode, wait until the channel is done (shows as disabled)
    while(uDMAChannelIsEnabled(UDMA_CH13_SSI2TX))
    {
        // In an RTOS environment, you might want to yield here
        // osThreadYield(); // Example if using CMSIS-RTOS
    }
    
    // Disable DMA for SSI2 TX
    SSIDMADisable(SSI2_BASE, SSI_DMA_TX);
    
    
    // Ensure all data has been transmitted before returning
    while (SSIBusy(SSI2_BASE)) {}
}