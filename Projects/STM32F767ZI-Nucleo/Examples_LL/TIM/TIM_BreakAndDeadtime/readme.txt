/**
  @page TIM_BreakAndDeadtime TIM example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/TIM/TIM_BreakAndDeadtime/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM_BreakAndDeadtime example.
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

Configuration of the TIM peripheral to
  – generate three center-aligned PWM and complementary PWM signals
  – insert a defined dead time value
  – use the break feature
  – lock the desired parameters
This example is based on the STM32F7xx TIM LL API;
peripheral initialization is done using LL unitary services
functions for optimization purpose (performance and size).

TIM1CLK is fixed to 216 MHz, the TIM1 Prescaler is set to have
TIM1 counter clock = 10 MHz.

The TIM1 auto-reload is set to generate PWM signal at 10 KHz:
               
The Three Duty cycles are computed as the following description: 
The channel 1 duty cycle is set to 50% so channel 1N is set to 50%.
The channel 2 duty cycle is set to 25% so channel 2N is set to 75%.
The channel 3 duty cycle is set to 12.5% so channel 3N is set to 87.5%.

A dead time equal to 4 us is inserted between 
the different complementary signals, and the Lock level 1 is selected.
  - The OCx output signal is the same as the reference signal except for the rising edge,
    which is delayed relative to the reference rising edge.
  - The OCxN output signal is the opposite of the reference signal except for the rising
    edge, which is delayed relative to the reference falling edge

Note that calculated duty cycles apply to the reference signal (OCxREF) from
which outputs OCx and OCxN are generated. As dead time insertion is enabled the
duty cycle measured on OCx will be slightly lower.
  
The break Polarity is used at High level.

The TIM1 waveforms can be displayed using an oscilloscope.

@par Keywords

Timers, Break, Dead time, PWM, Frequency, Duty Cycle, Waveform, Oscilloscope, Output, Signal

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

  - TIM/TIM_BreakAndDeadtime/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - TIM/TIM_BreakAndDeadtime/Inc/main.h                  Header for main.c module
  - TIM/TIM_BreakAndDeadtime/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_BreakAndDeadtime/Src/stm32f7xx_it.c          Interrupt handlers
  - TIM/TIM_BreakAndDeadtime/Src/main.c                  Main program
  - TIM/TIM_BreakAndDeadtime/Src/system_stm32f7xx.c      STM32F7xx system source file

@par Hardware and Software environment

  - This example runs on STM32F767xx devices.
    
  - This example has been tested with NUCLEO-F767ZI board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F767ZI Set-up
    - Connect the TIM1 pins to an oscilloscope to monitor the different waveforms:
      - TIM1_CH1  PE.09: connected to pin 4 of CN10 connector  
      - TIM1_CH1N PE.08: connected to pin 18 of CN10 connector 
      - TIM1_CH2  PE.11: connected to pin 6 of CN10 connector 
      - TIM1_CH2N PE.10: connected to pin 24 of CN10 connector 
      - TIM1_CH3  PE.13: connected to pin 10 of CN10 connector 
      - TIM1_CH3N PE.12: connected to pin 26 of CN10 connector 

    - Connect the TIM1 break to the GND. To generate a break event, switch this
      pin level from 0V to 3.3V.  
      - TIM1_BKIN  PE.15: connected to pin 30 of CN10 connector 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 