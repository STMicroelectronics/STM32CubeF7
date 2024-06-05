/**
  @page BSP  Example on how to use the BSP drivers
  
  @verbatim
  ******************************************************************************
  * @file    BSP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
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

This example provides a description of how to use the different BSP drivers. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 200 MHz and provide 50 MHz at the output PLL divided by PLL_Q. 
This frequency permit to reach 25 Mhz clock needed for SD operation and in line 
with microSD specification. 

This example shows how to use the different functionalities of LCD, SD card, 
touchscreen, camera and external memories (SDRAM, QSPI flash) by switching 
between all tests using key button. 

1st test : LCD test. This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

2nd test : touchscreen test. This example shows touchscreen capabilities. It's a capacitive
touchscreen which supports up to 5 finger touch. The example fills some drawn circle according  
to touch position and touch pressure. It displays the coordinates of up to 5 detected touchs.

3rd test : audio record test. This example shows how to record an audio file through the SAI peripheral
using the external codec WM8994 implemented on the STM32746G-Discovery board. The SAI input 
clock, provided by a dedicated PLL (PLLI2S), is configured to have an audio sampling 
frequency at 16 KHz. The test records an audio sample from MEMs microphones (U19 and U20) for 8 seconds
and playback it right after to the headphones connected to CN10 audio jack connector.

4th test : audio loopback test. This example shows how to acquire sound from microphones and playback 
it in parallel on headphones.

5th test : audio playback test. This example shows how to play an audio file through the SAI peripheral
using the external codec WM8994 implemented on the STM32746G-Discovery board. The SAI input 
clock, provided by a dedicated PLL (PLLI2S), is configured to have an audio sampling 
frequency at 48 KHz. The audio data is stored in the internal flash memory (Stereo, 
16-bit, 48 KHz).
@Note: Copy file 'audio_sample_tdm.bin' (available in Binary) directly in 
       the flash at @0x08080000 using ST-Link utility

6th test : SD test. This example shows how to erase, write and read the SD card and also 
how to detect the presence of the card.

7th test : LCD log test. This example shows how to use the LCD log features.

8th test : SDRAM test. This example provides of how to write, read and buffers compare 
for external SDRAM memory.

9th test : EEPROM test. This example shows how to read and write data in RF EEPROM. The I2C EEPROM
memory (M24LR64) is available on separate daughter board ANT7-M24LR-A, which is not
provided with the STM32746G-Discovery board. To use this driver you have to connect the 
ANT7-M24LR-A to CN1 connector of STM32746G-Discovery board.

10th test : QSPI test. This example provides of how to write, read and buffers compare 
for external flash memory using QSPI communication.
For the QSPI test, one of the following flags must be defined in the preprocessor:
- USE_STM32746G_DISCO_REVC03 : Board contains the W25Q128J QSPI component.
- USE_STM32746G_DISCO        : Board contains the N25Q128A QSPI component.(Defined by default)


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.
      
@par Keywords

BSP, LCD, Touchscreen, PSRAM, DMA, QUADSPI, Erase, Read, Write, Audio play, Record, MEMS, Audio Codec, DFSDM,
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
  - BSP/Src/system_stm32f7xx.c         STM32F7xx system clock configuration file
  - BSP/Src/stm32f7xx_it.c             Interrupt handlers 
  - BSP/Src/lcd.c                      LCD drawing features
  - BSP/Src/log.c                      LCD Log firmware functions
  - BSP/Src/sdram.c                    SDRAM features
  - BSP/Src/sdram_dma.c                SDRAM using DMA features
  - BSP/Src/qspi.c                     QSPI features
  - BSP/Src/eeprom.c                   EEPROM features      
  - BSP/Src/audio_play.c               Audio playback features      
  - BSP/Src/audio_rec.c                Audio record features      
  - BSP/Src/touchscreen.c              Touchscreen feature
  - BSP/Inc/main.h                     Main program header file  
  - BSP/Inc/stm32f7xx_hal_conf.h       HAL configuration file
  - BSP/Inc/stm32f7xx_it.h             Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h             lcd_log configuration template file
  - BSP/Inc/stlogo.h                   Image used for BSP example
        
        
@par Hardware and Software environment  

  - This example runs on STM32F746xx devices.
  
  - This example has been tested with STMicroelectronics STM32746G-Discovery 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
  
@par How to use it ? 

 - Use STLink utility, available on www.st.com or any other in system programming
   tool to load "BSP/Binary/audio_sample_tdm.bin" file to the STM32 internal flash 
   at the address 0x08080000.

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
     @note Make sure that the tool flash loader does not erase or overwrite the
        loaded audio file at address 0x08080000 by limiting the application
        end address to 0x0807FFFF. This is already done for the example project
 - Connect a headphone and a speaker to the audio jack connectors (CN10).


 */
