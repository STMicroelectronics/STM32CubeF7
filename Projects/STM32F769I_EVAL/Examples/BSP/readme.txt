/**
  @page BSP  Example on how to use the BSP drivers
  
  @verbatim
  ******************************************************************************
  * @file    BSP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
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

This example provides a description of how to use the different BSP drivers. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 200 MHz.

This example shows how to use the different functionalities of components 
available on the board by switching between all tests Wakeup/Tamper button. 

The program starts by turning ON GREEN, ORANGE and BLUE LEDs. RED LED is ON when
an error occurs.

 ** Push the Wakeup/Tamper button to start first Test.  

 ** AUDIO PLAY **
This example show how to play an audio file using the DMA circular mode and 
how to handle the buffer update.
Plug a headphone to ear the sound 
@Note: Copy file 'audio_sample_tdm.bin' (available in Binary) directly in 
       the flash at @0x08080000 using ST-Link utility

 ** AUDIO RECORD **
This example show how to record an audio stream from microphnes mounted on STM32F769I-EVAL
and play it using the DMA circular mode and how to handle the buffer update.
Plug a headphone to ear the sound. Make sure JP7 is plugged.  

 ** JOYSTICK **
Use the joystick button to move a pointer inside a rectangle 
(up/down/right/left) and change the pointer color(select).

 ** Touchscreen demo1 **
Use touchscreen functionality to select or activate colored circle inside a rectangle.

 ** Touchscreen demo2 **
Use touchscreen functionality to select 2 touch points, their positions will be
captured and displayed on the LCD.

 ** LCD **
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

** SD **
This example shows how to erase, write and read the SD cards and also 
how to detect the presence of the cards(SD1/SD2). 

 ** EEPROM **
This example show how to read and write data in EEPROM I2C M24LR64 connected on STM327x6G-EVAL revB
   * The I2C RF EEPROM memory (M24LR64) is available through the connector CN2

 ** NOR **
This example shows how to erase, write and read the NOR memory.

 ** SRAM **
This example shows how to erase, write and read the SRAM memory.

 ** SRAM DMA **
This example shows how to erase, write and read the SRAM memory in DMA mode.

 ** SDRAM **
This example shows how to erase, write and read the SDRAM memory.

 ** SDRAM DMA **
This example shows how to erase, write and read the SDRAM memory in DMA mode.

 ** LCD LOG **
This example show how to use the LCD log features. 

 ** QSPI test **
This example provides of how to write, read and buffers compare for external 
flash memory using QSPI communication.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.

@par Keywords

BSP, Board support package, DMA, Audio play, Headphone, Audio record, microphone, Touchscreen, Joystick,
LCD, SD Card, EEPROM, NOR, SRAM, SDRAM, LOG, QSPI

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
  - BSP/Src/joystick.c             joystick feature
  - BSP/Src/lcd.c                  LCD drawing features
  - BSP/Src/log.c                  LCD Log firmware functions
  - BSP/Src/eeprom.c               EEPROM Read/Write features
  - BSP/Src/audio_play.c           Play audio 
  - BSP/Src/audio_record.c         Record and Play audio
  - BSP/Src/sram.c                 SRAM Read/Write features
  - BSP/Src/sdram.c                SDRAM Read/Write features
  - BSP/Src/sdram_dma.c            SDRAM Read/Write DMA mode
  - BSP/Src/nor.c                  NOR Read/Write features 
  - BSP/Src/qspi.c                 QUADSPI Read/Write features   
  - BSP/Inc/main.h                 Main program header file  
  - BSP/Inc/stm32f7xx_hal_conf.h   HAL Configuration file
  - BSP/Inc/stm32f7xx_it.h         Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h         lcd_log configuration template file
  - BSP/Inc/stlogo.h               Image used for BSP example
        
@par Hardware and Software environment  

  - This example runs on STM32F769xx/STM32F779xx devices.
    
  - This example has been tested with STMicroelectronics STM32F769I-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    
  - STM32F769I-EVAL Set-up :
   For AUDIO RECORD Module:
    - Make sure that JP3 is in position 1-2.
    - Make sure that JP23 is in position 2-3.
    - Make sure that JP24 is in position 2-3.
    - Make sure that JP7 is in position 2-3.
    - Plug headset on CN28 connector.
   For AUDIO PLAY Module:
    - Make sure that JP3 is in position 1-2.
    - Plug headset on CN28 connector.
   For NOR Module:
    - Make sure that JP10 is fitted.
   For SD module:
    - Make sure that JP7 is in position 1-2.
  
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
