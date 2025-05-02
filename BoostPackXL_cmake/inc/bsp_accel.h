#ifndef BSP_ACCEL_H
#define BSP_ACCEL_H

#include "includes.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"

void BSP_Accel_Init(void);
void BSP_Accel_Read(uint32_t *x, uint32_t *y, uint32_t *z);

#endif