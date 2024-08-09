/** H Header  ******************************************************************
*
* NAME      timer.h
*
* SUMMARY   
*
* TARGET    
*
* TOOLS     IAR Embedded worbench for ARM v7.4
*
* REVISION LOG
*
*******************************************************************************
* Copyright (c) 2024, MICROLOGIC
* Calgary, Alberta, Canada, www.webpage.ca
*******************************************************************************/


#ifndef SYS_TIMER_H
#define SYS_TIMER_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "timers.h"

BOOL TimerSetupUs( void );
void TimerDelayUs( UINT32 delayUL );
TimerHandle_t TimerCreate( int32_t timeout, void (*pCallBack)() );
TimerHandle_t TimerCreateOneshot( int32_t timeout, void (*pCallBack)() );

BaseType_t TimerStop(TimerHandle_t timerHandle);
#endif



