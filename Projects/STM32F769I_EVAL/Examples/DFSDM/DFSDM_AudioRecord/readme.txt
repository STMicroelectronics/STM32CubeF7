/**
  @page DFSDM_AudioRecord  Description of the DFSDM audio record example
  
  @verbatim
  ******************************************************************************
  * @file    DFSDM/DFSDM_AudioRecord/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DFSDM audio record example.
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

This example shows how to use the DFSDM HAL API to perform stereo audio recording.
For this audio record we will use two MP34DT01 digital microphones mounted on the board. 

MP34DT01 runs with a clock frequency from 1MHz to 3,25MHz.
This clock frequency has to be delivered by DFSDM. So first channel of DFSDM will be
configurated to enable output clock generation, to choose audio clock as output clock source,
and output clock divider will be set to generate output clock on MP34DT01 frequency clock range.
Audio clock will be configurated to 11.294MHz, so DFSDM output clock divider will be set to 4.

The digital audio outputs from the microphones are coded in PDM (Pulse Density Modulation) and 
interlaced to use the same data wire connected to PD6. When CLK = 0, the left channel is sent
on PD6 and when CLK = 1, the right channel is sent on PD6.

PD6 can be configurated as DFSDM_DATIN1 (DFSDM channel 1 external data input).
We will use DFSDM channel 1 to get left channel. DFSDM channel 1 will be configurated to take
inputs from external serial inputs and from pins of channel 1. DFSDM channel 1 will also be 
configurated to operate in SPI mode with rising edge and with internal clock.
We will use also DFSDM channel 0 to get right channel. DFSDM channel 0 will be configurated to take
inputs from external serial inputs and from pins of channel 1. DFSDM channel 0 will also be 
configurated to operate in SPI mode with falling edge and with internal clock.

DFSDM filter 0 will be used to perform left channels conversions.
We will use a SINC3 filter with oversampling at 64 and integrator oversampling at 1.
Oversampling is set to 64 in order to have audio sample frequency at 44.1KHz (11.294MHz/(4*64)).
Regular conversions in fast mode will be performed in DMA mode on DFSDM filter 0 in order to fill
a buffer containing left audio samples at 44.1KHz.
DFSDM filter 1 will be used to perform right channels conversions.
We will use a SINC3 filter with oversampling at 64 and integrator oversampling at 1.
Oversampling is set to 64 in order to have audio sample frequency at 44.1KHz (11.294MHz/(4*64)).
Regular conversions in fast mode and synchronized with DFSDM filter 0 regular conversions will be 
performed in DMA mode on DFSDM filter 1 in order to fill a buffer containing right audio samples at 44.1KHz.

Playback of the recorded data will be performed on headset using HAL_SAI and WM8994 audio codec.
A circular playback buffer will be filled as soon as recorded data are available. 
When half left and right buffers will be filled, we put first parts of left and right channels data 
on first half of playback buffer. 
When left and right buffers will be full filled, we put second parts of left and right channels data 
on second half of playback buffer.

STM32F769I-EVAL board's LEDs are used to monitor the example status:
  - LED3 toggle with a period of one second when  there is an error occurred.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Audio, DFSDM, Audio record, Digital Microphone, Stereo, MP34DT01, DMA, PDM, Filter, Playback, Audio codec, SAI

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

  - DFSDM/DFSDM_AudioRecord/Src/main.c                  Main program
  - DFSDM/DFSDM_AudioRecord/Src/system_stm32f7xx.c      STM32F7xx system source file
  - DFSDM/DFSDM_AudioRecord/Src/stm32f7xx_it.c          Interrupt handlers
  - DFSDM/DFSDM_AudioRecord/Inc/main.h                  Main program header file
  - DFSDM/DFSDM_AudioRecord/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - DFSDM/DFSDM_AudioRecord/Inc/stm32f7xx_it.h          Interrupt handlers header file


@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with STMicroelectronics STM32F769I-EVAL
    board and can be easily tailored to any other supported device
    and development board.      

  - STM32F769I-EVAL Set-up :
    - Make sure that JP3 is in position 1-2.
    - Make sure that JP23 is in position 2-3.
    - Make sure that JP24 is in position 2-3.
    - Make sure that JP7 is in position 2-3.
    - Plug headset on CN28 connector.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
