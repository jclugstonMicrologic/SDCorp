/** H header  ******************************************************************
*
* NAME      wdtiHi.h
*
* SUMMARY   
*
* TARGET    
*
* TOOLS     IAR Embedded workbench for ARM v7.4
*
* REVISION LOG
*
*******************************************************************************
* Copyright (c) 2017, TRIG
* Calgary, Alberta, Canada, www.webpage.ca
*******************************************************************************/

#ifndef WDT_HI_H
#define WDT_HI_H

#include "types.h"
#include "stm32f4xx.h"

void WdtInit(void);
void KickWdt(void);

void WWDG_Init(u8 tr,u8 wr,u32 fprer);

#endif
