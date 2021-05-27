#define PART_TM4C123GH6PM
#define gcc 1

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "BSP.h"

void HWInit()
{

}

int main(void)
{
	PWMInit();

	int count1 = 0;
	int count2 = 0;
	int count3 = 0;

	while(1)
	{
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, count2+=3 );
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, count3+=2 );
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, count1++ );
		
		//SysCtlDelay(1000);

		if(count1 > 0x0000B366)
		{
			count1 = 0;
		}
		if(count2 > 0x0000B366)
		{
			count2 = 0;
		}
		if(count3 > 0x0000B366)
		{
			count3 = 0;
		}
	}
}
