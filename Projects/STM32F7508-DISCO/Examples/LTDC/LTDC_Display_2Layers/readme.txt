/**
  @page LTDC_Display_2Layers LTDC Display 2 layers example
  
  @verbatim
  ******************************************************************************
  * @file    LTDC/LTDC_Display_2Layers/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LTDC Display 2 layers example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
   @endverbatim

@par Example Description

  How to configure the LTDC peripheral to display two layers at the same time.
  The goal of this example is to explain how to blend two layers and how to display 
  an image with L8 (8bits per pixels indexed) as pixel format. 

  In this example, the code is executed from QSPI external memory while data are in internal
  SRAM memory.

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  Then the SystemClock_Config() function is used to configure the system
  clock (SYSCLK) to run at 216 MHz.
 
  After LCD initialization, the LCD layer 1 and Layer 2 are configured as follows :
  - Layer 1 is configured to display an image loaded from flash memory with direct
    color (RGB565) as pixel format and 320x240 (QVGA) size.
  - Layer 2 is configured to display an image loaded from flash memory with indirect
    color (L8) as pixel format and 320x240 (QVGA) size.
    To display an image with an indirect color as pixel format, a color lookup table (CLUT) 
    is loaded, then every byte from image data (L8_320x240) is considered as 
    a position in color lookup table (CLUT).(see example below)  

  The blending is always active and the two layers can be blended following 
  the configured blending factors and the constant alpha.
  In this example the constant alpha for layer 2 is decreased to see the layer 1
  in the intersection zone.

  Example :
  ------------------------------------------------------------------------------
  |Image data   |  position   CLUT(RGB Value)  |   Alpha  | Output (ARGB value)|
  |-------------|------------------------------|----------|------------------- |
  |00 10 FF 25  |  0   -----> 0xFD10EA         | 0xFF     | 0xFFFD10EA         |
  |             |  .                           |          | 0xFF1548AD         |
  |             |  .                           |          | 0xFFAE6547         |
  |             |  .                           |          | 0xFFDA14EA         |
  |             |  .                           |          | .                  |
  |             |  16  -----> 0x1548AD         |          | .                  |
  |             |  .                           |          | .                  |
  |             |  .                           |          | .                  |
  |             |  37  -----> 0xDA14EA         |          | .                  |
  |             |  .                           |          |                    |
  |             |  .                           |          |                    |
  |             |  255 -----> 0xAE6547         |          |                    |
  ------------------------------------------------------------------------------       

  The images swap their positions across the LCD in an infinite loop.

STM32 Discovery board's LED can be used to monitor the transfer status:
 - LED1 is ON when there is an error in Init process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, LTDC, Display Controller, TFT, LCD, Graphic, RGB888, 2 Layers, BMP, 

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

    - LTDC/LTDC_Display_2Layers/Inc/stm32f7xx_hal_conf.h    HAL configuration file
    - LTDC/LTDC_Display_2Layers/Inc/stm32f7xx_it.h          Interrupt handlers header file
    - LTDC/LTDC_Display_2Layers/Inc/main.h                  Main configuration file
    - LTDC/LTDC_Display_2Layers/Src/stm32f7xx_it.c          Interrupt handlers
    - LTDC/LTDC_Display_2Layers/Src/main.c                  Main program 
    - LTDC/LTDC_Display_2Layers/Src/stm32f7xx_hal_msp.c     HAL MSP module	
    - LTDC/LTDC_Display_2Layers/Src/system_stm32f7xx.c      STM32F7xx system clock configuration file
    - LTDC/LTDC_Display_2Layers/Inc/RGB565_320x240.h        Image layer 1 to be displayed on LCD : QVGA (320x240) in RGB565
    - LTDC/LTDC_Display_2Layers/Inc/L8_320x240.h            Image layer 2 to be displayed on LCD QVGA (320x240) with CLUT8 format.

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
  3. Program the external memory with this example "Examples\LTDC\LTDC_Display_2Layers" (see below).
  4. Start debugging user example or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image
      
In order to load this LTDC example to the external memory :
 - Open your preferred toolchain 
 - Rebuild all files. 
 - Run & debug the program:
   - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
   - Using SW4STM32 :
       - Open the STM32CubeProgrammer tool
       - Select the QSPI external flash loader "N25Q128A_STM32F7508-DISCO" 
       - From Erasing & Programming menu, browse and open the output binary file relative to this example
       - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)

                           
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
                                   