#ifndef BSP_SSI_H
#define BSP_SSI_H

#include "includes.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_ssi.h"
#include "inc/hw_udma.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "uartstdio.h"


void BSP_SSI_Init(void);
void BSP_SSI_Send(const uint8_t *pui8Buffer, uint32_t ui32Count);
void BSP_SSI_Send_16(const uint16_t *pui8Buffer, uint32_t ui32Count);
void BSP_SSI_Init_DMA(void);
void BSP_SSI_Send_16_DMA(const uint16_t *pui16Buffer, uint32_t ui32Count);

#endif