/**
  @page STemWin_Animation  STemWin animation Readme file
 
  @verbatim
  ******************** (C) COPYRIGHT 2018 STMicroelectronics *******************
  * @file    STemWin/STemWin_Animation/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STemWin animation application. 
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
   @endverbatim

@par Application Description

This directory contains a set of source files that implement a simple "animation" 
application based on STemWin for STM32F7xx devices.

The application shows the capability of STemWin to do different animations of different objects independently.

In this application, the code is executed from QSPI external memory while data are in internal
SRAM memory.

 How to interact with the application
 ------------------------------------
 - The application is an automatic run.
 - It will show 3 objects animated independently: the dog, the cloud and a balloon
 - It will loop infinetly.  

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
      then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
      The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
      In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
      Even though the user must manage the cache coherence for read accesses.
      Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
      Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

@par Keywords

Display, Graphic, STemWin, Animation, HelloWorld, LCD, GUI, Demonstration, Touch screen

@par Directory contents 

    - STemWin/STemWin_Animation/Core/Inc/main.h				            Main program header file
    - STemWin/STemWin_Animation/Core/Inc/stm32f7xx_hal_conf.h		    Library Configuration file
    - STemWin/STemWin_Animation/Core/Inc/stm32f7xx_it.h			        Interrupt handlers header file
    - STemWin/STemWin_Animation/Core/Src/main.c				            Main program file
    - STemWin/STemWin_Animation/Core/Src/stm32f7xx_it.c			        STM32F7xx Interrupt handlers
    - STemWin/STemWin_Animation/Core/Src/system_stm32f7xx.c		        STM32F7xx system file
    - STemWin/STemWin_Animation/STemWin/App/animation_app.c		        animation application
    - STemWin/STemWin_Animation/STemWin/App/generated/Background.c	    Background picture
    - STemWin/STemWin_Animation/STemWin/App/generated/Balloon.c		    Balloon picture    
    - STemWin/STemWin_Animation/STemWin/App/generated/dog1_walk1.c	    Dog first picture going right
    - STemWin/STemWin_Animation/STemWin/App/generated/dog1_walk1_r.c	Dog first picture going left
    - STemWin/STemWin_Animation/STemWin/App/generated/dog1_walk2.c	    Dog second picture going right
    - STemWin/STemWin_Animation/STemWin/App/generated/dog1_walk2_r.c	Dog second picture going left
    - STemWin/STemWin_Animation/STemWin/App/generated/cloud.c		    cloud picture
    - STemWin/STemWin_Animation/STemWin/Target/GUIConf.c		        Display controller initialization
    - STemWin/STemWin_Animation/STemWin/Target/GUIConf.h                Header for GUIConf.c
    - STemWin/STemWin_Animation/STemWin/Target/LCDConf.c                Configuration file for the GUI library
    - STemWin/STemWin_Animation/STemWin/Target/LCDConf.h                Header for LCDConf.c
  
@par Hardware and Software environment

  - This application runs on STM32F750xx devices.

  - This application has been tested with STMicroelectronics STM32F7508-DISCO
    boards and can be easily tailored to any other supported device
    and development board.   

@par How to use it ?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc.
     The default configuration is the right one: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with this application "Applications\STemWin\STemWin_Animation" (see below).
  4. Start debugging user example or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image
      
In order to load this STemWin application to the external memory :
 - Open your preferred toolchain 
 - Rebuild all files. 
 - Run & debug the program:
   - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
   - Using SW4STM32 :
       - Open the STM32CubeProgrammer tool
       - Select the QSPI external flash loader "N25Q128A_STM32F7508-DISCO" 
       - From Erasing & Programming menu, browse and open the output binary file relative to this application
       - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
