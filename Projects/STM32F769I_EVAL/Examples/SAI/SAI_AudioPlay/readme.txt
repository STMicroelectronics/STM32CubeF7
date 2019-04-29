/**
  @page SAI_AudioPlay  Description of the SAI audio play example
  
  @verbatim
  ******************************************************************************
  * @file    SAI/SAI_AudioPlay/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SAI audio play example.
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

Use of the SAI HAL API to play an audio file in DMA circular mode and handle the buffer update.

      Plug a headphone to ear the sound  /!\ Take care of yours ears.
      Default volume is 20%.
      The audio file is played in loop
      @Note: Copy file 'audio.bin' (available in AudioFile) directly in the flash 
      at @0x08010000 using ST-Link utility
Once started, LED1 is toggling on STM32F769I-EVAL.

@note Note the DMA runs in circular buffer mode and never stops. If you break with 
      the debugger, the DMA hw will keep running and a noise will be heard.

@note This example does not use BSP_AUDIO so the MspInit is coded in the main.c.

@par Keywords

Audio, SAI, DMA, Buffer update, play, headphone, audio protocol

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

  - SAI/SAI_AudioPlay/Src/main.c                  Main program
  - SAI/SAI_AudioPlay/Src/system_stm32f7xx.c      STM32F7xx system source file
  - SAI/SAI_AudioPlay/Src/stm32f7xx_it.c          Interrupt handlers
  - SAI/SAI_AudioPlay/Inc/main.h                  Main program header file
  - SAI/SAI_AudioPlay/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - SAI/SAI_AudioPlay/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - SAI/SAI_AudioPlay/AudioFile/audio.bin         Audio wave extract.

@par Hardware and Software environment

  - This example runs on STM32F769xx/STM32F779xx/STM32F777xx/STM32F767xx devices.
    
  - This example has been tested with STMicroelectronics STM32F769I-EVAL
    board and can be easily tailored to any other supported device
    and development board.      

  - STM32F769I-EVAL Set-up :
    - Make sure that JP3 is in position 1-2.
    - Plug headset on CN28 connector. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
