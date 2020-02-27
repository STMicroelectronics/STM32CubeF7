/**
  @page TIM_OutputCompare TIM example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/TIM/TIM_OutputCompare/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM_OutputCompare example.
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

Configuration of the TIM peripheral to generate an output 
waveform in different output compare modes. This example is based on the 
STM32F7xx TIM LL API. The peripheral initialization uses 
LL unitary service functions for optimization purposes (performance and size).

In this example TIM3 input clock (TIM3CLK) frequency is set to APB1 clock (PCLK1),
since APB1 pre-scaler is equal to 4 and it is four times PCLK1.
    TIM3CLK = 4*PCLK1
    PCLK1 = HCLK/4
    => TIM3CLK = (4/4)*HCLK = SystemCoreClock (216 Mhz)

To set the TIM3 counter clock frequency to 10 KHz, the pre-scaler (PSC) is
calculated as follows:
PSC = (TIM3CLK / TIM3 counter clock) - 1
PSC = (SystemCoreClock /10 KHz) - 1

SystemCoreClock is set to 216 MHz for STM32F7xx Devices.

Auto-reload (ARR) is calculated to get a time base period of 100 ms,
meaning a time base frequency of 10 Hz.
ARR = (TIM3 counter clock / time base frequency) - 1
ARR = (TIM3 counter clock / 10) - 1

The capture/compare register (CCR3) of the output channel is set to half the
auto-reload value. Therefore the timer output compare delay is 50 ms.
Generally speaking this delay is calculated as follows:
CC3_delay = TIM3 counter clock / CCR3

The timer output channel is mapped on the pin PB.0 (connected to LED1 on board
NUCLEO-F767ZI). Thus LED1 status (on/off) mirrors the timer output
level (active v.s. inactive).

User push-button can be used to change the output compare mode:
  - When the output channel is configured in output compare toggle:  LED1 
    TOGGLES when the counter (CNT) matches the capture/compare register (CCR3).
  - When the output channel is configured in output compare active:  LED1 is
    switched ON when the counter (CNT) matches the capture/compare register
    (CCR3).
  - When the output channel is configured in output compare inactive:  LED1 is
    switched OFF when the counter (CNT) matches the capture/compare register
    (CCR3).
    
Initially the output channel is configured in output compare toggle mode.

@par Keywords

Timers, Output, Compare, signal, Oscilloscope, Frequency, Duty cycle, Waveform

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

  - TIM/TIM_OutputCompare/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - TIM/TIM_OutputCompare/Inc/main.h                  Header for main.c module
  - TIM/TIM_OutputCompare/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_OutputCompare/Src/stm32f7xx_it.c          Interrupt handlers
  - TIM/TIM_OutputCompare/Src/main.c                  Main program
  - TIM/TIM_OutputCompare/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx devices.
    
  - This example has been tested with NUCLEO-F767ZI board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
