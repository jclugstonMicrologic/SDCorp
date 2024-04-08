/** C Source ******************************************************************
*
* NAME      RadioMachine.c
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

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "stdio.h"

#include "rtcHi.h"

#include "SciBinaryMachine.h"
#include "SciAsciiMachine.h"

#include "gpioHi.h"
#include "LcdFd.h"
#include "RadioMachine.h"

#include "sysTimers.h"

/** Local Constants and Types *************************************************/


typedef enum
{
    RADIO_INIT_STATE =0,
   
    RADIO_LAST_STATE
      
}RadioStatesTypeEnum;


typedef struct
{
    RadioStatesTypeEnum machState;
    RadioStatesTypeEnum prevMachState;
        
    UINT32 timer;
    UINT32 waitTimer;    
          
}radio_t;



/** Local Variable Declarations ***********************************************/
radio_t RadioData;

char RadioSerialTxBuffer[32]; /* transmit buffer */   

/** Local Function Prototypes *************************************************/
void Radio_ProcessCommands(int cmd,char *pCmd);
void Radio_SendString(char *pBuf);

/** Functions *****************************************************************/


/*
*|----------------------------------------------------------------------------
*|  Routine: Radio_MachineInit
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
BOOL Radio_MachineInit(void)
{
    if( !SciAsciiReceiverInit(SCI_RADIO_COM, SCI_RADIO_BAUD_RATE, NULL_PTR, Radio_ProcessCommands) )
    {
        //!!!
        return FALSE;
    }         
     
    SciAsciiStartReceiver();
    
    memset( &RadioData, 0x00, sizeof(RadioData) );

    return TRUE;
}


/*
*|----------------------------------------------------------------------------
*|  Routine: Radio_Notify_Timer_Callback
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
static void Radio_Notify_Timer_Callback (void * pvParameter)
{ 

}

/*
*|----------------------------------------------------------------------------
*|  Routine: Radio_StartPeriodicNotify
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void Radio_StartPeriodicNotify(void)
{
    #define TIMER_PERIOD      1000          /**< Timer period (msec) */
    /* Start timer for LED1 blinking */
    TimerHandle_t notify_timer_handle; 
    notify_timer_handle =xTimerCreate( "RadioNotify", TIMER_PERIOD, pdTRUE, NULL, Radio_Notify_Timer_Callback);
    xTimerStart(notify_timer_handle, 0);
}


/*
*|----------------------------------------------------------------------------
*|  Routine: Radio_SendString
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void Radio_SendString(char *pBuf)
{
    SciTxPacket(SCI_RADIO_COM, strlen(pBuf), pBuf);      
}


/*
*|----------------------------------------------------------------------------
*|  Routine: Radio_ProcessCommands
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void Radio_ProcessCommands
(
    int nbrbytes,
    char *pRxBuf  /* pointer to the receive buffer */      
)
{   
    int j=0, k=0;
    char abyte =0x1b;
    char cmdArr[32];
    while(j<nbrbytes)
    {
        //abyte =*pRxBuf;
        
        if( abyte ==0x1b )
        {
            abyte =*pRxBuf ++;
            j++;
            while(abyte !=0x1b)
            {
                cmdArr[k++] =abyte;
                abyte =*pRxBuf ++;
                if( j++ >nbrbytes)
                    break;
            }
            
            if( strstr(cmdArr,"HBattery") )
            {
                cmdArr[15]  =0x00;
                Lcd_SendString(LINE1, &cmdArr[1]);
            }
            if( strstr(cmdArr,"PWM Level") )
            {
                cmdArr[18]  =0x00;
                Lcd_SendString(LINE2, &cmdArr[3]);            
            }
            if( strstr(cmdArr,"PA") )
            {
                LED2_OFF;
            }
            if( strstr(cmdArr,"PB") )
            {
                LED3_OFF;
            }       
            if( strstr(cmdArr,"PC") )
            {
                LED4_OFF;
            }            
            if( strstr(cmdArr,"PD") )
            {
                LED5_OFF;
            }       
            if( strstr(cmdArr,"PE") )
            {
                LED6_OFF;
            }       
            if( strstr(cmdArr,"PK") )
            {
                LED3_ON;
            }                   
            if( strstr(cmdArr,"PH") )
            {
                LED1_ON;
            }            
            if( strstr(cmdArr,"PM") )
            {
                LED2_ON;
            } 
            if( strstr(cmdArr,"PI") )
            {
                LED6_ON;
            }             
            
            k=0;
        }       
    }
    
    /* send a packet */
    //SciSendPacket(SCI_RADIO_COM, cmd, 5, RadioSerialTxBuffer);     
}

/*
*|----------------------------------------------------------------------------
*|  Routine: Radio_StateProcess
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void Radio_StateProcess
(
    radio_t *pStructInfo,
    RadioStatesTypeEnum nextState
)
{
    pStructInfo->prevMachState =pStructInfo->machState;
    pStructInfo->machState =nextState;
    
    pStructInfo->timer =xTaskGetTickCount();
}

/*
*|----------------------------------------------------------------------------
*|  Routine: Radio_Machine
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
BOOL Radio_Machine(void)
{  
    TickType_t xTicks=xTaskGetTickCount();

#if 0
    switch( RadioData.machState )
    {
        case RADIO_INIT_STATE:           
            Ble_StateProcess(&RadioData,BLUETOOTH_WAIT_RESPONSE_STATE);
            break;
        case BLUETOOTH_WAIT_RESPONSE_STATE:
            if( (xTicks -BleData.waitTimer ) >500 )
            {                
                if( msgNbr ==1 && Sci_GetAsciiString(0, "%REBOOT") ==1 )
                {
                    msgPass =TRUE;
                    Ble_SendString("$$$");  //enter command mode
                }
                else if( msgNbr ==2 && Sci_GetAsciiString(0, "CMD>") ==1 )
                {
                    msgPass =TRUE;
                    Ble_SendString("SS,C0\r\n"); // enable device information and UART transparent service
                }
                else if( msgNbr ==3 && Sci_GetAsciiString(0, "AOK") ==1 )
                {
                    msgPass =TRUE;
                    Ble_SendString("S-,SwipeFashion\r\n"); // enable device information and UART transparent service                
                }
                else if(msgNbr ==4 && Sci_GetAsciiString(0, "CMD>") ==1 )
                {
                    msgPass =TRUE;
                    Ble_SendString("R,1\r\n"); // reboot the module                                      
                }
                else if(msgNbr ==5 && Sci_GetAsciiString(0, "%REBOOT") ==1 )
                {                
                    /* done */
                    SciAsciiSendString(SCI_PC_COM, "BLE INIT PASS\r\n");
                    msgPass =TRUE;
                    Ble_StateProcess(&BleData,BLUETOOTH_START_NOTIFY_STATE);;                  
                }
                
                if(msgPass)
                {                  
                    msgPass =FALSE;
                    msgNbr++;
                    
                    BleData.waitTimer =xTicks;    
                }
                
                if( (xTicks -BleData.waitTimer ) >5000 )
                {
                    SciAsciiSendString(SCI_PC_COM, "ERROR BLE INIT FAIL\r\n");
                    Ble_StateProcess(&BleData,BLUETOOTH_INIT_STATE);
                    
                    BleData.waitTimer =xTicks;       
                }
            }
            break;
        case BLUETOOTH_IDLE_STATE:
            //BleData.machState =BLUETOOTH_START_NOTIFY_STATE;
            break;       
        case BLUETOOTH_START_NOTIFY_STATE:
            Ble_StartPeriodicNotify();
            Ble_StateProcess(&BleData,BLUETOOTH_WAIT_FOR_CLIENT_STATE);
            break;          
        case BLUETOOTH_WAIT_FOR_CLIENT_STATE:
            /* GATT Client will subscribe to our notify characteristic */
            if( Sci_GetAsciiString(0, "%CONNECT") ==1 )
            {
                Ble_StateProcess(&BleData,BLUETOOTH_CONNECTED_STATE);
            }
            break;      
        case BLUETOOTH_CONNECTED_STATE:
            break;
    }
#endif
    
    return TRUE;
}
