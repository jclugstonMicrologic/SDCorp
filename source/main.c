/** C Source ******************************************************************
*
* NAME      main.c
*
* SUMMARY   
*
* TARGET    
*
* TOOLS     IAR Embedded workbench for ARM v8.20.2
*
* REVISION LOG
*
*******************************************************************************/
/*
   ***************************************************************************
   Copyright (C) 2024, Micrologic Limited.
   Any information contained in this document which was hitherto unknown by
   the recipient or in which there subsists a proprietary right must not be
   disclosed to any other person, nor used for manufacturing purposes,
   without the permission of Micrologic Limited and the acceptance of this
   document will be construed as acceptance of the foregoing.
   ***************************************************************************
*/


/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* hardware includes. */
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#include "sysTimers.h"

/* HI layer, to be moved when FD complete, here for testing */
#include "pwmHi.h"
#include "gpioHi.h"
#include "rtcHi.h"
#include "wdtHi.h"

#include "AdcFd.h"
#include "crc.h"

#include "LcdFd.h"
#include "KeypadFd.h"

#include "RadioMachine.h"
#include "MainControlTask.h"
#include "PowerManagement.h"

#define PC_ENABLED              1

BOOL TargetHardwareInit(void);
BOOL TceTasksInit(void);
void vTask(void * pvParameters);

/*-----------------------------------------------------------*/


/*
*|----------------------------------------------------------------------------
*|  Routine: vTimerCallback
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void vTimerCallback( TimerHandle_t xTimer )
{
   
#define BLUE_LED_TEST    
#ifdef RED_LED_TEST  
    RED_LED_TOGGLE;   
#else        
//    GREEN_LED_TOGGLE;   
#endif    

    TickType_t xTicks=xTaskGetTickCount();  
}


/*
*|----------------------------------------------------------------------------
*|  Routine: main
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
int main(void)
{       
    /* initialize the amc hardware */
    if( !TargetHardwareInit() )
    {
        //!!!
        return FALSE;
    }

    /* initialize the amc tasks, modules, state machines, etc */
    if( !TceTasksInit() )
    {
        //!!!
        return FALSE;      
    }

      
    /* start the scheduler */
    vTaskStartScheduler();

 
    /* if all is well, the scheduler will now be running, and the following line
    will never be reached.  If the following line does execute, then there was
    insufficient FreeRTOS heap memory available for the idle and/or timer tasks
    to be created.  See the memory management section on the FreeRTOS web site
    for more details. */
    for( ;; );
}


/*
*|----------------------------------------------------------------------------
*|  Routine: TargetHardwareInit
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
BOOL TargetHardwareInit(void)
{   
    /* setup STM32 system (clock, PLL and Flash configuration) */
    SystemInit();

    /* ensure all priority bits are assigned as preemption priority bits */
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

    /* adjust interrupt vector table by vector offset (0x4000) */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, (0x08000000 | 0x0000) ); //(0x08000000 | 0x4000) );
    
    /* check if the system has resumed from IWDG reset */
    if( RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET )
    {
        /* IWDGRST flag set */
        /* Clear reset flags */
        RCC_ClearFlag();
    }

    /* initialize uSec timer */
    TimerSetupUs();
    
    /* initialize all GPIO */
    Gpio_Init();
                
    Lcd_Init();
    
    Keypad_Init();
    /* initialize hardware crc */
    //CrcInit32();
      
    /* initialize watch dog timer, serviced in MainControlTask
       do not init if debugging, watchdog is a nuisance
    */ 
    
    /* disable IWDG if core is halted */
    DBGMCU->APB1FZR1 |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;
     
    /*!!! need to find out how to disable during sleep(don't think it's possible)
     OR allow reset, then go to sleep out of reset without enabling wdt    
    */
    WdtInit();  
   
    return TRUE;    
}


/*
*|----------------------------------------------------------------------------
*|  Routine: TceTasksInit
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
BOOL TceTasksInit(void)
{   
    if( !Radio_MachineInit() )
    {
        //!!!
        return FALSE;      
    }
                 
    /* initialize the main control task */
    MainControlTaskInit();  
     
    return TRUE;
}



/*-----------------------------------------------------------*/

