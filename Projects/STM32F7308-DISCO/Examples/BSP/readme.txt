/**
  @page BSP  Example on how to use the BSP drivers
  
  @verbatim
  ******************************************************************************
  * @file    BSP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
  ******************************************************************************
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

This example provides a description of how to use the different BSP drivers. 
The code is executed from QSPI external memory while data are in internal
SRAM memory.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 216 MHz.

This example shows how to use the different functionalities of components 
available on the board by switching between all tests using user button.

#### LCD ####
This example shows how to use the different LCD features to display strings
with different fonts, to display different shapes and to draw a bitmap.

#### Touchscreen demo1 ####
Use touchscreen functionality to select or activate colored circle inside a rectangle.

#### Touchscreen demo2 ####
Use touchscreen functionality to select 2 touch points, their positions will be
captured and displayed on the LCD.

#### Touchscreen demo3 ####
Use touchscreen functionality to select or activate one square in a grid and colore it.
The touch detection is done in interrupt mode.

#### LOG ####
This example show how to use the LCD log features.

#### PSRAM ####
This example shows how to use the IS66WV51216EBLL-55BLI PSRAM features(read/write/erase..).

#### PSRAM DMA ####
This example shows how to use the IS66WV51216EBLL-55BLI PSRAM features(read/write/erase..) in DMA mode.

#### AUDIO PLAY ####
This example shows how to play an audio file through the I2S peripheral
using the external codec WM8994 implemented on the STM327308-DISCOVERY board. The I2S input 
clock, provided by a dedicated PLL (PLLI2S), is configured initially to have an audio  
sampling frequency at 48 KHz. The audio data is stored in the external QSPI memory 
(4 channels, 16-bit, 48 KHz). Following the instruction on the display, stream can be 
paused and resumed, volume can be changed and sample frequency can be changed.

#### AUDIO LOOPBACK ####
This example shows how to acquire sound from microphones and playback 
it in parallel on headphones.

LED6(GREEN) remains toggling while no example is running.
At the end of all examples when pushing the user-button the application loops 
to the beginning (first examples). 

@Note: Copy file 'audio_sample.bin' (available in Binary) directly in 
       the QSPI external memory at @0x90040000 using STM32CubeProgrammer
       
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

BSP, LCD, Touchscreen, PSRAM, DMA, QSPI, Erase, Read, Write, Audio play, Record, MEMS, Audio Codec, DFSDM,
I2S, Microphone, Headphones

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
  - BSP/Src/system_stm32f7xx.c         STM32f7xx system clock configuration file
  - BSP/Src/stm32f7xx_it.c             Interrupt handlers 
  - BSP/Src/lcd.c                      LCD drawing features
  - BSP/Src/touchscreen.c              Touch Screen features  
  - BSP/Src/log.c                      LCD Log firmware functions
  - BSP/Src/sd.c                       SD features
  - BSP/Src/audio.c                    Audio playback features     
  - BSP/Src/audio_record_loopback.c    DFSDM audio record features      
  - BSP/Inc/main.h                     Main program header file  
  - BSP/Inc/stm32f7xx_hal_conf.h       HAL configuration file
  - BSP/Inc/stm32f7xx_it.h             Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h             lcd_log configuration template file
  - BSP/Inc/stlogo.h                   Image used for BSP example
        
        
@par Hardware and Software environment  

  - This example runs on STM32F730xx devices.
  
  - This example has been tested with STMicroelectronics STM327308-DISCOVERY 
    board and can be easily tailored to any other supported device 
    and development board.
    
  
@par How to use it ? 

 - Use STM32CubeProgrammer, available on www.st.com or any other in system programming
   tool to load "BSP/Binary/audio_sample_tdm.bin" file to the external QSPI flash 
   at the address 0x90040000.

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc.
     The default configuration is the right one: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with this example "Examples/BSP" (see below).
  4. Start debugging user example or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image
      
In order to load the BSP example to the external memory :
 - Open your preferred toolchain 
 - Rebuild all files. 
 - Run & debug the program:
   - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
   - Using SW4STM32 :
       - Open the STM32CubeProgrammer tool
       - Select the QSPI external flash loader "M25L512G_STM32F7308-DISCO" 
       - From Erasing & Programming menu, browse and open the output binary file 
         'XIP_QSPI_InternalSRAM.bin'
       - Load the binary file into the external QSPI flash using "Start Programming"
         at the address 0x90000000

     @note Make sure that the tool flash loader does not erase or overwrite the
        loaded audio file at address 0x90040000 by limiting the application
        end address to 0x9003FFFF. This is already done for the example project

 - Connect a headphone and a speaker to the audio jack connectors (CN5).


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
