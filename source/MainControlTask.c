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
* Copyright (c) 2020, MICROLOGIC
* Calgary, Alberta, Canada, www.micrologic.ab.ca
*******************************************************************************/


/** Include Files *************************************************************/
#include "MainControlTask.h"
#include "PowerManagement.h"
#include "FreeRTOS.h"
#include "timers.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "wdtHi.h"
#include "gpioHi.h"

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
*|  Routine: Solenoid_Toggle_Timer_Callback
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
static void Solenoid_Toggle_Timer_Callback (void * pvParameter)
{        
    static UINT8 solenoidState =0;
    int j =0;
    
    if( solenoidState ==0 )
    {
        for(j=1; j<8; j++)
        {
            OpenValve(j);          
            TimerDelayUs(1000);
        }        
        
        for(j=1; j<5; j++)
        {
            OpenReliefValve(j);
        }
        
        SciAsciiSendString(SCI_PC_COM, "OPEN\r\n");
        
        solenoidState =1;
    }
    else
    {
        for(j=1; j<8; j++)
        {
            CloseValve(j);          
            TimerDelayUs(1000);
        }   
        for(j=1; j<5; j++)
        {
            CloseReliefValve(j);
        }        
        
        SciAsciiSendString(SCI_PC_COM, "CLOSE\r\n");
        
        solenoidState =0;      
    }
}

/*
*|----------------------------------------------------------------------------
*|  Routine: Solenoid_StartPeriodicToggle
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void Solenoid_StartPeriodicToggle(void)
{
    #define TIMER_PERIOD      2000          /**< Timer period (msec) */
    /* Start timer for LED1 blinking */
    TimerHandle_t read_timer_handle; 
    read_timer_handle =xTimerCreate( "TdrRead", TIMER_PERIOD, pdTRUE, NULL, Solenoid_Toggle_Timer_Callback);
    xTimerStart(read_timer_handle, 0);
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

#ifdef DEV_BOARD
UINT8 valveState =0;
UINT8 debounceCnt =0;
#endif

  //  TickType_t delayTime = xTaskGetTickCount();    
   
    /* !!! test !!! */
    strcpy(aStr, "TCE v");
    strcat(aStr, TEC_FW_VERSION);
    strcat(aStr, __DATE__);
    strcat(aStr, "\r\n\r\n");
     
    SciAsciiSendString(SCI_PC_COM, aStr);
       
#endif    
   
  //  Solenoid_StartPeriodicToggle();
   
    /* get our board id now */
    GpioGetBoardId();
    
    for( ;; )
    {      
        KickWdt();         
              
   //     AdcMeasureReadings();

#ifdef TERMINAL_ENABLED        
        if( (++loopCnt %100) ==0 )
        {
        }
                
    #ifdef DEV_BOARD
        if( valveState == 0)
        {
            debounceCnt =0;
            
            while( !GPIO_ReadInputDataBit(USER_BTN_PORT, USER_BTN_PIN) )
            {
                if( debounceCnt ++ >50 )
                {                
                    OpenValve(1);      
                    valveState =1;
                    break;
                }                
                TimerDelayUs(1000);
            }                    
        }
        else if( valveState == 1)
        {
            debounceCnt =0;
            
            while( GPIO_ReadInputDataBit(USER_BTN_PORT, USER_BTN_PIN) )
            {
                if( debounceCnt ++ >50 )
                {                
                    valveState =2;
                    break;
                }                
                TimerDelayUs(1000);
            }
        }
        else if( valveState == 2)
        {
            debounceCnt =0;
           
            while( !GPIO_ReadInputDataBit(USER_BTN_PORT, USER_BTN_PIN) )
            {
                if( debounceCnt ++ >50 )
                {
                    CloseValve(1); 
                    valveState =3;
                    break;
                }
                TimerDelayUs(1000);
            }
        }                
        else if( valveState ==3)
        {
            debounceCnt =0;
            
            while( GPIO_ReadInputDataBit(USER_BTN_PORT, USER_BTN_PIN) )
            {
                if( debounceCnt ++ >50 )
                {
                    valveState =0;
                    break;
                }
                TimerDelayUs(1000);
            }
        }               
    #endif
        
#endif
        
        /* this delay allows lower priorty tasks to run */
        //vTaskDelay(1);
         
        if( (loopCnt %1000 ) ==0 )
        {
          SciAsciiSendString(SCI_PC_COM, "BLADDER OVER PRESSURE!!!\r\n");
        }
        
        /* place this task in the blocked state until it is time to run again */
        vTaskDelayUntil( &xNextWakeTime, 1 );        
    }
}







