/** C Source ******************************************************************
*
* NAME      KeypadFd.c
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
#include <stddef.h>

#include "FreeRTOS.h"
#include "timers.h"

#include "gpioHi.h"

#include "sysTimers.h"
#include "SciAsciiMachine.h"
#include "keypadFd.h"


/*
*****************************************************************************
 L O C A L    M A C R O S
*****************************************************************************
*/
#define ROW_START (0)
#define ROW_END   (2)

#define NBR_KEYPAD_SWITCHES (16)

#define KEYPAD_UP_BIT_MASK   (0x01)
#define KEYPAD_DOWN_BIT_MASK (0x02)
#define KEYPAD_DUAL_ACK      (0x04)

#pragma segment="BOOTLOADEREND"
#define APP_START_ADDRESS ((void *)0x00020000)
/*
*****************************************************************************
 L O C A L    T Y P E    D E F I N I T I O N S
*****************************************************************************
*/
enum
{
   KEYPAD_MAIN_STATE =0,

   KEYPAD_RUN_STATE,
   KEYPAD_LOAD_STATE,
   KEYPAD_SETUP_STATE,

   KEYPAD_RUN_AUTO_DIR_STATE,
   KEYPAD_RUN_AUTO_RATE_STATE,
   
   KEYPAD_RUN_MANUAL_STATE,

   KEYPAD_LOAD_INDEX_STATE,

   KEYPAD_SETUP_CANNISTER_STATE,
   KEYPAD_SETUP_BATTERY_STATE,
     
  
   KEYPAD_TEST_DIR_STATE,
   KEYPAD_TEST_RATE_STATE,   
   
   KEYPAD_DROPPER_STATE,
   
   KEYPAD_LAST_STATE
};


typedef struct
{
   UINT8 currentState;
   UINT8 prevState;
   UINT32 waitTimer;
   
   void (*keyCallback)(void);
}KEY_PAD_INFO;

/*
*****************************************************************************
 P U B L I C   D A T A
*****************************************************************************
*/
UINT8 KeyActionRequest;

/*
*****************************************************************************
 P R I V A T E   G L O B A L   D A T A
*****************************************************************************
*/
KEY_PAD_INFO KeypadInfo;

/*
*****************************************************************************
 P R I V A T E   F U N C T I O N   P R O T O T Y P E S
*****************************************************************************
*/

void KeyRunState(int key);
void KeyLoadState(int key);
void KeySetupState(int key);
void KeyRunAutoDirState(int key);
void KeyRunAutoRateState(int key);
void KeyRunManualState(int key);                 
void KeyLoadIndexState(int key_);        
void KeySetupCannisterState(int key_);
void KeySetupBatteryState(int key_);
void KeyTestDirState(int key_);
void KeyTestRateState(int key_);

void KeyDropperState(int key_);

/*
*****************************************************************************
 P R O C E D U R E S
*****************************************************************************
*/

/*
*|----------------------------------------------------------------------------
*|  Module: Keypad_Init Module
*|  Routine: InitKeypad
*|  Description:
*|   Initialisation for this module
*|
*|   *callbackPtr1 =assert, then negate
*|   *callbackPtr2 =held asserted, periodic event
*|   *callbackPtr3 =held asserted
*|   *callbackPtr4 =held asserted, then released
*|----------------------------------------------------------------------------
*/
void Keypad_Init
(
   void
)
{
   /*
   ***********************
    L O C A L   D A T A
   ***********************
   */
   UINT8 j;

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

   // enable all the keys
   SwitchData[KEY_PWR_UP].callBackPtr1 =NULL_PTR;
   SwitchData[KEY_PWR_UP].callBackPtr2 =NULL_PTR;
   SwitchData[KEY_PWR_UP].callBackPtr3 =KeyPwrUp;
   SwitchData[KEY_PWR_UP].callBackPtr4 =NULL_PTR;

   SwitchData[KEY_STOP].callBackPtr1 =NULL_PTR;
   SwitchData[KEY_STOP].callBackPtr2 =NULL_PTR;
   SwitchData[KEY_STOP].callBackPtr3 =KeyStop;
   SwitchData[KEY_STOP].callBackPtr4 =NULL_PTR;

   SwitchData[KEY_FWD_LOW].callBackPtr1 =NULL_PTR;
   SwitchData[KEY_FWD_LOW].callBackPtr2 =NULL_PTR;
   SwitchData[KEY_FWD_LOW].callBackPtr3 =KeyFwdLow;
   SwitchData[KEY_FWD_LOW].callBackPtr4 =NULL_PTR;

   SwitchData[KEY_FWD_MED].callBackPtr1 =NULL_PTR;
   SwitchData[KEY_FWD_MED].callBackPtr2 =NULL_PTR;
   SwitchData[KEY_FWD_MED].callBackPtr3 =KeyFwdMed;
   SwitchData[KEY_FWD_MED].callBackPtr4 =NULL_PTR;

   SwitchData[KEY_FWD_FAST].callBackPtr1 =NULL_PTR;
   SwitchData[KEY_FWD_FAST].callBackPtr2 =NULL_PTR;
   SwitchData[KEY_FWD_FAST].callBackPtr3 =KeyFwdFast;
   SwitchData[KEY_FWD_FAST].callBackPtr4 =NULL_PTR;
   
   SwitchData[KEY_FWD_JOG].callBackPtr1 =NULL_PTR;
   SwitchData[KEY_FWD_JOG].callBackPtr2 =NULL_PTR;
   SwitchData[KEY_FWD_JOG].callBackPtr3 =KeyFwdJog;
   SwitchData[KEY_FWD_JOG].callBackPtr4 =KeyReleaseFwdJog;   
   
   SwitchData[KEY_REV_LOW].callBackPtr1 =NULL_PTR;
   SwitchData[KEY_REV_LOW].callBackPtr2 =NULL_PTR;
   SwitchData[KEY_REV_LOW].callBackPtr3 =KeyRevLow;
   SwitchData[KEY_REV_LOW].callBackPtr4 =NULL_PTR;

   SwitchData[KEY_REV_MED].callBackPtr1 =NULL_PTR;
   SwitchData[KEY_REV_MED].callBackPtr2 =NULL_PTR;
   SwitchData[KEY_REV_MED].callBackPtr3 =KeyRevMed;
   SwitchData[KEY_REV_MED].callBackPtr4 =NULL_PTR;

   SwitchData[KEY_REV_FAST].callBackPtr1 =NULL_PTR;
   SwitchData[KEY_REV_FAST].callBackPtr2 =NULL_PTR;
   SwitchData[KEY_REV_FAST].callBackPtr3 =KeyRevFast;
   SwitchData[KEY_REV_FAST].callBackPtr4 =NULL_PTR;
   
   SwitchData[KEY_REV_JOG].callBackPtr1 =NULL_PTR;
   SwitchData[KEY_REV_JOG].callBackPtr2 =NULL_PTR;
   SwitchData[KEY_REV_JOG].callBackPtr3 =KeyRevJog;
   SwitchData[KEY_REV_JOG].callBackPtr4 =KeyReleaseRevJog;      


   for(j =0; j<MAX_NUM_SWITCHES; j++)
   {
      SwitchData[j].bounce = 0;
      SwitchData[j].start  = 0;
      SwitchData[j].machineState = SWITCH_IDLE_STATE;
   }

   KeypadInfo.currentState =KEYPAD_MAIN_STATE;

   KeyActionRequest =KEY_NONE;
              
}/* end InitKeypad */

void SendKeyPwrupMsg(void)
{
    char radioTxBuf[]={0x01,0x7f,0x01,0x01,0x01};
    uint8_t len =sizeof(radioTxBuf);
  
    SciSendDataPacket(SCI_RADIO_COM, radioTxBuf, len);
}

void SendKeyStopMsg(void)
{
    char radioTxBuf[]={0x01,0x7f,0x01,0x03,0x10,0x00,0x00};
    uint8_t len =sizeof(radioTxBuf);
  
    SciSendDataPacket(SCI_RADIO_COM, radioTxBuf, len);
}

void SendKeyFwdLowMsg(void)
{
    char radioTxBuf[]={0x01,0x7f,0x01,0x03,0x10,0x5a,0x5a};
    uint8_t len =sizeof(radioTxBuf);
  
    SciSendDataPacket(SCI_RADIO_COM, radioTxBuf, len);
}

void SendKeyFwdMedMsg(void)
{
    char radioTxBuf[]={0x01,0x7f,0x01,0x03,0x10,0x6e,0x6e};
    uint8_t len =sizeof(radioTxBuf);
  
    SciSendDataPacket(SCI_RADIO_COM, radioTxBuf, len);
}

void SendKeyFwdFastMsg(void)
{
    char radioTxBuf[]={0x01,0x7f,0x01,0x03,0x10,0x7f,0x7f};
    uint8_t len =sizeof(radioTxBuf);
  
    SciSendDataPacket(SCI_RADIO_COM, radioTxBuf, len);
}

void SendKeyFwdJogMsg(void)
{
    char radioTxBuf[]={0x01,0x7f,0x01,0x03,0x10,0x32,0x32};
    uint8_t len =sizeof(radioTxBuf);
  
    SciSendDataPacket(SCI_RADIO_COM, radioTxBuf, len);
}


void SendKeyRevLowMsg(void)
{
    char radioTxBuf[]={0x01,0x7f,0x01,0x03,0x10,0x81,0x81};
    uint8_t len =sizeof(radioTxBuf);
  
    SciSendDataPacket(SCI_RADIO_COM, radioTxBuf, len);
}

void SendKeyRevMedMsg(void)
{
    char radioTxBuf[]={0x01,0x7f,0x01,0x03,0x10,0x92,0x92};
    uint8_t len =sizeof(radioTxBuf);
  
    SciSendDataPacket(SCI_RADIO_COM, radioTxBuf, len);
}

void SendKeyRevFastMsg(void)
{
    char radioTxBuf[]={0x01,0x7f,0x01,0x03,0x10,0xa6,0xa6};
    uint8_t len =sizeof(radioTxBuf);
  
    SciSendDataPacket(SCI_RADIO_COM, radioTxBuf, len);
}

void SendKeyRevJogMsg(void)
{
    char radioTxBuf[]={0x01,0x7f,0x01,0x03,0x10,0xce,0xce};
    uint8_t len =sizeof(radioTxBuf);
  
    SciSendDataPacket(SCI_RADIO_COM, radioTxBuf, len);
}

/*
*|----------------------------------------------------------------------------
*|  Routine: Buzzer_Toggle_Timer_Callback
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
static void Buzzer_Toggle_Timer_Callback (void * pvParameter)
{          
    BUZZER_TOGGLE;
}

/*
*|----------------------------------------------------------------------------
*|  Routine: Buzzer_StartPeriodicBuzz
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void Buzzer_StartPeriodicBuzz(void)
{
    #define BUZZER_TIMER_PERIOD      500          /**< Timer period (msec) */

    TimerHandle_t timer_handle; 
    timer_handle =TimerCreate(BUZZER_TIMER_PERIOD, Buzzer_Toggle_Timer_Callback);    
}

static void Buzzer_Off_Timer_Callback (void * pvParameter)
{
    BUZZER_OFF;
}

#define BUZZER_KEY_ON_TIME (150)
/*
*|----------------------------------------------------------------------------
*|  Module: KeypadFd Module
*|  Routine: KeypadMachine
*|  Description:
*|
*|----------------------------------------------------------------------------
*/
void KeypadMachine
(
   UINT8 key_
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
   
   switch( KeypadInfo.currentState )
   {
      case KEYPAD_MAIN_STATE:        
         switch( key_ )
         {
            case KEY_PWR_UP: //key1
                break;
            case KEY_STOP:  //key6           
                break;
            case KEY_FWD_LOW: //key2
                break;
            case KEY_FWD_MED: //key3
                break;
            case KEY_FWD_FAST: //key4
                break;    
            case KEY_FWD_JOG: //key5
                break;       
            case KEY_RELEASE_FWD_JOG: //key5
                break;        
            case KEY_REV_LOW:
            case KEY_REV_MED:
            case KEY_REV_FAST:
            case KEY_REV_JOG:
                //Buzzer_StartPeriodicBuzz();
                break;
            case KEY_RELEASE_REV_JOG: //
                break;                  
         }
         break;
#if 0         
      case KEYPAD_RUN_STATE:
         KeyRunState(key_);
         break;
#endif         
   }
   
   if( KeyActionRequest !=KEY_NONE )
   {
      /*!!!! TEST !!!!*/      
      KeypadInfo.keyCallback =SendKeyStopMsg;
      
      BUZZER_ON;
      KeypadInfo.keyCallback();      
      TimerCreateOneshot(BUZZER_KEY_ON_TIME, Buzzer_Off_Timer_Callback); 
   }
   
   KeyActionRequest =KEY_NONE;

} // end KeypadMachine()

  
   
/*
*|----------------------------------------------------------------------------
*|  Module: KeypadFd Module
*|  Routine: KeyPwrUp
*|  Description: Switch module has called this routine
*|----------------------------------------------------------------------------
*/
void KeyPwrUp
(
   void
)
{
   KeyActionRequest =KEY_PWR_UP;
   KeypadInfo.keyCallback =SendKeyPwrupMsg;
} // end

void KeyStop
(
   void
)
{
   KeyActionRequest =KEY_STOP;
   KeypadInfo.keyCallback =SendKeyStopMsg;
} // end

void KeyFwdLow
(
   void
)
{
   KeyActionRequest =KEY_FWD_LOW;
   KeypadInfo.keyCallback =SendKeyFwdLowMsg;
} // end

void KeyFwdMed
(
   void
)
{
   KeyActionRequest =KEY_FWD_MED;
   KeypadInfo.keyCallback =SendKeyFwdMedMsg;
} // end

void KeyFwdFast
(
   void
)
{
   KeyActionRequest =KEY_FWD_FAST;
   KeypadInfo.keyCallback =SendKeyFwdFastMsg;
} // end

void KeyFwdJog
(
   void
)
{
   KeyActionRequest =KEY_FWD_JOG;
   KeypadInfo.keyCallback =SendKeyFwdJogMsg;
} // end


void KeyReleaseFwdJog
(
   void
)
{
   KeyActionRequest =KEY_RELEASE_FWD_JOG;
   KeypadInfo.keyCallback =SendKeyStopMsg;
} // end


void KeyRevLow
(
   void
)
{
   KeyActionRequest =KEY_REV_LOW;
   KeypadInfo.keyCallback =SendKeyRevLowMsg;
} // end

void KeyRevMed
(
   void
)
{
   KeyActionRequest =KEY_REV_MED;
   KeypadInfo.keyCallback =SendKeyRevMedMsg;
} // end

void KeyRevFast
(
   void
)
{
   KeyActionRequest =KEY_REV_FAST;
   KeypadInfo.keyCallback =SendKeyRevFastMsg;
} // end

void KeyRevJog
(
   void
)
{
   KeyActionRequest =KEY_REV_JOG;
   KeypadInfo.keyCallback =SendKeyRevJogMsg;
} // end

void KeyReleaseRevJog
(
   void
)
{
   KeyActionRequest =KEY_RELEASE_REV_JOG;
   KeypadInfo.keyCallback =SendKeyStopMsg;
} // end
/*
*|----------------------------------------------------------------------------
*|  Module: KeypadFd Module
*|  Routine: KeyRunState
*|  Description:
*|----------------------------------------------------------------------------
*/
void KeyRunState(int key_)
{
}

#if 0
/*
*|----------------------------------------------------------------------------
*|  Module: KeypadFd Module
*|  Routine: KeySetupState
*|  Description:
*|----------------------------------------------------------------------------
*/
void KeySetupState(int key_)
{
   switch( key_ )        
   {
      case KEY_LEFT:
         // go back
         MainDisplay();         
         break;
      case KEY_UP:
      case KEY_HELD_UP:                
         if( ++BallCount.load >MAX_BALL_COUNT )
         {
            BallCount.load =MAX_BALL_COUNT;                 
         }
         
         BallCount.index =0;
         MotorControl.indexState =INDEX_COMPLETE_STATE;
         break;
      case KEY_DOWN:
      case KEY_HELD_DOWN:              
         if( --BallCount.load<0 )// ==255 )
         {
            BallCount.load =0;                
         }        
         break;
      case KEY_ENTER:
         // go to cannister screen
         KeypadInfo.currentState =KEYPAD_SETUP_CANNISTER_STATE;
         LcdMachineState =LCD_SETUP_CANNISTER_SCREEN;          
         break;    
      case KEY_UPDATE_SCREEN:       
         // update LCD values
         LcdMachineState =LCD_SETUP_UPDATE_SCREEN;         
         break;                
   }
}

#endif

// end KeypadFd.c




