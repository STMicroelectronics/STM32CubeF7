/**
  @page TIM_7PWMOutput TIM Complementary Signals example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_7PWMOutput/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWM signals generation using Timer Example.
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

Configuration of the TIM1 peripheral to generate seven PWM signals 
with four different duty cycles (50%, 37.5%, 25% and 12.5%).

TIM1CLK is fixed to SystemCoreClock, the TIM1 Prescaler is set to 0 to have
TIM1 counter clock = SystemCoreClock.

The objective is to generate 7 PWM signals at 17.57kHz:
  - TIM1_Period = (TIM1 counter clock / 17570) - 1
               
The channel 1 and channel 1N duty cycle is set to 50%
The channel 2 and channel 2N duty cycle is set to 37.5%
The channel 3 and channel 3N duty cycle is set to 25%
The channel 4 duty cycle is set to 12.5%
The Timer pulse is calculated as follows:
  
    - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
  
The TIM1 waveforms can be displayed using an oscilloscope.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timers, PWM, Duty Cycle, Waveform, Oscilloscope, Output, Signal, commutation, timing mode

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

  - TIM/TIM_7PWMOutput/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - TIM/TIM_7PWMOutput/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - TIM/TIM_7PWMOutput/Inc/main.h                  Header for main.c module  
  - TIM/TIM_7PWMOutput/Src/stm32f7xx_it.c          Interrupt handlers
  - TIM/TIM_7PWMOutput/Src/main.c                  Main program
  - TIM/TIM_7PWMOutput/Src/stm32f7xx_hal_msp.c     HAL MSP file
  - TIM/TIM_7PWMOutput/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with STMicroelectronics STM32F769I-EVAL
    board and can be easily tailored to any other supported device
    and development board.      

  - STM32F769I-EVAL Set-up
    - Connect the TIM1 pins to an oscilloscope to monitor the different waveforms:
      - TIM1_CH1  pin (PA.08)  
      - TIM1_CH1N pin (PB.13)  
      - TIM1_CH2  pin (PA.09)  
      - TIM1_CH2N pin (PB.14)  
      - TIM1_CH3  pin (PA.10)  
      - TIM1_CH3N pin (PB.15)
      - TIM1_CH4  pin (PE.14) 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
