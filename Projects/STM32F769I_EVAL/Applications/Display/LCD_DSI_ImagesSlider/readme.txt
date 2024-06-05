/**
  @page LCD_DSI_ImagesSlider : LCD DSI images slider application  

  @verbatim
  ******************************************************************************
  * @file    Display\LCD_DSI_ImagesSlider/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LCD DSI images slider application.
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

@par Application Description

This application aims to show the outstanding capability of Display Serial Interface (DSI)
 periphiral to display images with high resolution (800x480). With a simple movement of finger,
the content of GRAM is directly updated and displayed on DSI LCD. 
Thanks to Tearing Effect and Partial refresh features, that allow user to provoke
a refresh whenever he wants and on the specified region he desires, the refresh 
of LCD is more rapid than any time before.

At the begining, the quadspi is intialized to put images in. Then TS and LCD DSI 
are initialized then a first image is displayed.
In this example, the refresh of LCD is made in partial mode (left then right). 

Two mode of images draw are possible through this application; vertical draw or
horizontal draw. The default mode is horizontal. User can change the mode by 
pressing Tamper button.

By moving the finger on TS, a new image will progressively displayed on LCD while
the old one disappears progressively. When HORIZONTAL_DRAW mode is active, moving 
finger left or right causes the display of next or previous image. 
When VERTICAL_DRAW mode is active, moving finger up or down causes the display of 
next or previous image.

The program runs in infinite loop.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, Graphic, DSI, LCD, Image slider, GRAM, Touch screen, 

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

  - Display/LCD_DSI_ImagesSlider/Inc/stm32f7xx_hal_conf.h          HAL configuration file
  - Display/LCD_DSI_ImagesSlider/Inc/stm32f7xx_it.h                Interrupt handlers header file
  - Display/LCD_DSI_ImagesSlider/Inc/main.h                        Header for main.c module
  - Display/LCD_DSI_ImagesSlider/Inc/image*.h                      Images to be displayed
  - Display/LCD_DSI_ImagesSlider/Src/stm32f7xx_it.c                Interrupt handlers
  - Display/LCD_DSI_ImagesSlider/Src/main.c                        Main program
  - Display/LCD_DSI_ImagesSlider/Src/system_stm32f7xx.c            STM32F7xx system source file

@par Hardware and Software environment

  - This example runs on STM32F769xx/STM32F779xx devices.

  - This example has been tested with STMicroelectronics STM32F769I-EVAL
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files
 - Open STM32 ST-Link Utility V3.9, click on "External Loader" from the bar menu then check "MT25QL512A_STM32769I-EVAL" box
 - Connect the STM32F769I-EVAL board to PC with USB cable through CN22
 - Use generated hex file with STM32 ST-Link Utility to program both internal Flash and external QSPI memory
 - Run the application 



 */
