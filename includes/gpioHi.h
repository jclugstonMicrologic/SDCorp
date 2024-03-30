/** H Header  ******************************************************************
*
* NAME      gpio.h
*
* SUMMARY   
*
* TARGET    
*
* TOOLS     IAR Embedded workbench for ARM v8.20.2
*
* REVISION LOG
*
*******************************************************************************
* Copyright (c) 2024, MICROLOGIC
* Calgary, Alberta, Canada, www.micrologic.ab.ca
*******************************************************************************/


#ifndef GPIO_H
#define GPIO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_gpio.h"
#include "types.h"


/************************************************************************/
#define OLED_RST_PIN       GPIO_Pin_7
#define OLED_RST_PORT      GPIOA

#define OLED_CS_PIN   GPIO_Pin_11
#define OLED_CS_PORT  GPIOB

#define PIEZO_BZR_PIN   GPIO_Pin_12
#define PIEZO_BZR_PORT  GPIOB

#define LED6_PIN   GPIO_Pin_6
#define LED6_PORT  GPIOC
#define LED5_PIN   GPIO_Pin_7
#define LED5_PORT  GPIOC
#define LED4_PIN   GPIO_Pin_8
#define LED4_PORT  GPIOC
#define LED3_PIN   GPIO_Pin_9
#define LED3_PORT  GPIOC
#define LED2_PIN   GPIO_Pin_13
#define LED2_PORT  GPIOC
#define LED1_PIN   GPIO_Pin_8
#define LED1_PORT  GPIOA


#define KEYPAD7_PIN          GPIO_Pin_3
#define KEYPAD7_PORT         GPIOC
#define KEYPAD6_PIN          GPIO_Pin_4
#define KEYPAD6_PORT         GPIOC
#define KEYPAD5_PIN          GPIO_Pin_5
#define KEYPAD5_PORT         GPIOC
#define KEYPAD4_PIN          GPIO_Pin_0
#define KEYPAD4_PORT         GPIOB
#define KEYPAD3_PIN          GPIO_Pin_1
#define KEYPAD3_PORT         GPIOB
#define KEYPAD2_PIN          GPIO_Pin_2
#define KEYPAD2_PORT         GPIOB
#define KEYPAD1_PIN          GPIO_Pin_8
#define KEYPAD1_PORT         GPIOB

/* ADC channels */
//#define ADC_1_PIN             GPIO_Pin_0
//#define ADC_1_PORT            GPIOC
//#define ADC_2_PIN             GPIO_Pin_1
//#define ADC_2_PORT            GPIOC
//#define ADC_PORT              GPIOC

/********************* MACROS **************************************/
#define OLED_ASSERT_CS  GPIO_ResetBits(OLED_CS_PORT, OLED_CS_PIN)
#define OLED_NEGATE_CS  GPIO_SetBits(OLED_CS_PORT, OLED_CS_PIN)

#define OLED_ASSERT_RST GPIO_ResetBits(OLED_RST_PORT, OLED_RST_PIN)
#define OLED_NEGATE_RST GPIO_SetBits(OLED_RST_PORT, OLED_RST_PIN)

#define LED1_ON     GPIO_SetBits(LED1_PORT, LED1_PIN)
#define LED1_OFF    GPIO_ResetBits(LED1_PORT, LED1_PIN)
#define LED1_TOGGLE GPIO_ToggleBits(LED1_PORT, LED1_PIN)

#define LED2_ON     GPIO_SetBits(LED2_PORT, LED2_PIN)
#define LED2_OFF    GPIO_ResetBits(LED2_PORT, LED2_PIN)
#define LED3_ON     GPIO_SetBits(LED3_PORT, LED3_PIN)
#define LED3_OFF    GPIO_ResetBits(LED4_PORT, LED3_PIN)
#define LED4_ON     GPIO_SetBits(LED4_PORT, LED4_PIN)
#define LED4_OFF    GPIO_ResetBits(LED4_PORT, LED4_PIN)
#define LED5_ON     GPIO_SetBits(LED5_PORT, LED5_PIN)
#define LED5_OFF    GPIO_ResetBits(LED5_PORT, LED5_PIN)
#define LED6_ON     GPIO_SetBits(LED6_PORT, LED6_PIN)
#define LED6_OFF    GPIO_ResetBits(LED6_PORT, LED6_PIN)

#define BUZZER_ON       GPIO_SetBits(PIEZO_BZR_PORT, PIEZO_BZR_PIN)
#define BUZZER_OFF      GPIO_ResetBits(PIEZO_BZR_PORT, PIEZO_BZR_PIN)
#define BUZZER_TOGGLE   GPIO_ToggleBits(PIEZO_BZR_PORT, PIEZO_BZR_PIN)
                                
typedef union
{
  struct
  {
    UINT8 id:2;         
    UINT8 fitting:1;    
    UINT8 compr:2;      
    UINT8 relief:2;
    UINT8 spare:1;
  } b;         
  
}status_t;

typedef struct
{
    status_t status;
    UINT8 valve; /* state of all solenoids/valves */
}board_status_t;

//extern UINT16 BoardId;
extern board_status_t BoardStatus;

void Gpio_Init(void);
void GpioSleep(void);

void GpioSetOutput(void);
void GpioGetBoardId(void);

#endif


