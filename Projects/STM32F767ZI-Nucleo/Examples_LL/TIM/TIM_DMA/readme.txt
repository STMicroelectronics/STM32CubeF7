/**
  @page TIM_DMA TIM example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/TIM/TIM_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM_DMA example.
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

Use of the DMA with a timer update request 
to transfer data from memory to Timer Capture Compare Register 3 (TIM3_CCR3). This 
example is based on the STM32F7xx TIM LL API. The peripheral initialization 
uses LL unitary service functions for optimization purposes (performance and size).

The following configuration values are used in this example:
  - TIM3CLK = SystemClock
  - Counter repetition = 3 
  - Prescaler = 0 
  - TIM3 counter clock = SystemClock
  - SystemCoreClock is set to 216 MHz for STM32F7xx Devices.

The objective is to configure TIM3 channel 3 to generate PWM edge aligned 
signal with a frequency equal to 17.57 KHz, and a variable duty cycle that
is changed by the DMA after a specific number of Update DMA request.

The number of this repetitive requests is defined by the TIM3 Repetition counter,
each 4 Update Requests, the TIM3 Channel 3 Duty Cycle changes to the next new 
value defined by the aCCValue_Buffer.

The PWM waveform can be displayed using an oscilloscope.

Whenever a DMA transfer fails LED2 flashes with a frequency of 1 Hz.
 
@note PWM signal frequency value mentioned above is theoretical (obtained when
      the system clock frequency is exactly 80 MHz). Since the generated system
      clock frequency may vary from one board to another observed PWM signal
      frequency might be slightly different.

@par Keywords

Timers, DMA, PWM, Frequency, Duty Cycle, Waveform, Oscilloscope, Output, Signal

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

  - TIM/TIM_DMA/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - TIM/TIM_DMA/Inc/main.h                  Header for main.c module
  - TIM/TIM_DMA/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_DMA/Src/stm32f7xx_it.c          Interrupt handlers
  - TIM/TIM_DMA/Src/main.c                  Main program
  - TIM/TIM_DMA/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx devices.
    
  - This example has been tested with NUCLEO-F767ZI board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F767ZI Set-up
    - Connect the TIM3 pins to an oscilloscope to monitor the different waveforms:
      - TIM3_CH3  PB.00: connected to pin 31 of CN10 connector 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
