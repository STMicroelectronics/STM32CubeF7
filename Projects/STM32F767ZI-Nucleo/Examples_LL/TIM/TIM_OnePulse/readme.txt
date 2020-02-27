/**
  @page TIM_OnePulse TIM example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/TIM/TIM_OnePulse/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM_OnePulse example.
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

Configuration of a timer to generate a positive pulse in 
Output Compare mode with a length of tPULSE and after a delay of tDELAY. This example 
is based on the STM32F7xx TIM LL API. The peripheral initialization uses 
LL unitary service functions for optimization purposes (performance and size).


The pulse is generated on OC3.

This example uses 2 timer instances:

  - TIM1 generates a positive pulse of 50 us after a delay of 50 us. User push-button
    is used to start TIM1 counter. 
                                              ___ 
                                             |   |
    User push-button ________________________|   |________________________________
                                                            ___________
                                                           |           |
    OC1              ______________________________________|           |________
    (TIM1_CH1)                                <---50 us---><---50 us--->
                                                |            |_ uwMeasuredPulseLength
                                                |_ uwMeasuredDelay

  TIM1_CH1 delay and pulse length are measured every time a pulse is generated. 
  Both can be observed through the debugger by monitoring the variables uwMeasuredDelay and
  uwMeasuredPulseLength respectively.

  - TIM3 generates a positive pulse of 3 s after a delay of 2 s. TIM3 counter start
    is controlled through the slave mode controller. TI2FP2 signals is selected as
    trigger input meaning that TIM3 counter starts when a rising edge is detected on
    TI2FP2. The TIM3 output channel is mapped on the pin PB.0 (connected to LED1 on board
    NUCLEO-F767ZI). Thus LED1 status (on/off) mirrors the timer output level (active v.s. inactive).
                                  ___
                                 |   |
    TI2 _________________________|   |_________________________________________
    (TIM3_CH2)
                                               ___________________________
                                              |                           |
    OC3 ______________________________________|                           |____
    (TIM3_CH3)                   <-----2s-----><----------3 s------------->
   


Both TIM1 and TIM3 are configured to generate a single pulse (timer counter 
stops automatically at the next update event (UEV).

Connecting TIM1 OC1 to TIM3 TI2 allows to trigger TIM3 counter by pressing
the User push-button.

@par Keywords

Timers, Output, signals, One Pulse, PWM, Oscilloscope, External signal, Autoreload, Waveform

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

  - TIM/TIM_OnePulse/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - TIM/TIM_OnePulse/Inc/main.h                  Header for main.c module
  - TIM/TIM_OnePulse/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_OnePulse/Src/stm32f7xx_it.c          Interrupt handlers
  - TIM/TIM_OnePulse/Src/main.c                  Main program
  - TIM/TIM_OnePulse/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx devices.
    
  - This example has been tested with NUCLEO-F767ZI board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F767ZI Set-up:
      - TIM1_CH1  PE.09: connected to pin 4 of CN10 connector  
      - TIM3_CH3  PB.00: connected to pin 31 of CN10 connector 
      - TIM3_CH2  PB.05: connected to pin 13 of CN7 connector 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
