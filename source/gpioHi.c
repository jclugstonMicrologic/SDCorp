/** C Source ******************************************************************
*
* NAME      gpioHi.c
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


/** Include Files *************************************************************/

#include "gpioHi.h"

/** Functions *****************************************************************/


/*
*|----------------------------------------------------------------------------
*|  Routine: Gpio_Init
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void Gpio_Init
(
    void
)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
 
    //NVIC_InitTypeDef NVIC_InitStructure;
    //EXTI_InitTypeDef EXTI_InitStructure;
  
    /* enable the GPIO Clock */
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_GPIOA, ENABLE);
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_GPIOB, ENABLE);
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_GPIOC, ENABLE); 
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_GPIOD, ENABLE); 
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_GPIOE, ENABLE); 
    
    OLED_NEGATE_CS;
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    LED4_OFF;
    LED5_OFF;
    LED6_OFF;    
    BUZZER_OFF;
    
    OLED_ASSERT_RST;
    
    /* configure the following GPIO pins as outputs */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_InitStructure.GPIO_Pin = OLED_CS_PIN;
    GPIO_Init(OLED_CS_PORT, &GPIO_InitStructure);        
        
    GPIO_InitStructure.GPIO_Pin = OLED_RST_PIN;
    GPIO_Init(OLED_RST_PORT, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = LED1_PIN;
    GPIO_Init(LED1_PORT, &GPIO_InitStructure);        
    GPIO_InitStructure.GPIO_Pin = LED2_PIN;
    GPIO_Init(LED2_PORT, &GPIO_InitStructure);    
    GPIO_InitStructure.GPIO_Pin = LED3_PIN;
    GPIO_Init(LED3_PORT, &GPIO_InitStructure);    
    GPIO_InitStructure.GPIO_Pin = LED4_PIN;
    GPIO_Init(LED4_PORT, &GPIO_InitStructure);    
    GPIO_InitStructure.GPIO_Pin = LED5_PIN;
    GPIO_Init(LED5_PORT, &GPIO_InitStructure);    
    GPIO_InitStructure.GPIO_Pin = LED6_PIN;
    GPIO_Init(LED6_PORT, &GPIO_InitStructure);        
    
    GPIO_InitStructure.GPIO_Pin = PIEZO_BZR_PIN;
    GPIO_Init(PIEZO_BZR_PORT, &GPIO_InitStructure);   
    
    GPIO_InitStructure.GPIO_Pin = KEYPAD1_PIN;
    GPIO_Init(KEYPAD1_PORT, &GPIO_InitStructure);        
    GPIO_InitStructure.GPIO_Pin = KEYPAD2_PIN;
    GPIO_Init(KEYPAD2_PORT, &GPIO_InitStructure);    
    
    GPIO_SetBits(KEYPAD1_PORT, KEYPAD1_PIN);
    GPIO_SetBits(KEYPAD2_PORT, KEYPAD2_PIN);
      
    /* configure the following GPIO pins as inputs */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_UP;//GPIO_PuPd_UP;// GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;  
    
    GPIO_InitStructure.GPIO_Pin = KEYPAD3_PIN;
    GPIO_Init(KEYPAD3_PORT, &GPIO_InitStructure);    
    GPIO_InitStructure.GPIO_Pin = KEYPAD4_PIN;
    GPIO_Init(KEYPAD4_PORT, &GPIO_InitStructure);    
    GPIO_InitStructure.GPIO_Pin = KEYPAD5_PIN;
    GPIO_Init(KEYPAD5_PORT, &GPIO_InitStructure);    
    GPIO_InitStructure.GPIO_Pin = KEYPAD6_PIN;
    GPIO_Init(KEYPAD6_PORT, &GPIO_InitStructure);    
    GPIO_InitStructure.GPIO_Pin = KEYPAD7_PIN;
    GPIO_Init(KEYPAD7_PORT, &GPIO_InitStructure);        
    
#if 0
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12;
    GPIO_Init(GPIOC, &GPIO_InitStructure);    
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);       
   
    /* enable and set EXTI line interrupt */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource6);
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode =EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);    
   
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);                
      
    /* configure ADC Channels as analog input */    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AN_ADC_CNTRL;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    
    GPIO_InitStructure.GPIO_Pin =(ADC_1_PIN | ADC_2_PIN | 
                                  ADC_VSENSE1_PIN);
    GPIO_Init(ADC_PORT, &GPIO_InitStructure);		   
   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
        
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;                                 
    GPIO_Init(GPIOB, &GPIO_InitStructure);		           
#endif    

}

/*
*|----------------------------------------------------------------------------
*|  Routine: GpioSetOutput
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void GpioSetOutput(void)
{
#if 0  
    GPIO_InitTypeDef  GPIO_InitStructure;    
    
    /* configure the following GPIO pins as outputs */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    
    //GPIO_InitStructure.GPIO_Pin =BRD_ID_BIT1_PIN;
    //GPIO_Init(BRD_ID_BIT1_PORT, &GPIO_InitStructure);
#endif
}

void GpioGetBoardId(void)
{
#if 0
    /* must make global, as reading this port only available out of reset,
       it then becomes an output for ble reset
    */   
    BoardStatus.status.b.id =BOARD_ID;
    BoardStatus.status.b.id =0;
#endif
}
/*
*|----------------------------------------------------------------------------
*|  Routine: GpioSleep
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void GpioSleep
(
    void
)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
       
    /* configure the following GPIO pins as inputs */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz; 
   
    //GPIO_InitStructure.GPIO_Pin =GREEN_LED_PIN;
    //GPIO_Init(GREEN_LED_PORT, &GPIO_InitStructure);                   

    //GPIO_InitStructure.GPIO_Pin = RED_LED_PIN;
    //GPIO_Init(RED_LED_PORT, &GPIO_InitStructure);              
       
    //GPIO_InitStructure.GPIO_Pin =(ADC_1_PIN | ADC_2_PIN );                                
    //GPIO_Init(ADC_PORT, &GPIO_InitStructure);		   

    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6;                                 
    GPIO_Init(GPIOA, &GPIO_InitStructure);         
    
    /* USART TX/RX pins */
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;                                 
    GPIO_Init(GPIOD, &GPIO_InitStructure);                 
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9;                                 
    GPIO_Init(GPIOD, &GPIO_InitStructure);      
    
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12;                                 
    GPIO_Init(GPIOC, &GPIO_InitStructure);                 
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;                                 
    GPIO_Init(GPIOD, &GPIO_InitStructure);          
        
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;                                 
    GPIO_Init(GPIOA, &GPIO_InitStructure);          
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9;                                 
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10;                                 
    GPIO_Init(GPIOC, &GPIO_InitStructure);     
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_11;
    GPIO_Init(GPIOC, &GPIO_InitStructure);      
    
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_11;                                 
    GPIO_Init(GPIOB, &GPIO_InitStructure);                     
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10;                                 
    GPIO_Init(GPIOB, &GPIO_InitStructure);     
    
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15;                                 
    GPIO_Init(GPIOE, &GPIO_InitStructure);     
    
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15;                                 
    GPIO_Init(GPIOB, &GPIO_InitStructure);    
    
    /* SPI pins */
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12;                                 
    GPIO_Init(GPIOE, &GPIO_InitStructure);      
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13;
    GPIO_Init(GPIOE, &GPIO_InitStructure);          
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_14;                                 
    GPIO_Init(GPIOE, &GPIO_InitStructure);      
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);      
    
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5;                                 
    GPIO_Init(GPIOB, &GPIO_InitStructure);      
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6;
    GPIO_Init(GPIOB, &GPIO_InitStructure);          
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7;                                 
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
    

    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AN; //GPIO_MODE_ANALOG;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_All;
    
    GPIO_Init(GPIOD, &GPIO_InitStructure); 
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
        
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_All;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
    
    /* ensure JTAG pins are not changed, can be changed for production use */
    GPIO_InitStructure.GPIO_Pin =0xffe7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);     
    
    GPIO_InitStructure.GPIO_Pin =0x1fff;
    GPIO_Init(GPIOA, &GPIO_InitStructure);         
    
    ADC_Cmd( ADC1, DISABLE );
}


/*
*|----------------------------------------------------------------------------
*|  Routine: EXTI9_5_IRQHandler
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetFlagStatus(EXTI_Line6)==SET)
    {
        EXTI_ClearFlag(EXTI_Line6);            
    }    
}


/*
*|----------------------------------------------------------------------------
*|  Routine: EXTI15_10_IRQHandler
*|  Description:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetFlagStatus(EXTI_Line13)==SET)
    {
        EXTI_ClearFlag(EXTI_Line13);            
    }  
}

