#include "includes.h"
#include "driverlib/sysctl.h"
#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "bsp_ssi.h"
#include "st7735.h"

#define PART_TM4C1294NCPDT


int main(void)
{
    uint32_t g_ui32SysClock;

    //
    // Run from the PLL at 120 MHz.
    // Note: SYSCTL_CFG_VCO_240 is a new setting provided in TivaWare 2.2.x and
    // later to better reflect the actual VCO speed due to SYSCTL#22.
    //
    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
        SYSCTL_OSC_MAIN |
        SYSCTL_USE_PLL |
        SYSCTL_CFG_VCO_240), 120000000);

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    BSP_UART_Init(g_ui32SysClock);
    UARTprintf("UART Init Successful...\n  ");

    UARTprintf("GPIO Init...\n  ");
    BSP_GPIO_Init();
    UARTprintf("GPIO Init Successful...\n  ");
    
    UARTprintf("SSI Init...\n  ");
    BSP_SSI_Init();
    UARTprintf("SSI Init Successful...\n  ");

    

    UARTprintf("ST7735 Init...\n  ");
    ST7735_Init();
    
    // Screen_HX8353E(
    //     GPIO_PORTH_BASE, 
    //     GPIO_PIN_3, 
    //     GPIO_PORTL_BASE, 
    //     GPIO_PIN_3, 
    //     GPIO_PORTN_BASE, 
    //     GPIO_PIN_2, 
    //     GPIO_PORTF_BASE, 
    //     GPIO_PIN_2);
    // begin();
    UARTprintf("ST7735 Init Successful...\n  ");

    // uint8_t pui32DataTx[4];
    // pui32DataTx[0] = 'Q';
    // pui32DataTx[1] = 'S';
    // pui32DataTx[2] = 'S';
    // pui32DataTx[3] = 'I';

    // BSP_SSI_Send(pui32DataTx, 4);

    uint32_t counter = 0;
    while(1)
    {
        SysCtlDelay(100);
        //UARTprintf("Loop #%d...\n  ", counter);
        counter++;
    }
}