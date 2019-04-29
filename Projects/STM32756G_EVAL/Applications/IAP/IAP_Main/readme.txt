/**
  @page IAP AN4657 STM32F7xx In-Application Programming using the USART Readme file

  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    IAP/IAP_Main/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of implementation of the AN4657 (in-application programming
  *          using the USART (IAP)) on STM32F7xx devices.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

@par Example Description

This directory contains a set of sources files and pre-configured projects that 
describes how to build an application to be loaded into Flash memory using
In-Application Programming (IAP, through USART).

@par Keywords

In-Application Programming, IAP, USART, Binary, flash

@Note If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
      then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
      The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
      In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
      Even though the user must manage the cache coherence for read accesses.
      Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
      Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

@par Directory contents

 - "IAP/IAP_Main/inc": contains the IAP firmware header files 
    - IAP/IAP_Main/inc/main.h              The main include file of the project.
    - IAP/IAP_Main/inc/common.h            This file provides all the headers of the common functions.
    - IAP/IAP_Main/inc/flash_if.h          This file provides all the firmware 
                                                     function headers of the flash_if.c file.
    - IAP/IAP_Main/inc/menu.h              This file provides all the firmware
                                                     function headers of the menu.c file.
    - IAP/IAP_Main/inc/ymodem.h            This file provides all the firmware
                                                     function headers of the ymodem.c file.
    - IAP/IAP_Main/inc/stm32f7xx_hal_conf.h  Library Configuration file
    - IAP/IAP_Main/inc/stm32f7xx_it.h      Header for stm32f7xx_it.c

 - "IAP/IAP_Main/MDK-ARM": contains pre-configured project for MDK-ARM toolchain

 - "IAP/IAP_Main/EWARM": contains pre-configured project for EWARM toolchain
 
 - "IAP/IAP_Main/SW4STM32": contains preconfigured project for SW4STM32 toolchain (main program)

 - "IAP/IAP_Main/src": contains the IAP firmware source files
    - IAP/IAP_Main/src/main.c              Main program
    - IAP/IAP_Main/src/stm32f7xx_it.c      Interrupt handlers
    - IAP/IAP_Main/src/stm32f7xx_hal_msp.c Microcontroller specific packages
                                                     initialization file.
    - IAP/IAP_Main/src/flash_if.c          The file contains write, erase and disable
                                                     write protection of the internal Flash
                                                     memory.
    - IAP/IAP_Main/src/menu.c              This file contains the menu to select
                                                     downloading a new binary file, uploading
                                                     internal Flash memory, executing the binary
                                                     and disabling the write protection of
                                                     write-protected pages
    - IAP/IAP_Main/src/common.c            This file provides functions related to
                                                     read/write from/to USART peripheral
    - IAP/IAP_Main/src/ymodem.c            This file provides all the firmware functions
                                                     related to the ymodem protocol.
    - IAP/IAP_Main/src/system_stm32f7xx.c  STM32F7xx system source file

@par Hardware and Software environment

  - This example runs on STM32F756xx/STM32F746xx devices.
    
  - This example has been tested with STM327x6G-EVAL board revB and can be
    easily tailored to any other supported device and development board.
  
Table 1. IAP implementation on STM327x6G-EVAL RevB
/*** Platform ***|************* Implementation **************************|***** Configuration *****\
****************************************************************************************************
|    Firmware    | The IAP program is located at 0x08000000. The Flash   |                         |
|                | routines (program/erase) are executed from the Flash  |                         |
|                | memory.                                               |                         |
|                | The size of this program is about 16 Kbytes and       |                         |
|                | programmed on:                                        | Sector 0                |  
|                | ------------------------------------------------------|-------------------------|
|                | The user application (image to be downloaded with the |                         | 
|                | IAP) will be programmed starting from address         |                         |
|                | (uint32_t)0x08008000(1).                              | (Sector 1 - Sector 7)   | 
|                | The maximum size of the image to be loaded is:        | 992 Kbytes              | 
|                | ------------------------------------------------------|-------------------------|
|                | The image is uploaded with the IAP from the STM32F7xx | 7 Kbytes                | 
|                | internal Flash.                                       | (Sector 1)              |
|                | The size of the image to be uploaded is:              |                         |
|----------------|-------------------------------------------------------|-------------------------|
|    Hardware    | Push-button (active level: high)                      | Tamper push-button      |                                                                     
|                |                                                       | connected to pin PC13   |
|                | ------------------------------------------------------|-------------------------| 
|                | USART used                                            |  USART1   (CN7)         |
\**************************************************************************************************/
(1) User application location address is defined in the flash_if.h file as: 
#define APPLICATION_ADDRESS           ((uint32_t)0x08008000)
To modify it, change the default value to the desired one. Note that the application must be linked
relatively to the new address too.

Following picture illustrates the situation in program memory:
Figure 2. Flash memory usage

 Top Flash Memory address /-------------------------------------------\  0x080FFFFF
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |          Sector 1 - Sector 7              |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |          User code                        |
                          |                                           |
                          |- - - - - - - - - - - - - - - - - - - - - -|
                          |          Vector table                     |
                          |-------------------------------------------|  0x08008000
                          |          IAP code                         |
                          |- - - - - - - - - - - - - - - - - - - - - -|
                          |          Vector table                     |
                          \-------------------------------------------/	 0x08000000					  
   
  - STM327x6G-EVAL RevB Set-up
    - Connect a null-modem female/female RS232 cable between the boards DB9 connector 
      CN7 (USART1) and PC serial port.
      (make sure that jumper JP7 is RS232_RX position pos 1-2).
    - Hold the Key push-button during reset to enter the IAP.    

  - Terminal configuration: 
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - flow control: None 
    - Ymodem protocol is using CRC16 by default. To switch to checksum, comment #define CRC16_F
      in ymodem.c

@par How to use it? 

In order to make the program work, you must do the following:

  1. Generate a binary image for the program provided in the 
     "IAP/IAP_Binary_Template" project directory. 
  2. Program the internal Flash with the IAP (see below) 
  3. Open HyperTerminal window using the settings already defined in section
     "Hardware and Software environment" 
  4. To run the IAP driver, keep the Key push-button pressed at Reset. 
     The IAP main menu is then displayed on the HyperTerminal window.
  5. To download an application, press 1 and use the Ymodem protocol

In order to load the IAP code, you have do the following:
   - EWARM:
      - Open the Project.eww workspace
      - Rebuild all files: Project->Rebuild all
      - Load project image: Project->Debug
      - Run program: Debug->Go(F5)

   - MDK-ARM:
      - Open the Project.uvproj project
      - Rebuild all files: Project->Rebuild all target files
      - Load project image: Debug->Start/Stop Debug Session
      - Run program: Debug->Run (F5)
      
   - System Workbench for STM32: 
      - Open System Workbench for STM32 toolchain
      - Browse to the SW4STM32 workspace directory, select the project (.project file in \IAP_Main\SW4STM32 \STM327x6G_EVAL directory).
      - Rebuild all project files: Select the project in the "Project explorer" window then click on Project->build project menu.
      - Run program: Run->Debug

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
