#ifndef BSP_UART_H
#define BSP_UART_H

#include "includes.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "uartstdio.h"

void BSP_UART_Init(uint32_t g_ui32SysClock);
void BSP_UART_Send(const uint8_t *pui8Buffer, uint32_t ui32Count);


#endif