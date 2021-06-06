#define PART_TM4C123GH6PM
#define gcc 1

#include  <ucos_ii.h>
#include <stdbool.h>
#include <stdint.h>

#include "BSP.h"
#include "BSP2.h"
//#include "tm4c123gh6pm.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "uartstdioTemp.h"

static  OS_STK       AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
static  OS_STK       Task1Stk[APP_CFG_TASK_START_STK_SIZE];
static  OS_STK       Task2Stk[APP_CFG_TASK_START_STK_SIZE];
static  OS_STK       Task3Stk[APP_CFG_TASK_START_STK_SIZE];
static  OS_STK       Task4Stk[APP_CFG_TASK_START_STK_SIZE];

static  void  AppTaskCreate         (void);
static  void  AppTaskStart          (void       *p_arg);
static  void  Task1          (char       *data);
static  void  Task2          (char       *data);
static  void  Task3          (char       *data);
static  void  Task4          (char       *data);

void CPU_IntDis(void);
void CPU_Init(void);
void Mem_Init(void);

uint32_t JoyX, JoyY, JoyZ;
uint32_t AccelX, AccelY, AccelZ;
uint32_t Mic;


void HWInit()
{
  
  //
  // Initialize the UART and write initial status.
  //
  BSP_UART_Init();
  //UARTprintf("Timer->ADC->uDMA demo!\n\n");
  //UARTprintf("ui32AverageResult1\tui32AverageResult2\tTotal Samples\n");
  BSP_Joystick_Init();
  BSP_Accelerometer_Init();
  BSP_Microphone_Init();
  BSP_LCD_Init();
  BSP_LCD_FillScreen(BSP_LCD_Color565(0, 0, 0));
}

int main(void)
{
  CPU_IntDis();
	HWInit();
  UARTprintf("Hardware initialization completed without errors...\n");
  UARTprintf("Starting uC/OS-II initilization...\n");
  OSInit();
  UARTprintf("uC/OS-II initilization completed without errors...\n");
  UARTprintf("Startup Task...\n");

  OSTaskCreateExt((void (*)(void *)) AppTaskStart,           /* Create the start task                                */
                    (void           *) 0,
                    (OS_STK         *)&AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_START_PRIO,
                    (INT16U          ) APP_CFG_TASK_START_PRIO,
                    (OS_STK         *)&AppTaskStartStk[0],
                    (INT32U          ) APP_CFG_TASK_START_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

  UARTprintf("Startup Task complete...\n");
  OSStart(); 

	while(1)
	{
		
	}
}



static  void  AppTaskStart (void *p_arg)
{
    (void)p_arg;                                                /* See Note #1                                              */

		//AckMbox = OSMboxCreate((void *)0); /* Create 2 message mailboxes */
		//TxMbox = OSMboxCreate((void *)0);
   (void)&p_arg;

    //BSP_Init();                                                 /* Initialize BSP functions                             */

    //cpu_clk_freq = BSP_SysClkFreqGet();                         /* Determine SysTick reference freq.                    */
    //cnts         = cpu_clk_freq                                 /* Determine nbr SysTick increments                     */
    //             / (CPU_INT32U)OS_TICKS_PER_SEC;

    OS_CPU_SysTickInit(0x00003E80);
    CPU_Init();                                                 /* Initialize the uC/CPU services                       */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity                                   */
#endif

    Mem_Init();


		OSTimeDlyHMSM(0, 0, 1, 0);   

		AppTaskCreate();                                            /* Creates all the necessary application tasks.         */

    OSTimeDlyHMSM(0, 0, 0, 200);			
}


/*
*********************************************************************************************************
*                                         AppTaskCreate()
*
* Description :  Create the application tasks.
*
* Argument(s) :  none.
*
* Return(s)   :  none.
*
* Caller(s)   :  AppTaskStart()
*
* Note(s)     :  none.
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
OSTaskCreate((void (*)(void *)) Task1,           /* Create the second task                                */
                    (void           *) 0,							// argument
                    (OS_STK         *)&Task1Stk[APP_CFG_TASK_START_STK_SIZE - 1],
                    (INT8U           ) 5 );  						// Task Priority
                

OSTaskCreate((void (*)(void *)) Task2,           /* Create the second task                                */
                    (void           *) 0,							// argument
                    (OS_STK         *)&Task2Stk[APP_CFG_TASK_START_STK_SIZE - 1],
                    (INT8U           ) 6 );  						// Task Priority
                

OSTaskCreate((void (*)(void *)) Task3,           /* Create the second task                                */
                    (void           *) 0,							// argument
                    (OS_STK         *)&Task3Stk[APP_CFG_TASK_START_STK_SIZE - 1],
                    (INT8U           ) 7 );  						// Task Priority
                

OSTaskCreate((void (*)(void *)) Task4,           /* Create the second task                                */
                    (void           *) 0,							// argument
                    (OS_STK         *)&Task4Stk[APP_CFG_TASK_START_STK_SIZE - 1],
                    (INT8U           ) 8 );  						// Task Priority
         										
}

static  void  Task1 (char *data)
{
  
  UARTprintf("Starting Task1...\n");
	while(1)
	{
    //BSP_Joystick_Input(&JoyX, &JoyY, &current);
    BSP_Joystick_Input(&JoyX, &JoyY, &JoyZ);
		OSTimeDlyHMSM(0, 0, 0, 50); /* Wait 1 second */
	}
}

static  void  Task2 (char *data)
{
  UARTprintf("Starting Task2...\n");
	while(1)
	{
    // print joystick status
    BSP_LCD_DrawString(0, 3, "JoyX=    ", BSP_LCD_Color565(255, 255, 255));
    BSP_LCD_SetCursor(5, 3);
    BSP_LCD_OutUDec((uint32_t)JoyX, BSP_LCD_Color565(255, 0, 255));
    BSP_LCD_DrawString(0, 4, "JoyY=    ", BSP_LCD_Color565(255, 255, 255));
    BSP_LCD_SetCursor(5, 4);
    BSP_LCD_OutUDec((uint32_t)JoyY, BSP_LCD_Color565(255, 0, 255));
    BSP_LCD_DrawString(0, 5, "Micr=    ", BSP_LCD_Color565(255, 255, 255));
    BSP_LCD_SetCursor(5, 5);
    BSP_LCD_OutUDec((uint32_t)Mic, BSP_LCD_Color565(255, 0, 255));
    BSP_LCD_DrawString(0, 6, "AccX=    ", BSP_LCD_Color565(255, 255, 255));
    BSP_LCD_SetCursor(5, 6);
    BSP_LCD_OutUDec((uint32_t)AccelX, BSP_LCD_Color565(255, 0, 255));
    BSP_LCD_DrawString(0, 7, "AccY=    ", BSP_LCD_Color565(255, 255, 255));
    BSP_LCD_SetCursor(5, 7);
    BSP_LCD_OutUDec((uint32_t)AccelY, BSP_LCD_Color565(255, 0, 255));
    BSP_LCD_DrawString(0, 8, "AccZ=    ", BSP_LCD_Color565(255, 255, 255));
    BSP_LCD_SetCursor(5, 8);
    BSP_LCD_OutUDec((uint32_t)AccelZ, BSP_LCD_Color565(255, 0, 255));

    UARTprintf("JoyX, JoyY= %d, %d\r", (uint32_t)JoyX, (uint32_t)JoyY);
		OSTimeDlyHMSM(0, 0, 1, 0); /* Wait 1 second */
	}
}

static  void  Task3 (char *data)
{
  
  UARTprintf("Starting Task3...\n");
	while(1)
	{
    BSP_Accelerometer_Input(&AccelX, &AccelY, &AccelZ);
		OSTimeDlyHMSM(0, 0, 0, 50); /* Wait 1 second */
	}
}

static  void  Task4 (char *data)
{
  
  UARTprintf("Starting Task4...\n");
	while(1)
	{
    //BSP_Joystick_Input(&JoyX, &JoyY, &current);
    BSP_Microphone_Input(&Mic);
		OSTimeDlyHMSM(0, 0, 0, 50); /* Wait 1 second */
	}
}
