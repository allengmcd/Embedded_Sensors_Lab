#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include "includes.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

void BSP_GPIO_Init(void);
void BSP_GPIO_Write(uint32_t ui32Port, uint8_t ui8Pins, uint8_t ui8Val);

#endif