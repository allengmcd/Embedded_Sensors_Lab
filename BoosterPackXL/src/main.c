#include "includes.h"
#include "bsp_uart.h"

#define PART_TM4C1294NCPDT


int main(void)
{
    BSP_UART_Init();

    BSP_UART_Send((uint8_t *)"\033[2JEnter text: ", 16);

    while(1)
    {
        
    }
}