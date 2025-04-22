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
// static uint32_t g_ui32DMABufferSize;
#define UDMA_SSI_BUFFER_SIZE 1024
#define MAX_DMA_BUFFER_SIZE 4096  // Adjust based on your needs
#define MAX_STATIC_BUFFER_SIZE 128*128*2
// static uint8_t g_pui8DMABuffer[MAX_DMA_BUFFER_SIZE];
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
    
    // Set up the control table base pointer
    uDMAControlBaseSet(g_pui8ControlTable);
    
    // Configure the SSI2 TX DMA channel
    uDMAChannelAttributeDisable(UDMA_CH13_SSI2TX, 
                                UDMA_ATTR_ALTSELECT | UDMA_ATTR_HIGH_PRIORITY |
                                UDMA_ATTR_REQMASK);
    
    // Enable DMA for SSI2 TX
    SSIDMAEnable(SSI2_BASE, SSI_DMA_TX);
    
    // Set the SSI2 to trigger the uDMA controller
    uDMAChannelAssign(UDMA_CH13_SSI2TX);  // Assign the SSI2 TX to DMA channel 12
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

// Convert 16-bit buffer to 8-bit buffer for DMA transfer
static void ConvertBuffer16To8(const uint16_t *pui16Buffer, uint8_t *pui8Buffer, uint32_t ui32Count)
{
    for (uint32_t i = 0; i < ui32Count; i++)
    {
        pui8Buffer[i*2] = (uint8_t)(pui16Buffer[i] >> 8);     // MSB first
        pui8Buffer[i*2 + 1] = (uint8_t)(pui16Buffer[i]);      // LSB second
    }
}

// DMA-based SSI send function without interrupts (using polling)
void BSP_SSI_Send_16_DMA(const uint16_t *pui16Buffer, uint32_t ui32Count)
{
    static uint8_t pui8StaticBuffer[MAX_STATIC_BUFFER_SIZE];
    // OS_ERR err;
    
    // Calculate total number of bytes to transfer
    uint32_t ui32TotalBytes = ui32Count * 2;
    
    // Check if buffer is large enough
    if (ui32TotalBytes > MAX_STATIC_BUFFER_SIZE)
    {
        // Fall back to version with dynamic allocation
        BSP_SSI_Send_16_DMA(pui16Buffer, ui32Count);
        return;
    }
    
    // Convert 16-bit values to 8-bit format
    ConvertBuffer16To8(pui16Buffer, pui8StaticBuffer, ui32Count);
    
    // Process the buffer in chunks
    uint32_t ui32BytesRemaining = ui32TotalBytes;
    uint32_t ui32BufferOffset = 0;
    
    while (ui32BytesRemaining > 0)
    {
        // Calculate size of this transfer
        uint32_t ui32TransferSize = (ui32BytesRemaining < UDMA_SSI_BUFFER_SIZE) ? 
                                   ui32BytesRemaining : UDMA_SSI_BUFFER_SIZE;
        
        // Configure the DMA channel
        uDMAChannelControlSet(UDMA_CH13_SSI2TX | UDMA_PRI_SELECT,
                             UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE |
                             UDMA_ARB_4);
        
        // Set up the transfer
        uDMAChannelTransferSet(UDMA_CH13_SSI2TX | UDMA_PRI_SELECT,
                              UDMA_MODE_BASIC,
                              &pui8StaticBuffer[ui32BufferOffset],
                              (void *)(SSI2_BASE + SSI_O_DR),
                              ui32TransferSize);
        
        // Enable the DMA channel
        uDMAChannelEnable(UDMA_CH13_SSI2TX);
        
        // Poll for completion - check if DMA is done
        while (uDMAChannelIsEnabled(UDMA_CH13_SSI2TX))
        {
            // Optional: Allow other tasks to run during polling
            // OSTimeDly(1, OS_OPT_TIME_DLY, &err);
        }
        
        // Update counters for next chunk
        ui32BytesRemaining -= ui32TransferSize;
        ui32BufferOffset += ui32TransferSize;
    }
    
    // Wait for the SSI to finish transmitting
    while (SSIBusy(SSI2_BASE)) {}
}