/*
*|***************************************************************************
*|PROJECT:  
*|Module:   KeypadFd
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
#ifndef KEYPAD_FD_H
 #define KEYPAD_FD_H



/*
*****************************************************************************
 P U B L I C   I N C L U D E   F I L E S
*****************************************************************************
*/
#include "switchFd.h"


/*
*****************************************************************************
 P U B L I C   T Y P E    D E F I N I T I O N S
*****************************************************************************
*/

/*
*****************************************************************************
 P U B L I C    M A C R O S
*****************************************************************************
*/


/*
*****************************************************************************
 P U B L I C   D A T A
*****************************************************************************
*/
extern UINT8 KeyActionRequest;
extern INT8 LcdRow;
extern INT8 LcdSelection;
/*
*****************************************************************************
 P U B L I C   F U N C T I O N   P R O T O T Y P E S
*****************************************************************************
*/


void Keypad_Init
(
   void
);

void KeypadMachine
(
   UINT8 switchId_
);

void KeyPwrUp(void);
void KeyStop(void);

void KeyFwdLow(void);
void KeyFwdMed(void);
void KeyFwdFast(void);
void KeyFwdJog(void);

void KeyRevLow(void);
void KeyRevMed(void);
void KeyRevFast(void);
void KeyRevJog(void);

void KeyReleaseFwdJog(void);
void KeyReleaseRevJog(void);

enum
{
   CONTACT_ID =0,
   KEYBOARD,
   FEEDBACK

};

enum
{              
   KEY_FWD_LOW =0,
   KEY_FWD_MED,      
   KEY_FWD_FAST,  
   KEY_FWD_JOG,
   KEY_STOP,
         
   KEY_REV_LOW,
   KEY_REV_MED,      
   KEY_REV_FAST,  
   KEY_REV_JOG,      

   KEY_PWR_UP,
   
   KEY_RELEASE_FWD_JOG,
   KEY_RELEASE_REV_JOG,
     
   KEY_VIRTUAL,
     
   KEY_NONE,
};

#endif
