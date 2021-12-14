/**
  @page IAP AN4657 STM32F7xx In-Application Programming using the USART Readme file

  ******************************************************************************
  * @file    IAP/IAP_Main/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of implementation of the AN4657 (in-application programming
  *          using the USART (IAP)) on STM32F7xx devices.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

@par Example Description

This directory contains a set of sources files and pre-configured projects that 
describes how to build an application to be loaded into Flash memory using
In-Application Programming (IAP, through USART).

If the flash configuration is Dual bank mode uncomment the #define USE_DUAL_BANK_FLASH in main.h to download, upload
and execute the binary file on a dual bank flash, otherwise the default flash configuration is a Single bank flash.

@note You have to initially configure the FLASH memory at Single or Dual Bank mode using STM32 ST-LINK 
      Utilities or any similar tool (check "nDBANK" box) according to the flash mode selected on main.h.

@note Using the Dual bank mode, the firmware code is executed from Sector0 on Bank1 and the download/upload
      operations are done on Bank2.

@par Keywords

IAP, In-Application Programming, Dual Bank, Binary, Flash memory, USART

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

 - "IAP/IAP_Main/Inc": contains the IAP firmware header files 
    - IAP/IAP_Main/Inc/main.h              The main include file of the project.
    - IAP/IAP_Main/Inc/common.h            This file provides all the headers of the common functions.
    - IAP/IAP_Main/Inc/flash_if.h          This file provides all the firmware 
                                                     function headers of the flash_if.c file.
    - IAP/IAP_Main/Inc/menu.h              This file provides all the firmware
                                                     function headers of the menu.c file.
    - IAP/IAP_Main/Inc/ymodem.h            This file provides all the firmware
                                                     function headers of the ymodem.c file.
    - IAP/IAP_Main/Inc/stm32f7xx_hal_conf.h  Library Configuration file
    - IAP/IAP_Main/Inc/stm32f7xx_it.h      Header for stm32f7xx_it.c
 - "IAP/IAP_Main/Src": contains the IAP firmware source files
    - IAP/IAP_Main/Src/main.c              Main program
    - IAP/IAP_Main/Src/stm32f7xx_it.c      Interrupt handlers
    - IAP/IAP_Main/Src/stm32f7xx_hal_msp.c Microcontroller specific packages
                                                     initialization file.
    - IAP/IAP_Main/Src/flash_if.c          The file contains write, erase and disable
                                                     write protection of the internal Flash
                                                     memory.
    - IAP/IAP_Main/Src/menu.c              This file contains the menu to select
                                                     downloading a new binary file, uploading
                                                     internal Flash memory, executing the binary
                                                     and disabling the write protection of
                                                     write-protected pages
    - IAP/IAP_Main/Src/common.c            This file provides functions related to
                                                     read/write from/to USART peripheral
    - IAP/IAP_Main/Src/ymodem.c            This file provides all the firmware functions
                                                     related to the ymodem protocol.
    - IAP/IAP_Main/Src/system_stm32f7xx.c  STM32F7xx system source file

@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with STM32F769I-EVAL and can be
    easily tailored to any other supported device and development board.
  
Table 1. IAP implementation on STM32F769I-EVAL
/*** Platform ***|************* Implementation **************************|***** Configuration *****\
+----------------+-------------------------------------------------------+-----------------------------+
|    Firmware    | The IAP program is located at 0x08000000. The Flash   |                             |
|                | routines (program/erase) are executed from the Flash  |                             |
|                | memory.                                               |                             |
|                | The size of this program is about 16 Kbytes and       |                             |
|                | programmed on:                                        | Sector0                     |  
|                | ------------------------------------------------------|-----------------------------|
|                | The user application (image to be downloaded with the |                             | 
|                | IAP) will be programmed starting from address as:     |                             |
|                |   -Dual Bank flash: 0x08100000                        |                             | 
|                |   -Single Bank flash: 0x08008000                      |                             |
|                | The maximum size of the image to be loaded is:        |                             | 
|                |   -Dual Bank flash:                                   | Sector12, 13 (Bank2): 32 KB |
|                |   -Single Bank flash:                                 | Sector1 (Bank1): 32 KB      |
|                | ------------------------------------------------------|-----------------------------|
|                | The image is uploaded with the IAP from the STM32F7xx | 11 Kbytes                   | 
|                | internal Flash.                                       |                             |
|                | The size of the image to be uploaded is:              |                             |
|----------------|-------------------------------------------------------|-----------------------------|
|    Hardware    | Push-button (active level: high)                      | Tamper push-button          |                                                                     
|                |                                                       | connected to pin PC13       |
|                | ------------------------------------------------------|-----------------------------| 
|                | USART used                                            |  USART1   (CN7)             |
+----------------+-------------------------------------------------------+-----------------------------+
(1) User application location address is defined in the flash_if.h file as: 
      -Dual Bank flash:   #define APPLICATION_ADDRESS           ((uint32_t)0x08100000)
      -Single Bank flash: #define APPLICATION_ADDRESS           ((uint32_t)0x08008000)
    User application location End address is defined in the flash_if.h file as: 
      -Dual Bank flash:   #define USER_FLASH_END_ADDRESS           ((uint32_t)0x08100000)
      -Single Bank flash: #define USER_FLASH_END_ADDRESS           ((uint32_t)0x08008000)
To modify it, change the default value to the desired one. Note that the application must be linked
relatively to the new address too.

Following picture illustrates the situation in program memory:
Figure 2. Flash memory usage

                                 Single bank flash            Dual bank flash
  Flash End       0x08200000  /----------------------\    /----------------------\  0x08200000 Flash End
                              |       Sector11       |    |       Sector23       |
                              |          :           |    |          :           |
                              |          :           |    |       Sector14       |
  Sector4 (128KB) 0x08018000  |----------------------|    |----------------------|  0x08108000 Sector14 (64KB)
                              |          :           |    |                      |
                              |          :           |    |                      |
  Sector3 (32KB)  0x08018000  |          :           |    |      User code       |  0x08104000 Sector13 (16KB)
                              |          :           |    |                      |
                              |       Sector2        |    |     Vector table     |
  Sector2 (32KB)  0x08010000  |----------------------|    |----------------------|  0x08100000 Sector12 (16KB)
              :               |       User code      |    |          :           |           :
              :               |     Vector table     |    |          :           |           :
  Sector1 (32KB)  0x08008000  |----------------------|    |----------------------|  0x08004000 Sector1 (16KB)
                              |       IAP code       |    |       IAP code       |
                              |     Vector table     |    |     Vector table     |
  Sector0 (32KB)  0x08000000  \----------------------/    \---------------------/   0x08000000 Sector0 (16KB)
						  
   
  - STM32F769I-EVAL Set-up
    - Connect a null-modem female/female RS232 cable between the boards DB9 connector 
      CN7 (USART1) and PC serial port.
      (make sure that jumper JP11 is fitted).
    - Hold the Tamper push-button during reset to enter the IAP.   

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

  1. Generate a binary image for the program provided in the "IAP/IAP_SingleBank_Binary_Template"
     or "IAP/IAP_DualBank_Binary_Template" project directory. 
  2. Program the internal Flash with the IAP (see below) 
  3. Open HyperTerminal window using the settings already defined in section
     "Hardware and Software environment" 
  4. To run the IAP driver, keep the Tamper push-button pressed at Reset. 
     The IAP main menu is then displayed on the HyperTerminal window.
  5. To download an application, press 1 and use the Ymodem protocol

In order to load the IAP code, you have do the following:
   - EWARM:
      - Open the Project.eww workspace
      - Rebuild all files: Project->Rebuild all
      - Load project image: Project->Debug
      - Run program: Debug->Go(F5)


 */
