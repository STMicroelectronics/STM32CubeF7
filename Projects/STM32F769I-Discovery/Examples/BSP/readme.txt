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
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 200 MHz.

This example shows how to use the different functionalities of components 
available on the board by switching between all tests using key button. 

The program starts by turning ON GREEN. RED LED is ON when an error occurs.

 ** Push the User button to start first Test.  

 ** Touchscreen **
Use touchscreen functionality to select or activate colored circle inside a rectangle.
Then handle touchscreen in polling mode to detect single and dual touches.

 ** AUDIO PLAY **
This example show how to play an audio file using the DMA circular mode and 
how to handle the buffer update.
Plug a headphone to hear the sounds. On the left ear plug, user will hear the sound track "Hello man!",
while on the right ear plug, a recurring Beeping sound is heard. RED and GREEN Leds
will blink while the test is running.
@Note: Copy file 'audio_sample_tdm.bin' (available in Binary) directly in 
       the flash at @0x08080000 using ST-Link utility

 ** AUDIO RECORD **
This example show how to record an audio stream from microphnes mounted on STM32F769I-Discovery
and play it using the DMA circular mode and how to handle the buffer update.
Plug a headphone to ear the sound.

 ** LCD **
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

 ** SD **
This example shows how to erase, write and read in SD card.

 ** QSPI **
This example shows how to erase, write and read in Quad SPI Flash Macronix MX25L51245G.

 ** SDRAM **
This example shows how to erase, write and read the SDRAM memory.

 ** LCD LOG **
This example show how to use the LCD log features. 

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

  - BSP/Src/main.c                 Main program
  - BSP/Src/system_stm32f7xx.c     STM32F7xx system clock configuration file
  - BSP/Src/stm32f7xx_it.c         Interrupt handlers 
  - BSP/Src/lcd.c                  LCD drawing features
  - BSP/Src/log.c                  LCD Log firmware functions
  - BSP/Src/sd.c                   SD features
  - BSP/Src/audio_play.c           Play audio 
  - BSP/Src/audio_loopback.c       Record and Play audio  
  - BSP/Src/qspi.c                 QSPI Read/Write features
  - BSP/Src/sdram.c                SDRAM Read/Write features  
  - BSP/Inc/main.h                 Main program header file  
  - BSP/Inc/stm32f7xx_hal_conf.h   HAL Configuration file
  - BSP/Inc/stm32f7xx_it.h         Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h         lcd_log configuration template file
  - BSP/Inc/stlogo.h               Image used for BSP example
        
@par Hardware and Software environment  

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with STMicroelectronics STM32F769I-Discovery
    boards and can be easily tailored to any other supported device 
    and development board.
  
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
