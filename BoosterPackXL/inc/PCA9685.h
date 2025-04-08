#ifndef PCA9685_H
#define PCA9685_H

#include "includes.h"
#include "driverlib/debug.h"
#include "driverlib/i2c.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "bsp_i2c.h"

void PCA9685_Init();

#endif