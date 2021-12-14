/**
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "GUI.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/* Define the available number of bytes available for the GUI */
#define GUI_NUMBYTES  (1024) * 3800

#if defined ( __ICCARM__ ) // !< IAR Compiler
#pragma location=0xC0300000
static __no_init U32 aMemory[GUI_NUMBYTES / 4];

#elif defined ( __CC_ARM ) //!< Keil Compiler
U32 aMemory[GUI_NUMBYTES / 4] __attribute__((at(0xC0300000)));

#elif defined ( __GNUC__ ) // !< GNU Compiler
U32 aMemory[GUI_NUMBYTES / 4] __attribute__((section(".GUI_NUMBYTES_section")));
#endif


/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   available memory for the GUI.
*/
void GUI_X_Config(void) {
  /* Assign memory to STemWin */
  GUI_ALLOC_AssignMemory(aMemory, GUI_NUMBYTES);
}

