/**
  @page LCD_DSI_VideoMode_DoubleBuffering : LCD DSI examples in DSI mode Video Burst

  @verbatim
  ******************************************************************************
  * @file    LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LCD DSI in mode video burst example.
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
  @endverbatim

@par Example Description

How to use the embedded LCD DSI controller (using the LTDC and DSI Host IPs) to
drive the KoD LCD mounted on-board.

The goal of this example is to display a QVGA landscape (320x240) images on LCD glass
or the HDMI monitor in continuous mode (video burst) using two buffers one for display and one for draw.
The example can automatically detect if the KoD LCD is connected or if the DSI-HDMI 
bridge is connected to the DSI connector (CN1), if no DSI slave is connected the RED LED will turn On.

Layer0 is initialized to display a brief description of the example. It will be
used for images display also.

Line Event callback (LTDC) is used to swap front and back buffer upon drawing 
complete refresh (V-SYNC). Each image is displayed for two seconds. 
'back_buffer' pointer is used to select image to draw in back buffer,
while 'front_buffer' pointer is used to select image to display in front buffer.

The LED1 is ON when an error occurs during initialization of the LCD.

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

@Note: The STM32F769I-DISCO board uses an LCD display in landscape mode with a 45� tearing effect
       limitation during the transition between the first and second images. To avoid this limitation,
       we suggest using an adapted DSI command mode with a left/right refresh technique

@par Keywords

Graphic, Display, LCD, DSI, MIPI Alliance, Video mode, Double buffering, LTDC, QVGA, ARGB8888, SDRAM

@Note�If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
 �����then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
������The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
����� It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
������In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
������Even though the user must manage the cache coherence for read accesses.
������Please refer to the AN4838 �Managing memory protection unit (MPU) in STM32 MCUs�
������Please refer to the AN4839 �Level 1 cache on STM32F7 Series�

@par Directory contents

  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Inc/stm32f7xx_hal_conf.h          HAL configuration file
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Inc/stm32f7xx_it.h                Interrupt handlers header file
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Inc/main.h                        Header for main.c module
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Inc/life_augmented_argb8888.h     Image 320x240 in ARGB8888 to display on LCD
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Inc/image_320x240_argb8888.h      Image 320x240 in ARGB8888 to display on LCD   
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Src/stm32f7xx_it.c                Interrupt handlers
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Src/main.c                        Main program
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Src/stm32f7xx_hal_msp.c           HAL MSP file
  - LCD_DSI/LCD_DSI_VideoMode_DoubleBuffering/Src/system_stm32f7xx.c            STM32F7xx system source file

@par Hardware and Software environment

  - This example runs on STM32F769xx/STM32F779xx devices.

  - This example has been tested with STMicroelectronics STM32F769I-DISCOVERY
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Mount either the KoD LCD panel (Board MB1166 rev.A) or the DSI-HDMI bridge(Board MB1232 Rev.A)
   to the DSI connector (CN1). 
 - if using the DSI-HDMI bridge then connect a HDMI monitor using a HDMI cable to the 
   MB1232 board. 
 - Run the example



 */
