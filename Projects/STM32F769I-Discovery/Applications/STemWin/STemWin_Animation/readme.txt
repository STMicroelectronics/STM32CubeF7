/**
  @page STemWin_Animation  STemWin animation Readme file
 
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    STemWin/STemWin_Animation/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STemWin animation application. 
  ******************************************************************************
  *
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
  @endverbatim

@par Application Description

This directory contains a set of source files that implement a simple "animation" 
application based on STemWin for STM32F7xx devices.

The application shows the capability of STemWin to do different animations of different objects independently.

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

@par Keywords

Display, Graphic, STemWin, Animation, HelloWorld, LCD, GUI, Demonstration, Touch screen

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

    - STemWin/STemWin_Animation/Core/Inc/main.h				            Main program header file
    - STemWin/STemWin_Animation/Core/Inc/stm32f7xx_hal_conf.h	        Library Configuration file
    - STemWin/STemWin_Animation/Core/Inc/stm32f7xx_it.h                 Interrupt handlers header file
    - STemWin/STemWin_Animation/Core/Src/main.c                         Main program file
    - STemWin/STemWin_Animation/Core/Src/stm32f7xx_it.c                 STM32F7xx Interrupt handlers
    - STemWin/STemWin_Animation/Core/Src/system_stm32f7xx.c             STM32F7xx system file
    - STemWin/STemWin_Animation/STemWin/App/animation_app.c             animation application
    - STemWin/STemWin_Animation/STemWin/App/generated/Background.c      Background picture
    - STemWin/STemWin_Animation/STemWin/App/generated/Balloon.c         Balloon picture
    - STemWin/STemWin_Animation/STemWin/App/generated/dog1_walk1.c	    Dog first picture going right
    - STemWin/STemWin_Animation/STemWin/App/generated/dog1_walk1_r.c    Dog first picture going left
    - STemWin/STemWin_Animation/STemWin/App/generated/dog1_walk2.c      Dog second picture going right
    - STemWin/STemWin_Animation/STemWin/App/generated/dog1_walk2_r.c    Dog second picture going left
    - STemWin/STemWin_Animation/STemWin/App/generated/cloud.c           cloud picture
    - STemWin/STemWin_Animation/STemWin/Target/GUIConf.c                Display controller initialization
    - STemWin/STemWin_Animation/STemWin/Target/GUIConf.h                Header for GUIConf.c
    - STemWin/STemWin_Animation/STemWin/Target/LCDConf.c                Configuration file for the GUI library
    - STemWin/STemWin_Animation/STemWin/Target/LCDConf.h                Header for LCDConf.c
  
@par Hardware and Software environment  

  - This application runs on STM32F769xx devices.

  - This application has been tested with STMicroelectronics STM32769I-Discovery
    discovery boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
 

 */
