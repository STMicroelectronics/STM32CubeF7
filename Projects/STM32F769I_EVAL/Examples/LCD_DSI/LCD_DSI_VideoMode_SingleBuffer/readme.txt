/**
  @page LCD_DSI_VideoMode_SingleBuffer : LCD DSI examples in DSI mode Video Burst

  @verbatim
  ******************************************************************************
  * @file    LCD_DSI/LCD_DSI_VideoMode_SingleBuffer/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LCD DSI in mode video burst example.
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

How to use the embedded LCD DSI controller (using the LTDC and DSI Host IPs) to
drive the KoD LCD mounted on-board.

The goal of this example is to display a QVGA landscape (320x240) images on LCD glass
or the HDMI monitor in continuous mode (video burst) using a single buffer for display 
and for draw.
The example can automatically detect if the KoD LCD is connected or if the DSI-HDMI 
bridge is connected to the DSI connector (CN15), if no DSI slave is connected the RED LED will turn On.


Layer0 is initialized to display a brief description of the example. It will be
used for images display also.

Line Event callback (LTDC) is used to swap image to be draw to display buffer upon  
drawing complete refresh (V-SYNC). Each image is displayed for two seconds. The same
buffer is used to draw and display image. 

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.

@par Keywords

Graphic, Display, LCD, DSI, MIPI Alliance, Video mode, Single buffer, LTDC, QVGA, ARGB8888, SDRAM

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

  - LCD_DSI/LCD_DSI_VideoMode_SingleBuffer/Inc/stm32f7xx_hal_conf.h          HAL configuration file
  - LCD_DSI/LCD_DSI_VideoMode_SingleBuffer/Inc/stm32f7xx_it.h                Interrupt handlers header file
  - LCD_DSI/LCD_DSI_VideoMode_SingleBuffer/Inc/main.h                        Header for main.c module
  - LCD_DSI/LCD_DSI_VideoMode_SingleBuffer/Inc/life_augmented_argb8888.h     Image 320x240 in ARGB8888 to display on LCD
  - LCD_DSI/LCD_DSI_VideoMode_SingleBuffer/Inc/image_320x240_argb8888.h      Image 320x240 in ARGB8888 to display on LCD  
  - LCD_DSI/LCD_DSI_VideoMode_SingleBuffer/Src/stm32f7xx_it.c                Interrupt handlers
  - LCD_DSI/LCD_DSI_VideoMode_SingleBuffer/Src/main.c                        Main program
  - LCD_DSI/LCD_DSI_VideoMode_SingleBuffer/Src/stm32f7xx_hal_msp.c           HAL MSP file
  - LCD_DSI/LCD_DSI_VideoMode_SingleBuffer/Src/system_stm32f7xx.c            STM32F7xx system source file

@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.

  - This example has been tested with STMicroelectronics STM32F769I-EVAL
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Mount either the KoD LCD pannel (Board MB1166 rev.A) or the DSI-HDMI bridge(Board MB1232 Rev.A)
   to the DSI connector (CN15). 
 - if using the DSI-HDMI bridge then connect a HDMI monitor using a HDMI cable to the 
   MB1232 board.  
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
