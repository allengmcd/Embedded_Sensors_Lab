#include "includes.h"
#include "driverlib/sysctl.h"
#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "bsp_ssi.h"
#include "st7735.h"
#include "game_of_life.h"
#include "grlib/grlib.h"

#define PART_TM4C1294NCPDT

static  OS_STK       AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
static  OS_STK       Task1Stk[APP_CFG_TASK_START_STK_SIZE];
static  OS_STK       Task2Stk[APP_CFG_TASK_START_STK_SIZE];

static  void  AppTaskCreate         (void);
static  void  AppTaskStart          (void       *p_arg);
static  void  Task1          (char       *data);
static  void  Task2          (char       *data);

void CPU_IntDis(void);
void CPU_Init(void);
void Mem_Init(void);



int main(void)
{
    uint32_t g_ui32SysClock;

    //
    // Run from the PLL at 120 MHz.
    // Note: SYSCTL_CFG_VCO_240 is a new setting provided in TivaWare 2.2.x and
    // later to better reflect the actual VCO speed due to SYSCTL#22.
    //
    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
        SYSCTL_OSC_MAIN |
        SYSCTL_USE_PLL |
        SYSCTL_CFG_VCO_240), 160000000);

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    BSP_UART_Init(g_ui32SysClock);
    UARTprintf("UART Init Successful...\n  ");

    UARTprintf("GPIO Init...\n  ");
    BSP_GPIO_Init();
    UARTprintf("GPIO Init Successful...\n  ");
    
    UARTprintf("SSI Init...\n  ");
    BSP_SSI_Init();
    UARTprintf("SSI Init Successful...\n  ");

    UARTprintf("SSI DMA Init...\n  ");
    BSP_SSI_Init_DMA();
    UARTprintf("SSI DMA Init Successful...\n  ");
    

    UARTprintf("ST7735 Init...\n  ");
    BSP_LCD_Init();
    UARTprintf("ST7735 Init Successful...\n  ");

    
    UARTprintf("PCA9685 Init...\n  ");
    PCA9685_Init();
    UARTprintf("PCA9685 Init Successful...\n  ");


    UARTprintf("Joystick Init...\n  ");
    BSP_Joystick_Init();
    UARTprintf("Joystick Init Successful...\n  ");

    Init_grlib();
    // BSP_Test_grlib();

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
                         										
}

static  void  Task1 (char *data)
{
  
    UARTprintf("Starting Task1...\n");
	while(1)
	{
        loop_grlib();
        // nextGeneration();
		//OSTimeDlyHMSM(0, 0, 0, 50); /* Wait 1 second */
	}
}

static  void  Task2 (char *data)
{
    UARTprintf("Starting Task2...\n");
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 1, 0); /* Wait 1 second */
	}
}