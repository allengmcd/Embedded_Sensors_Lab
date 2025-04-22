#include "bsp_systick_handler.h"

static void (*user_tick_callback)(void) = NULL;

void SysTick_Handler(void) {
    // Handle μC/OS-II tick
    OSTimeTick();
}

void BSP_SysTickInit(void) {
    // Initialize SysTick timer for 1ms interrupts
    // (Implementation specific to TM4C1294XL)
    // ...
}

uint32_t BSP_GetTickCount(void) {
    return OSTimeGet();  // Get OS tick count
}

void BSP_DelayMS(uint32_t ms) {
    OSTimeDlyHMSM(0, 0, 0, ms);
}

void BSP_RegisterTickCallback(void (*callback)(void)) {
    user_tick_callback = callback;
}