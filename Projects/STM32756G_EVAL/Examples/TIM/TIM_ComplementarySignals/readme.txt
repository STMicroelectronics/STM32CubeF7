/**
  @page TIM_ComplementarySignals TIM Complementary Signals example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_ComplementarySignals/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM Complementary Signals example.
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

Configuration of the TIM1 peripheral to generate three 
complementary signals, insert a predefined deadtime value, use the break 
feature, and lock the break and dead-time configuration.

TIM1CLK is fixed to SystemCoreClock, the TIM1 Prescaler is set to have
TIM1 counter clock = 21.6MHz.

The objective is to generate PWM signal at 10 KHz:
  - TIM1_Period = (TIM1 counter clock / 10000) - 1
               
The Three Duty cycles are computed as the following description: 
The channel 1 duty cycle is set to 50% so channel 1N is set to 50%.
The channel 2 duty cycle is set to 25% so channel 2N is set to 75%.
The channel 3 duty cycle is set to 12.5% so channel 3N is set to 87.5%.
The Timer pulse is calculated as follows:
  - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100

A dead time equal to 100/SystemCoreClock (arround 0.5us) is inserted between 
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



@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timers, PWM, Complementary signals, Master, Slave, Duty Cycle, Waveform, Oscilloscope, Output, Signal,
dead time, break lock,

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

  - TIM/TIM_ComplementarySignals/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - TIM/TIM_ComplementarySignals/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - TIM/TIM_ComplementarySignals/Inc/main.h                  Header for main.c module  
  - TIM/TIM_ComplementarySignals/Src/stm32f7xx_it.c          Interrupt handlers
  - TIM/TIM_ComplementarySignals/Src/main.c                  Main program
  - TIM/TIM_ComplementarySignals/Src/stm32f7xx_hal_msp.c     HAL MSP file
  - TIM/TIM_ComplementarySignals/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F756xx/STM32F746xx devices.
    
  - This example has been tested with STMicroelectronics STM327x6G-EVAL revB
    board and can be easily tailored to any other supported device
    and development board.      

  - STM327x6G-EVAL revB Set-up
    - Connect the TIM1 pins to an oscilloscope to monitor the different waveforms:
      - TIM1_CH1  pin (PA.08)  
      - TIM1_CH1N pin (PB.13)  
      - TIM1_CH2  pin (PA.09)  
      - TIM1_CH2N pin (PB.14)  
      - TIM1_CH3  pin (PA.10)  
      - TIM1_CH3N pin (PB.15)

    - Connect the TIM1 break pin TIM1_BKIN pin (PB.12) to the GND. To generate a 
      break event, switch this pin level from 0V to 3.3V.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
