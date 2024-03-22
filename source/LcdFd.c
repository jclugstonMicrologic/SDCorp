/** C Source ******************************************************************
*
* NAME      LcdFd.c
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
#include "FreeRTOS.h"
#include "math.h"
#include "gpioHi.h"
#include "spiHi.h"
#include "LcdFd.h"



/*
*****************************************************************************
 L O C A L    M A C R O S
*****************************************************************************
*/

/*
*****************************************************************************
 L O C A L    T Y P E    D E F I N I T I O N S
*****************************************************************************
*/

/*
*****************************************************************************
 P U B L I C   D A T A
*****************************************************************************
*/

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
void lcdCommand(unsigned char cmd);
void lcdData(unsigned char data);

void lcdDisplayOn(void);
void lcdDisplayOff(void);

void lcdSetPosition(uint8_t pos);
/*
*****************************************************************************
 P R O C E D U R E S
*****************************************************************************
*/

/*
*|----------------------------------------------------------------------------
*|  Routine: Lcd_Init
*|  Description:
*|  Arguments:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void Lcd_Init( void )
{
    //UINT8 regData =0;
    //UINT8 chipId =0;      
    //regData =12;
    
    SpiInit(LCD_SPI_PORT);    
       
    SpiDeviceInit(0);
    
    /* assert cs */
    spiStart(0);
    
    lcdCommand(0x38+3); //function set european chararacter set
    lcdCommand(0x08);   //display off
    lcdCommand(0x06);   //entry mode set increment cursor by 1 not shifting display
    lcdCommand(0x17);   //Character mode and internel power on (have to turn on internel power to get the best brightness)
    lcdCommand(0x01);   //clear display
    lcdCommand(0x02);   //return home
    lcdCommand(0x0c);   //display on
    
    /* clear display */    
    lcdCommand(0x01);
    lcdSetPosition(LINE1);         
      
    /* display OnOff */
    lcdCommand(0x08 | (DISPLAY_ON | CURSOR_ON | BLINK_ON) );
      
    /* negate cs */
    spiStop(0);    
}


/*
*|----------------------------------------------------------------------------
*|  Routine: Lcd_Send
*|  Description:
*|  Arguments:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void Lcd_SetCursor(uint8_t pos)
{
    lcdCommand(LCD_HOME_L1+pos);        
}

/*
*|----------------------------------------------------------------------------
*|  Routine: Lcd_Send
*|  Description:
*|  Arguments:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void Lcd_Send(uint8_t nbrBytes, unsigned char *pTxData )
{
    int j;
    
    for(j=0; j<nbrBytes; j++)
    {
        lcdData( *pTxData++ );
    }
}


/*
*|----------------------------------------------------------------------------
*|  Routine: Lcd_SendString
*|  Description:
*|  Arguments:
*|  Retval:
*|----------------------------------------------------------------------------
*/
void Lcd_SendString(uint8_t row, char *pBuf)
{
    uint8_t nbrBytes;
    
    nbrBytes =strlen( (char *)pBuf );
    
    Lcd_SetCursor(row);
    
    Lcd_Send(nbrBytes, (unsigned char *)pBuf);   
}

void lcdDisplayOn(void)
{
    lcdCommand(0x0c);
} 


void lcdDisplayOff(void)
{
    lcdCommand(0x08);
}

void lcdSetPosition(uint8_t pos)
{
    lcdCommand(LCD_HOME_L1+pos);
}

void lcdCommand(unsigned char cmd)
{
    SpiDeviceInit(0);
    
    /* assert cs */
    spiStart(0);
    
    SPI_TRANSFER_LCD(0x00);
    SPI_TRANSFER_LCD(cmd);
    
    /* negate cs */
    spiStop(0);    
}

void lcdData(unsigned char data)
{
    SpiDeviceInit(0);
    
    /* assert cs */
    spiStart(0);
    
    SPI_TRANSFER_LCD(0x02); 
    SPI_TRANSFER_LCD(data);
    
    /* negate cs */
    spiStop(0);    
}




