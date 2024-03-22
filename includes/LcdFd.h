/*
*|***************************************************************************
*|PROJECT:  
*|Module:   LcdFd
*|   NOTES:
*|**************************************************************************
*/
/*
Tools:
   Compiler: IAR Kickstart development environment for AT91SAM7Sxxx
   ***************************************************************************
   Copyright (C) 2001, Micrologic Limited.
   Any information contained in this document which was hitherto unknown by
   the recipient or in which there subsists a proprietary right must not be
   disclosed to any other person, nor used for manufacturing purposes,
   without the permission of Micrologic Limited and the acceptance of this
   document will be construed as acceptance of the foregoing.
   ***************************************************************************
*/

/*
   ***************************************************************************
   Copyright (C) 2024, Micrologic Limited.
   Any information contained in this document which was hitherto unknown by
   the recipient or in which there subsists a proprietary right must not be
   disclosed to any other person, nor used for manufacturing purposes,
   without the permission of Micrologic Limited and the acceptance of this
   document will be construed as acceptance of the foregoing.
   ***************************************************************************
*/
#ifndef LCD_FD_H
 #define LCD_FD_H



/*
*****************************************************************************
 P U B L I C   I N C L U D E   F I L E S
*****************************************************************************
*/
#include "types.h"

/*
*****************************************************************************
 P U B L I C   T Y P E    D E F I N I T I O N S
*****************************************************************************
*/
   
#define DISPLAY_ON 	0x04
#define DISPLAY_OFF     0xFB
#define CURSOR_ON	0x02
#define CURSOR_OFF	0xFD
#define BLINK_ON	0x01
#define BLINK_OFF	0xFE

//--- Standard definitions for LCD ---
#define LCD_HOME_L1	0x80
#define LINE1	0

//HD44780
#define LINE2	LINE1+0x40
#define LINE3	LINE1+0x14
#define	LINE4 	LINE2+0x14

/*
*****************************************************************************
 P U B L I C    M A C R O S
*****************************************************************************
*/

#define LCD_SPI_PORT    SPI_PORT2
#define SPI_TRANSFER_LCD(txByte)    SpiTransferSpi2(txByte)

/*
*****************************************************************************
 P U B L I C   D A T A
*****************************************************************************
*/

/*
*****************************************************************************
 P U B L I C   F U N C T I O N   P R O T O T Y P E S
*****************************************************************************
*/
void Lcd_Init( void );
void Lcd_SendString(uint8_t row, char *pBuf);
#endif

