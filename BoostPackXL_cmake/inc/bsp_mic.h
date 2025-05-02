#ifndef BSP_MIC_H
#define BSP_MIC_H

#include "includes.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"

void BSP_Mic_Init(void);
void BSP_Mic_Read(uint32_t *signal);

#endif