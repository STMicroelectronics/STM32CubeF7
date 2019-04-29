/**
  @page BSP Example on how to use the BSP drivers
  
  @verbatim
  ******************************************************************************
  * @file    Examples/BSP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description STM32F7xx_Nucleo_144 Demo description.
  ******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

The BSP examples detects the presence of Adafruit 1.8" TFT shield with joystick and uSD.

If the Adafruit shield is NOT connected, then:
 - Blue led (led2) blinks waiting the user button is pushed.
 - Then blue led swithces off and green & red leds start blinking at high frequency.
 - Pushing user button again and again blinking frequency decreases until looping 
   to high frequency.

If the Adafruit shield is connected, then this example shows how to use the different 
functionalities of LCD, SD card and joystick by switching between tests using user button. 
  - Firstly, use the joystick button to move a pointer inside a rectangle 
    (up/down/right/left) and change the pointer color(select).
  - Secondly, this example shows how to use the different LCD features to display 
    string with different fonts, to display different shapes and to draw a bitmap.
  - Thirdly, this example shows how to erase, write and read the SD card.
At the end of the nine examples when pushing the user button the application loops 
to the beginning (first examples). 

@par Keywords

Board Support Package, BSP, Adafruit, TFT, LCD, Joystick, SD Card

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

  - BSP/Src/main.c                     Main program
  - BSP/Src/system_stm32f7xx.c         STM32F7xx system clock configuration file
  - BSP/Src/stm32f7xx_it.c             Interrupt handlers 
  - BSP/Src/joystick.c                 Joystick feature
  - BSP/Src/lcd.c                      LCD drawing features
  - BSP/Src/log.c                      LCD Log firmware functions
  - BSP/Src/sd.c                       SD features
  - BSP/Inc/main.h                     Main program header file  
  - BSP/Inc/stm32f7xx_hal_conf.h       HAL configuration file
  - BSP/Inc/stm32f7xx_it.h             Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h             lcd_log configuration template file
  - BSP/Inc/stlogo.h                   Image used for BSP example


@par Hardware and Software environment

 - This example runs on STM32F746ZG Devices 
  
 - This example has been tested with STMicroelectronics NUCLEO-F746ZG Rev.B board
   and can be easily tailored to any other supported device and development board.

 - NUCLEO-F746ZG Rev.B board Set-up
   - Connect the Adafruit 1.8" TFT shield (https://www.adafruit.com/products/802)
   - Make sure the SB165 is closed and the SB146 and SB174 are opened on the NUCLEO-F746ZG board
   
@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the Project.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
 
      
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
