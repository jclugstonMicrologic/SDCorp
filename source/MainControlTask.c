/** C Source ******************************************************************
*
* NAME      MainControlTask.c
*
* SUMMARY   
*
* TARGET    
*
* TOOLS     IAR Embedded worbench for ARM v8.20.2
*
* REVISION LOG
*
*******************************************************************************
* Copyright (c) 2024, MICROLOGIC
* Calgary, Alberta, Canada, www.micrologic.ab.ca
*******************************************************************************/


/** Include Files *************************************************************/
#include "MainControlTask.h"
#include "FreeRTOS.h"
#include "timers.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "wdtHi.h"
#include "gpioHi.h"

#include "SwitchFd.h"
#include "KeypadFd.h"
#include "LcdFd.h"

#include "sysTimers.h"

#include "SciAsciiMachine.h"

void MainControlTask(void * pvParameters);

/** Functions *****************************************************************/


/*
*|----------------------------------------------------------------------------
*|  Routine: MainControlTaskInit
*|  Description:
*|  Arguments:
*|  Retval:
*|----------------------------------------------------------------------------
*/
BOOL MainControlTaskInit( void )
{
    BaseType_t xReturn;
    
    /* spawn the Main Control task */
    xReturn =xTaskCreate( MainControlTask, 
                          "Main Control Task", 
                          1024, //(configMINIMAL_STACK_SIZE+128),
                          NULL, 
                          (tskIDLE_PRIORITY + 2UL), 
                          ( TaskHandle_t * ) NULL);
    
    if(xReturn !=pdPASS)
    {
        //!!!
        return FALSE;
    } 
    else
        return TRUE;    
}


/*
*|----------------------------------------------------------------------------
*|  Routine: Led_Toggle_Timer_Callback
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
static void Led_Toggle_Timer_Callback (void * pvParameter)
{          
    LED1_TOGGLE;
}


/*
*|----------------------------------------------------------------------------
*|  Routine: LedFlash_StartPeriodicToggle
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void LedFlash_StartPeriodicToggle(void)
{
    #define TIMER_PERIOD      200          /**< Timer period (msec) */  

    TimerHandle_t timer_handle; 
    timer_handle =TimerCreate(TIMER_PERIOD, Led_Toggle_Timer_Callback);
    
    //xTimerStop(timer_handle, 0);
}


static void Led_Off_Timer_Callback (void * pvParameter)
{
    LED1_OFF;LED2_OFF;LED3_OFF;LED4_OFF;LED5_OFF;
}

/*
*|----------------------------------------------------------------------------
*|  Routine: MainControlTask
*|  Description:
*|  Arguments:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void MainControlTask(void * pvParameters)
{
    TickType_t xNextWakeTime;
    xNextWakeTime = xTaskGetTickCount();
    
#define TERMINAL_ENABLED
#ifdef TERMINAL_ENABLED
    UINT32 loopCnt =0;
    char aStr[32];

    SciAsciiReceiverInit(SCI_PC_COM, SCI_PC_BAUD_RATE, NULL_PTR, NULL_PTR);
    //TickType_t delayTime = xTaskGetTickCount();    
   
    /* !!! test !!! */
    strcpy(aStr, "TCE v");
    strcat(aStr, TCE_FW_VERSION);
    strcat(aStr, __DATE__);
    strcat(aStr, "\r\n\r\n");
     
    SciAsciiSendString(SCI_PC_COM, aStr);       
#endif    
    LED1_ON; LED2_ON; LED3_ON; LED4_ON; LED5_ON;
    TimerCreateOneshot(1000, Led_Off_Timer_Callback); 
     
    //LedFlash_StartPeriodicToggle();
    
    //Lcd_SendString(LINE1, "LCD");
    //Lcd_SendString(LINE2, "TEST");
    //Lcd_SendString(LINE3, "LINE3");
    //Lcd_SendString(LINE4, "LINE4");
    
    for( ;; )
    {      
        KickWdt();         
              
    #ifdef TERMINAL_ENABLED        
        if( (++loopCnt %1000) ==0 )
        {
            memset(&aStr, 0x00,sizeof(aStr));
            sprintf(aStr, "count: %d\r\n", loopCnt);
            SciAsciiSendString(SCI_PC_COM, aStr);
        }                      
    #endif
        
        /* this delay allows lower priorty tasks to run */
        //vTaskDelay(1);
                       
        for(int j=0; j<MAX_NUM_SWITCHES; j++)
        { 
            // debounce the switch input
            SwitchMachine(j);
        }        
        
        KeypadMachine(KeyActionRequest);
        
        /* place this task in the blocked state until it is time to run again */
        vTaskDelayUntil( &xNextWakeTime, 1 );        
    }
}







