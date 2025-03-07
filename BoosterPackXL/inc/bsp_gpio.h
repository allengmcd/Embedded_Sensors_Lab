#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include "includes.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

void BSP_GPIO_Init(void);
void BSP_GPIO_Write(const uint8_t *pui8Buffer, uint32_t ui32Count);

#endif