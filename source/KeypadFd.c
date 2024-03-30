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
   SwitchData[KEY_FWD_JOG].callBackPtr4 =NULL_PTR;   
   
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
   SwitchData[KEY_REV_JOG].callBackPtr4 =NULL_PTR;      


   for(j =0; j<MAX_NUM_SWITCHES; j++)
   {
      SwitchData[j].bounce = 0;
      SwitchData[j].start  = 0;
      SwitchData[j].machineState = SWITCH_IDLE_STATE;
   }

   KeypadInfo.currentState =KEYPAD_MAIN_STATE;

   KeyActionRequest =KEY_NONE;
              
}/* end InitKeypad */

#define BUZZER_ON_TIME (150)
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
            case KEY_PWR_UP:
                break;
            case KEY_STOP:             
                break;
            case KEY_FWD_LOW:
                break;
            case KEY_FWD_MED:
                break;
            case KEY_FWD_FAST:
                break;    
            case KEY_FWD_JOG:
                break;       
            case KEY_REV_LOW:
                break;
            case KEY_REV_MED:
                break;
            case KEY_REV_FAST:
                break;                   
            case KEY_REV_JOG:
                break;
         }
         break;
#if 0         
      case KEYPAD_RUN_STATE:
         KeyRunState(key_);
         break;
      case KEYPAD_LOAD_STATE:
         KeyLoadState(key_);
         break;
      case KEYPAD_SETUP_STATE:
         KeySetupState(key_);
         break;
      case KEYPAD_RUN_AUTO_DIR_STATE:
         KeyRunAutoDirState(key_);
         break;         
      case KEYPAD_RUN_AUTO_RATE_STATE:
         KeyRunAutoRateState(key_);
         break;
      case KEYPAD_RUN_MANUAL_STATE:
         KeyRunManualState(key_);                         
         break;         
      case KEYPAD_LOAD_INDEX_STATE:
         KeyLoadIndexState(key_);
         break;
      case KEYPAD_SETUP_CANNISTER_STATE:
         KeySetupCannisterState(key_);
         break;
      case KEYPAD_SETUP_BATTERY_STATE:
         KeySetupBatteryState(key_);
         break;         
      case KEYPAD_TEST_DIR_STATE:
         KeyTestDirState(key_);        
         break; 
      case KEYPAD_TEST_RATE_STATE:
         KeyTestRateState(key_);
         break;           
      case KEYPAD_DROPPER_STATE:         
         KeyDropperState(key_);
         break;
#endif         
   }
   
   if( KeyActionRequest !=KEY_NONE )
   {
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
} // end

void KeyStop
(
   void
)
{
   KeyActionRequest =KEY_STOP;
} // end

void KeyFwdLow
(
   void
)
{
   KeyActionRequest =KEY_FWD_LOW;
} // end

void KeyFwdMed
(
   void
)
{
   KeyActionRequest =KEY_FWD_MED;
} // end

void KeyFwdFast
(
   void
)
{
   KeyActionRequest =KEY_FWD_FAST;
} // end

void KeyFwdJog
(
   void
)
{
   KeyActionRequest =KEY_FWD_JOG;
} // end


void KeyRevLow
(
   void
)
{
   KeyActionRequest =KEY_REV_LOW;
} // end

void KeyRevMed
(
   void
)
{
   KeyActionRequest =KEY_REV_MED;
} // end

void KeyRevFast
(
   void
)
{
   KeyActionRequest =KEY_REV_FAST;
} // end

void KeyRevJog
(
   void
)
{
   KeyActionRequest =KEY_REV_JOG;
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
*|  Routine: KeyLoadState
*|  Description:
*|----------------------------------------------------------------------------
*/
void KeyLoadState(int key_)
{
   switch( key_ )        
   {
      case KEY_LEFT:
//         if( MotorControl.indexing ==1 )              
//         if( BallCount.load !=0 )    
         if( MotorControl.indexing ==1 || BallCount.load !=0 )  
         {
            // loading has been started, going back is no longer an option
            // key is now reverse           
            Motor.load.direction =MOTOR_REV;
            
            if( Motor.load.start ==0 )
            {
               // start motor           
               Motor.load.start =1;
               MotorControl.indexing =1; 
            }
            else
            {
               // stop motor
               Motor.load.start =0;                 
            }                     
         }
         else
            MainDisplay(); // go back              
         break;
      case KEY_UP:
      case KEY_HELD_UP:              
         if( (MotorControl.indexing ==1 || BallCount.load !=0) && Motor.load.start ==0 )  
         {
            // loading has been started, going back is no longer an option
            if( Motor.load.start ==0 ) // only change menu if motor stopped                 
            {
               KeypadInfo.currentState =KEYPAD_LOAD_INDEX_STATE;
               LcdMachineState =LCD_LOAD_INDEX_SCREEN;                                
                
               if( BallCount.load ==MAX_BALL_COUNT)                
                  MotorControl.indexState =INDEX_COMPLETE_STATE;
            }           
         }
         else
         {
            // adjust speed if available to do so
//            if( Motor.load.start )
            {
               if( ++Motor.load.rate >MAX_MOTOR_RATE )
               {
                  Motor.load.rate =MAX_MOTOR_RATE;
               }
            }
         }                  
         break;
      case KEY_DOWN:
      case KEY_HELD_DOWN:
         // adjust speed if available to do so
//         if( Motor.load.start )
         {        
            if( --Motor.load.rate ==0 )
            {
               Motor.load.rate =MIN_MOTOR_RATE;
            }
         }
         break;
      case KEY_ENTER:
         if( !Motor.load.start &&
              MotorControl.indexState !=INDEX_COMPLETE_STATE            
           )
         {
            // start motor      
            Motor.load.direction =MOTOR_FWD;
            
            Motor.load.start =true;
            MotorControl.indexing =1; 
            
            MotorControl.indexState =INDEX_IDLE_STATE;            
         }
         else
         {
            // stop motor
            Motor.load.start =false;                 
         }         
         break; 
      case KEY_UPDATE_SCREEN:
         // update LCD values
         LcdMachineState =LCD_LOAD_UPDATE_SCREEN;
         break;                 
   }                
}

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


/*
*|----------------------------------------------------------------------------
*|  Module: KeypadFd Module
*|  Routine: KeyRunAutoDirState
*|  Description:
*|----------------------------------------------------------------------------
*/
void KeyRunAutoDirState(int key_)
{
   switch( key_ )        
   {
      case KEY_LEFT:
         // go back
         KeypadInfo.currentState =KEYPAD_RUN_STATE;
         LcdMachineState =LCD_RUN_SCREEN;               
         break;
      case KEY_UP:               
      case KEY_HELD_UP:              
      case KEY_DOWN:
      case KEY_HELD_DOWN:              
         if( ++Motor.injection.direction >1 )
         {
            Motor.injection.direction =MOTOR_FWD;                 
         }
         break;
      case KEY_ENTER:             
         KeypadInfo.currentState =KEYPAD_RUN_AUTO_RATE_STATE;
         LcdMachineState =LCD_RUN_AUTO_RATE_SCREEN;                   
         break;   
      case KEY_UPDATE_SCREEN:
         // update LCD values
         LcdMachineState =LCD_RUN_AUTO_UPDATE_DIR_SCREEN;
         break;        
   }
} // end KeyRunAutoDirState(int key_)


/*
*|----------------------------------------------------------------------------
*|  Module: KeypadFd Module
*|  Routine: KeyRunAutoRateState
*|  Description:
*|----------------------------------------------------------------------------
*/
void KeyRunAutoRateState(int key_)
{
   switch( key_ )        
   {
      case KEY_LEFT:
         // stop motor, go back
         Motor.injection.start =0;
                            
         // go back to direction adjust
         KeypadInfo.currentState =KEYPAD_RUN_AUTO_DIR_STATE;
         LcdMachineState =LCD_RUN_AUTO_DIR_SCREEN;
               
         break;
      case KEY_UP:               
      case KEY_HELD_UP:              
         if( ++Motor.injection.rate >MAX_MOTOR_RATE )
         {
            Motor.injection.rate =MAX_MOTOR_RATE;                 
         }
         break;
      case KEY_DOWN:
      case KEY_HELD_DOWN:              
         if( --Motor.injection.rate ==0 )
         {
            Motor.injection.rate =MIN_MOTOR_RATE;
         }
         break;
      case KEY_ENTER:
         // start motor
         if( Motor.injection.start ==0 )
            Motor.injection.start =1;
         else
         {
            Motor.injection.start =0;                 
         }
         break;   
      case KEY_UPDATE_SCREEN:
         // update LCD values
         LcdMachineState =LCD_RUN_AUTO_UPDATE_RATE_SCREEN;
         break;        
   } 
} // end KeyRunAutoRateState(int key_)


/*
*|----------------------------------------------------------------------------
*|  Module: KeypadFd Module
*|  Routine: KeyRunManualState
*|  Description:
*|----------------------------------------------------------------------------
*/
void KeyRunManualState(int key_)
{
   switch( key_ )        
   {
       case KEY_LEFT:
          // stop motor, go back
          Motor.manual.start =0;
          KeypadInfo.currentState =KEYPAD_RUN_STATE;
          LcdMachineState =LCD_RUN_SCREEN;                
          break;
      case KEY_UP:
      case KEY_HELD_UP:              
      case KEY_DOWN:
      case KEY_HELD_DOWN:                             
         if( ++Motor.manual.direction >1 )
         {
            Motor.manual.direction =MOTOR_FWD;                 
         }
         break;
      case KEY_ENTER:
         // manual mode      
         Motor.manual.rate =100;
               
         if( Motor.manual.start ==0 )              
            Motor.manual.start =1;
         else
            Motor.manual.start =0; 
         break;
      case KEY_UPDATE_SCREEN:
         // update LCD values
         LcdMachineState =LCD_RUN_MANUAL_UPDATE_SCREEN;
         break;                       
   }        
}


/*
*|----------------------------------------------------------------------------
*|  Module: KeypadFd Module
*|  Routine: KeyLoadIndexState
*|  Description:
*|----------------------------------------------------------------------------
*/
void KeyLoadIndexState(int key_)
{
   switch( key_ )        
   {
      case KEY_LEFT:
         if( MotorControl.indexState ==INDEX_IDLE_STATE )
         {
            // stop motor, go back a screen
            Motor.manual.start =0;
            KeypadInfo.currentState =KEYPAD_LOAD_STATE;
            LcdMachineState =LCD_LOAD_SCREEN;                                             
         }
         else
         {
            // reverse
            Motor.index.direction =MOTOR_REV;           
            
            if( Motor.index.start ==0 )
            {
               // start motor           
               Motor.index.start =1;            
            }
            else
            {
               Motor.index.start =0;
            }
         }
         break;
      case KEY_UP:
      case KEY_HELD_UP:   
         if( MotorControl.indexState ==INDEX_COMPLETE_STATE )
         {
            if( Motor.index.start ==0 && BallCount.index !=0 )
            {
               // start motor           
               Motor.index.start =1;
            
               Motor.index.direction =MOTOR_FWD;            
            }  
            else
            {
               Motor.index.start =0;                 
            }
         }
         else
         {
            if( ++Motor.index.rate >MAX_MOTOR_RATE )
            {
               Motor.index.rate =MAX_MOTOR_RATE;                 
            }
         }
         break;
      case KEY_DOWN:
      case KEY_HELD_DOWN:               
         if( MotorControl.indexState ==INDEX_COMPLETE_STATE )
         {
            MainDisplay();        
         }
         else
         {
            if( --Motor.index.rate ==0 )
            {
               Motor.index.rate =MIN_MOTOR_RATE;                
            }
         }
         break;
      case KEY_ENTER:
         if( MotorControl.indexState ==INDEX_COMPLETE_STATE )
         {
            // still allow motor to stop, just no start
            Motor.index.start =0;
            break;
         }
         
         if( Motor.index.start ==0 )
         {
            // start motor           
            Motor.index.start =1;
            
            Motor.index.direction =MOTOR_FWD;
              
            MotorControl.indexState =INDEX_START_STATE;
         }
         else
         {
            // stop motor
            Motor.index.start =0;                 
            
            MotorControl.indexState =INDEX_STOP_STATE;            
         }
         break;       
      case KEY_UPDATE_SCREEN:
         // update LCD values
         LcdMachineState =LCD_LOAD_INDEX_UPDATE_SCREEN;
         break;                                
   }                
}


/*
*|----------------------------------------------------------------------------
*|  Module: KeypadFd Module
*|  Routine: KeySetupCannisterState
*|  Description:
*|----------------------------------------------------------------------------
*/
void KeySetupCannisterState(int key_)
{     
   switch( key_ )        
   {
      case KEY_LEFT:
         // go back
         MainDisplay();         
         break;
#ifndef SIX_BALLS_PER_REV    
      case KEY_UP:        
         if( MotorControl.ballsPerRev ==8 )
         {
            MotorControl.ballsPerRev =4;
            MotorControl.vanesPerBall =8;
         }
         else
         {
            MotorControl.ballsPerRev =8;
            MotorControl.vanesPerBall =4;
         }                 
         
         SaveToInternalMemory( APP_START_ADDRESS );
         break;
      case KEY_DOWN:
         if( MotorControl.ballsPerRev ==8 )
         {
            MotorControl.ballsPerRev =4;
            MotorControl.vanesPerBall =8;
         }
         else
         {
            MotorControl.ballsPerRev =8;
            MotorControl.vanesPerBall =4;
         }                          
         
         SaveToInternalMemory( APP_START_ADDRESS );                  
         break;
#endif         
      case KEY_ENTER:
         // go back
         KeypadInfo.currentState =KEYPAD_SETUP_BATTERY_STATE;
         LcdMachineState =LCD_SETUP_BATTERY_SCREEN;
         break;  
      case KEY_UPDATE_SCREEN:
         // update LCD values
         LcdMachineState =LCD_SETUP_CANNISTER_UPDATE_SCREEN;
         break;                          
   }                
}



/*
*|----------------------------------------------------------------------------
*|  Module: KeypadFd Module
*|  Routine: KeySetupBatteryState
*|  Description:
*|----------------------------------------------------------------------------
*/
void KeySetupBatteryState(int key_)
{     
#ifdef OLD  
   switch( key_ )        
   {
      case KEY_LEFT:
         Motor.test.start =0;  
         Motor.test.rate =100;
         KeypadInfo.currentState =KEYPAD_TEST_DIR_STATE;
         LcdMachineState =LCD_SETUP_TEST_DIR_SCREEN;               
         break;
      case KEY_UP:
      case KEY_DOWN:  
      case KEY_HELD_UP:
      case KEY_HELD_DOWN:
         if( !Motor.test.start )
         {
            // only switch direction when stopped
            if( ++Motor.test.direction >1 )
            {
               Motor.test.direction =MOTOR_FWD;                 
            }
         }
         break;
      case KEY_ENTER:
         Motor.test.rate =250;
         // start motor
         if( Motor.test.start ==0 )
            Motor.test.start =1;
         else
         {
            Motor.test.start =0;                 
         }              
         break;  
      case KEY_UPDATE_SCREEN:
         // update LCD values
         LcdMachineState =LCD_SETUP_BATTERY_UPDATE_SCREEN;
         break;               
   }                
#else
   switch( key_ )        
   {
      case KEY_LEFT:         
      case KEY_UP:
//      case KEY_HELD_LEFT:        
//      case KEY_HELD_UP:
         if( !Motor.test.start )
         {
            // only switch direction when stopped
            if( ++Motor.test.direction >1 )
            {
               Motor.test.direction =MOTOR_FWD;                 
            }
         }        
         break;
      case KEY_DOWN:
         Motor.test.rate =250;
         // start motor
         if( Motor.test.start ==0 )
            Motor.test.start =1;
         else
         {
            Motor.test.start =0;                 
         }              
         break;  
      case KEY_ENTER:       
         Motor.test.start =0;  
         Motor.test.rate =100;
         KeypadInfo.currentState =KEYPAD_TEST_DIR_STATE;
         LcdMachineState =LCD_SETUP_TEST_DIR_SCREEN;               
         break;         
      case KEY_UPDATE_SCREEN:
         // update LCD values
         LcdMachineState =LCD_SETUP_BATTERY_UPDATE_SCREEN;
         break;               
   }                   
#endif   
}


/*
*|----------------------------------------------------------------------------
*|  Module: KeypadFd Module
*|  Routine: KeyTestDirState
*|  Description:
*|----------------------------------------------------------------------------
*/
void KeyTestDirState(int key_)
{     
   switch( key_ )        
   {
            case KEY_LEFT:
               // go back
               MainDisplay();
               break;
            case KEY_UP:
            case KEY_DOWN:  
            case KEY_HELD_UP:
            case KEY_HELD_DOWN:
               if( ++Motor.test.direction >1 )
               {
                  Motor.test.direction =MOTOR_FWD;                 
               }
               break;
            case KEY_ENTER:   
               KeypadInfo.currentState =KEYPAD_TEST_RATE_STATE;
               LcdMachineState =LCD_SETUP_TEST_RATE_SCREEN;              
               break;      
             case KEY_UPDATE_SCREEN:
               // update LCD values
               LcdMachineState =LCD_SETUP_TEST_UPDATE_DIR_SCREEN;
               break;                
   }                
}


/*
*|----------------------------------------------------------------------------
*|  Module: KeypadFd Module
*|  Routine: KeyTestRateState
*|  Description:
*|----------------------------------------------------------------------------
*/
void KeyTestRateState(int key_)
{     
   switch( key_ )        
   {
            case KEY_LEFT:
               // stop motor, go back
               Motor.test.start =0;              
               
               // go back to mode where direction can be changed
               Motor.test.start =0;  
               KeypadInfo.currentState =KEYPAD_TEST_DIR_STATE;
               LcdMachineState =LCD_SETUP_TEST_DIR_SCREEN;                              
               break;
            case KEY_UP:               
            case KEY_HELD_UP:              
               if( ++Motor.test.rate >MAX_MOTOR_RATE )
               {
                  Motor.test.rate =MAX_MOTOR_RATE;                 
               }
               break;
            case KEY_DOWN:
            case KEY_HELD_DOWN:              
               if( --Motor.test.rate ==0 )
               {
                  Motor.test.rate =MIN_MOTOR_RATE;
               }
               break;
      case KEY_ENTER: 
         if( Motor.test.start ==0 )
            Motor.test.start =1;
         else
         {
            Motor.test.start =0;                 
         }                  
         break;      
      case KEY_UPDATE_SCREEN:
         // update LCD values
         LcdMachineState =LCD_SETUP_TEST_UPDATE_RATE_SCREEN;
         break;                
   }                
}


/*
*|----------------------------------------------------------------------------
*|  Module: KeypadFd Module
*|  Routine: KeyDropperState
*|  Description:
*|----------------------------------------------------------------------------
*/
void KeyDropperState(int key_)
{
   switch( key_ )
   {
      case KEY_LEFT:
         // go back
         LcdRow =0;
         MainDisplay();
         break;
      case KEY_UP:
         // JOG+
         Motor.jog.rate =100;

         Motor.jog.direction =MOTOR_FWD;
                  
         if( Motor.jog.start ==0 )              
            Motor.jog.start =1;
         else
            Motor.jog.start =0;                 
         break;
      case KEY_DOWN:
         //JOG-
         Motor.jog.rate =100;

         Motor.jog.direction =MOTOR_REV;
                  
         if( Motor.jog.start ==0 )              
            Motor.jog.start =1;
         else
            Motor.jog.start =0;                 
         break;
      case KEY_ENTER:
         // DROP
         // manual mode      
         Motor.manual.rate =100;
                  
         if( Motor.manual.start ==0 )              
            Motor.manual.start =1;
         else
            Motor.manual.start =0;         
         break;               
      case KEY_UPDATE_SCREEN:
         // update LCD values
         LcdMachineState =LCD_DROPPER_UPDATE_SCREEN;
         break;         
   }  
}
#endif

// end KeypadFd.c




