#ifndef BSP_JOYSTICK_H
#define BSP_JOYSTICK_H

#include "includes.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"

void BSP_Joystick_Init(void);
void BSP_Joystick_Input(uint32_t *x, uint32_t *y);
void BSP_Joystick_Select(uint32_t *isPressed);

#endif