// bsp_systick_handler.h
#ifndef BSP_SYSTICK_HANDLER_H
#define BSP_SYSTICK_HANDLER_H

#include "includes.h"
// #include "lvgl.h"

void BSP_SysTickInit(void);
uint32_t BSP_GetTickCount(void);
void BSP_DelayMS(uint32_t ms);
void BSP_RegisterTickCallback(void (*callback)(void));
uint8_t writecommand_test(const uint8_t *cmd, size_t cmd_size, const uint8_t *param, size_t param_size);
#endif