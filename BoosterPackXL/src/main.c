#include "includes.h"
#include "driverlib/sysctl.h"
#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "bsp_ssi.h"
#include "st7735.h"

#define PART_TM4C1294NCPDT


int main(void)
{
    BSP_UART_Init();
    UARTprintf("UART Init Successful...\n  ");

    UARTprintf("GPIO Init...\n  ");
    BSP_GPIO_Init();
    UARTprintf("GPIO Init Successful...\n  ");
    
    UARTprintf("SSI Init...\n  ");
    BSP_SSI_Init();
    UARTprintf("SSI Init Successful...\n  ");

    UARTprintf("ST7735 Init...\n  ");
    ST7735_Init();
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