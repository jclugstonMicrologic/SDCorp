/** C Source ******************************************************************
*
* NAME      switchFd.c
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



/*
*****************************************************************************
 L O C A L   I N C L U D E   F I L E S
*****************************************************************************
*/
#include "switchFd.h"
#include "FreeRTOS.h"
#include "task.h"
#include "gpioHi.h"
#include "Timer.h"

/*
*****************************************************************************
 L O C A L    T Y P E    D E F I N I T I O N S
*****************************************************************************
*/

/*
*****************************************************************************
 L O C A L    M A C R O S
*****************************************************************************
*/

/*
*****************************************************************************
 P U B L I C   D A T A
*****************************************************************************
*/
SWITCH_DATA SwitchData[MAX_NUM_SWITCHES];

/*
*****************************************************************************
 P R I V A T E   G L O B A L   D A T A
*****************************************************************************
*/


/*
*****************************************************************************
 P R I V A T E   F U N C T I O N   P R O T O T Y P E S
*****************************************************************************
*/

#define SWITCH_ASSERTED(switchId_)  GetSwitchStatus (switchId_)

/*
*****************************************************************************
 P R O C E D U R E S
*****************************************************************************
*/


/*
*|----------------------------------------------------------------------------
*|  Module: switch
*|  Routine:  InitSwitch
*|  Description:
*|   Initialises associated software and hardware.  It sets up the callback
*|   functions that may be invoked too.
*|----------------------------------------------------------------------------
*/
void InitSwitch
(
   UINT8 switchId_,
   void *callBackPtr1_,
   void *callBackPtr2_,
   void *callBackPtr3_,
   void *callBackPtr4_
)
{
   /*
   ***********************
    L O C A L   D A T A
   ***********************
   */

   /*
   *************************
    E R R O R   C H E C K S
   *************************
   */


   /*
   *************************
    C O D E
   *************************
   */


} /* InitSwitch()  */


/*
*|----------------------------------------------------------------------------
*|  Module: switch
*|  Routine:  SwitchMachine
*|  Description:
*|    The state machine that detects the condition of the toggle switches.
*|----------------------------------------------------------------------------
*/
void SwitchMachine
(
   UINT8 switchId
)
{
   /*
   ***********************
    L O C A L   D A T A
   ***********************
   */
   int id;

   /*
   *************************
    E R R O R   C H E C K S
   *************************
   */


   /*
   *************************
    C O D E
   *************************
   */   
   TickType_t xTicks=xTaskGetTickCount();
 
   id = switchId;
   
   switch( SwitchData[id].machineState )
   {
         case SWITCH_IDLE_STATE:
//            if (!SWITCH_ASSERTED(id) )
//            {
            SwitchData[id].machineState = NORMAL_NEGATED_STATE;
//            }
            break;
         case NORMAL_NEGATED_STATE:
            /* switch is negated */
            if( SWITCH_ASSERTED(id) )
            {
               /* switch looks asserted, start debounce counter */
//               SwitchData[id].bounce = 0;
               SwitchData[id].bounce =xTicks;
               SwitchData[id].machineState = DEBOUNCE_ASSERTING_STATE;
            }
            break;
         case DEBOUNCE_ASSERTING_STATE:
            /* switch may be changing to asserted state, wait for debounce */
            if( !SWITCH_ASSERTED(id) )
            {
               /* switch not asserted after all */
               SwitchData[id].machineState = NORMAL_NEGATED_STATE;
            }
            else if( (xTicks -SwitchData[id].bounce) >BOUNCE_COUNTS )
            {
               /* debounced, switch is asserted */
//               SwitchData[id].start = 0;
               SwitchData[id].bounce =xTicks;
               SwitchData[id].machineState = ASSERTED_STATE;
            }
            break;
         case ASSERTED_STATE:
            /* switch has just been asserted, measure how long it is asserted */
            if( !SWITCH_ASSERTED(id) )
            {
               SwitchData[id].bounce = 0;
               /* switch looks like its being negated */
               SwitchData[id].machineState = DEBOUNCE_NEGATING_STATE;
            }
            else if( (xTicks - SwitchData[id].bounce) >BOUNCE_COUNTS )
            {
               /* switch just detected as being held asserted */
               /* invoke callback */
               if( SwitchData[id].callBackPtr3 != NULL_PTR )
               {
                  SwitchData[id].callBackPtr3();
               }

               SwitchData[id].start =xTicks;
               SwitchData[id].assertCounts =0;
               SwitchData[id].machineState = HELD_ASSERTED_STATE;
            }
            break;
         case DEBOUNCE_NEGATING_STATE:
            /* switch may be negating */
            if( SWITCH_ASSERTED(id) )
            {
               /* bounce detected */
               SwitchData[id].start =0;// += SwitchData[id].bounce;

               SwitchData[id].machineState = ASSERTED_STATE;
            }
            else if( ++SwitchData[id].bounce > BOUNCE_COUNTS )
            {
               /* switch went to negated state */
               /* invoke callback */
               if( SwitchData[id].callBackPtr1 != NULL_PTR )
               {
                  SwitchData[id].callBackPtr1();
               }

               SwitchData[id].machineState = NORMAL_NEGATED_STATE;
            }
            break;
         case HELD_ASSERTED_STATE:
            /* switch is being held in asserted */
            if( !SWITCH_ASSERTED(id) )
            {
               /* switch looks like its negating */
               SwitchData[id].bounce = 0;
               SwitchData[id].machineState = DEBOUNCE_RELEASING_HELD_STATE;
            }
//            else if( ++SwitchData[id].start % REPEAT_COUNTS == 0)
            else if( (xTicks-SwitchData[id].start) >250 )
            {
               /* invoke callback periodically since switch held asserted */
               if( SwitchData[id].callBackPtr2 != NULL_PTR )                 
               {
                  SwitchData[id].callBackPtr2();
               }
               SwitchData[id].start =xTicks;
               
               if( ++SwitchData[id].assertCounts >HELD_ASSERT_COUNTS )
                  SwitchData[id].machineState =HELD_ASSERTED_NEW_RATE_STATE;
            }
            break;
         case HELD_ASSERTED_NEW_RATE_STATE:
            /* switch is being held in asserted */
            if( !SWITCH_ASSERTED(id) )
            {
               /* switch looks like its negating */
               SwitchData[id].bounce = 0;
               SwitchData[id].machineState = DEBOUNCE_RELEASING_HELD_STATE;
            }
            //else if( ++SwitchData[id].start % REPEAT_NEW_RATE_COUNTS == 0)
            else if( (xTicks-SwitchData[id].start) >20 )              
            {
               /* invoke callback periodically since switch held asserted */
               if( SwitchData[id].callBackPtr2 != NULL_PTR )
               {
                  // maybe add a new callback ( *callBackPtr5 )
                  SwitchData[id].callBackPtr2();
                  
                  SwitchData[id].start =xTicks;
               }
            }
            break;
         case DEBOUNCE_RELEASING_HELD_STATE:
            /* switch may be negating */
            if( SWITCH_ASSERTED(id) )
            {
               /* bounce detected */
               if( SwitchData[id].assertCounts >=HELD_ASSERT_COUNTS )
                  SwitchData[id].machineState =HELD_ASSERTED_NEW_RATE_STATE;
               else
                  SwitchData[id].machineState =HELD_ASSERTED_STATE;

               SwitchData[id].start += SwitchData[id].bounce;
            }
            else if( ++SwitchData[id].bounce > BOUNCE_COUNTS )
            {
               /* switch went to negated state */
               /* invoke callback for 'held' state being exited */
               if( SwitchData[id].callBackPtr4 != NULL_PTR )
               {
                  SwitchData[id].callBackPtr4();
               }

               SwitchData[id].machineState = NORMAL_NEGATED_STATE;
            }
            break;
         default:
            break;
      }

} /* end SwitchMachine()  */


/*
*|----------------------------------------------------------------------------
*|  Module: switchFd
*|  Routine:  GetSwitchStatus
*|  Description: Assert high/Negate low
*|
*|----------------------------------------------------------------------------
*/
BOOL GetSwitchStatus
(
   UINT8 switchId
)
{
   UINT32 switchMask;

   switchMask  =(GPIO_ReadInputDataBit(KEYPAD1_PORT, KEYPAD1_PIN)<<0);   
   switchMask |=(GPIO_ReadInputDataBit(KEYPAD2_PORT, KEYPAD2_PIN)<<1);   
   switchMask |=(GPIO_ReadInputDataBit(KEYPAD3_PORT, KEYPAD3_PIN)<<2);   
   switchMask |=(GPIO_ReadInputDataBit(KEYPAD4_PORT, KEYPAD4_PIN)<<3);   
   switchMask |=(GPIO_ReadInputDataBit(KEYPAD5_PORT, KEYPAD5_PIN)<<4);   
   switchMask |=(GPIO_ReadInputDataBit(KEYPAD6_PORT, KEYPAD6_PIN)<<5);   
   switchMask |=(GPIO_ReadInputDataBit(KEYPAD7_PORT, KEYPAD7_PIN)<<6);   
   
   //switchMask =(switchMask <<switchId);
   
   if( (switchMask & (0x01<<switchId) ) ==(0x01<<switchId) )
      return FALSE;
   else 
      return TRUE;     
}


/* switchFd.c */










