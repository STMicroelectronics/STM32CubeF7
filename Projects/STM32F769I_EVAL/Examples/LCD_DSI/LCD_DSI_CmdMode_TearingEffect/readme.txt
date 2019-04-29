/**
  @page LCD_DSI_CmdMode_TearingEffect :  LCD DSI examples in DSI command mode

  @verbatim
  ******************************************************************************
  * @file    LCD_DSI/LCD_DSI_CmdMode_TearingEffect/readme.txt
  * @author  MCD Application Team
  * @brief    Description of the LCD DSI in command mode example.
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
in Command mode using Tearing Effect, through DSI LINK, and partial 
refresh method. One buffer is used for display and for draw.

Layer0 is initialized to display a brief description of the example. It will be
used for images display also.

In this example, the display area is splitted in tow part right and left. When the
end of refresh event occured, left part of display area is refreshed/displayed while
the other right part is prepared to be displayed when Line Event occurs.

Partial Refresh with splitting method is based on the following steps:
- Set area of refresh by fixing Page and Column values.
- Select active area (LEFT_AREA)
- Set Scan Line at line 533, this to allow to write data to LEFT_AREA before a new
  refresh comes.
- Copy image to LCD frame buffer address
- Refresh LEFT_AREA
- refresh RIGHT_AREA

Each image is displayed for two secondes.

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

Graphic, Display, LCD, DSI, MIPI Alliance, Command mode, Tearing Effect, Partial refresh, LTDC, QVGA, ARGB8888

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

  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Inc/stm32f7xx_hal_conf.h       HAL configuration file
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Inc/stm32f7xx_it.h             Interrupt handlers header file
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Inc/main.h                     Header for main.c module
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Inc/life_augmented_argb8888.h  Image 320x240 in ARGB8888 to display on LCD
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Inc/image_320x240_argb8888.h   Image 320x240 in ARGB8888 to display on LCD
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Src/stm32f7xx_it.c             Interrupt handlers
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Src/main.c                     Main program
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Src/stm32f7xx_hal_msp.c        HAL MSP file
  - LCD_DSI/LCD_DSI_CmdMode_TearingEffect/Src/system_stm32f7xx.c         STM32F7xx system source file

@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.

  - This example has been tested with STMicroelectronics STM32F769I-EVAL
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
