#include "bsp_utils.h"

// ------------BSP_Delay1ms------------ 
// Simple delay function which delays about n
// milliseconds.
// Inputs: n  number of 1 msec to wait
// Outputs: none
void BSP_Delay_ms(uint32_t ms){
  SysCtlDelay((SysCtlClockGet() / 3 / 1000) * ms);
}