#define PART_TM4C123GH6PM
#define gcc 1

#include  <ucos_ii.h>
// #include  "app_cfg.h"
// #include  <cpu_core.h>
// #include  <os.h>
// #include 	<lib_mem.h>

#include <stdbool.h>
#include <stdint.h>

#include "BSP.h"

static  OS_STK       AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
static  OS_STK       Task1Stk[APP_CFG_TASK_START_STK_SIZE];
static  OS_STK       Task2Stk[APP_CFG_TASK_START_STK_SIZE];

static  void  AppTaskCreate         (void);
static  void  AppTaskStart          (void       *p_arg);
static  void  Task1          (char       *data);
static  void  Task2          (char       *data);
static  void  Task3          (char       *data);

void HWInit()
{

}

int main(void)
{
  CPU_IntDis();
  OSInit();
	HWInit();

  OSTaskCreateExt((void (*)(void *)) AppTaskStart,           /* Create the start task                                */
                    (void           *) 0,
                    (OS_STK         *)&AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_START_PRIO,
                    (INT16U          ) APP_CFG_TASK_START_PRIO,
                    (OS_STK         *)&AppTaskStartStk[0],
                    (INT32U          ) APP_CFG_TASK_START_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

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
         										
}

static  void  Task1 (char *data)
{
	INT8U counter = 0;
	while(1)
	{
		counter++;
		OSTimeDlyHMSM(0, 0, 1, 0); /* Wait 1 second */
	}
}

static  void  Task2 (char *data)
{
	INT8U counter = 0;
	while(1)
	{
		counter++;
		OSTimeDlyHMSM(0, 0, 1, 0); /* Wait 1 second */
	}
}
