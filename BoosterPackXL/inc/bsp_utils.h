#include "includes.h"
#include "driverlib/sysctl.h"

// ------------BSP_Delay1ms------------
// Simple delay function which delays about n
// milliseconds.
// Inputs: n  number of 1 msec to wait
// Outputs: none
void BSP_Delay_ms(uint32_t ms);