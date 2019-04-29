/**
  @page TIM_CascadeSynchro Timers Synchronization example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_CascadeSynchro/readme.txt 
  * @author  MCD Application Team
  * @brief   How to command 2 Timers as slaves (TIM3 & TIM4) using a Timer
  *          as master (TIM2)

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

Synchronization of TIM2 and TIM3/TIM4 timers in Cascade mode.

Timers synchronisation in cascade mode:
   ___________                     ___________              ___________                 
  |   MASTER  |TRGO_Update   ITR1 |  SLAVE 1  |       ITR2 |  SLAVE 2  |                 
  |    TIM2   |-------------------|    TIM3   |------------|    TIM4   |               
  |___________|                   |___________|            |___________| 
                                 

1/ TIM2 is configured as Master Timer:
   - PWM Mode is used
   - The TIM2 Update event is used as Trigger Output 
 
2)TIM3 is slave for TIM2 and Master for TIM4,
  - PWM Mode is used
  - The ITR1(TIM2) is used as input trigger 
  - Gated mode is used, so start and stop of slave counter
    are controlled by the Master trigger output signal(TIM2 update event).
  - The TIM3 Update event is used as Trigger Output.

3)TIM4 is slave for TIM3,
  - PWM Mode is used
  - The ITR2(TIM3) is used as input trigger
  - Gated mode is used, so start and stop of slave counter  are controlled by the
    Master trigger output signal(TIM3 update event).

The TIM2 counter clock is 216 MHz.

  The Master Timer TIM2 is running at:
  TIM2 frequency = TIM2 counter clock / (TIM2_Period + 1) = 421.875 KHz and
  a the duty cycle equal to: TIM2_CCR1/(TIM2_ARR + 1) = 25%

  The TIM3 is running at: 
  (TIM2 frequency)/ (TIM3 period +1) = 105.468 KHz and
  a duty cycle equal to TIM3_CCR1/(TIM3_ARR + 1) = 25%

  The TIM4 is running at:
  (TIM3 frequency)/ (TIM4 period +1) = 26.367 KHz and
  a duty cycle equal to TIM4_CCR1/(TIM4_ARR + 1) = 25%


The PWM waveform can be displayed using an oscilloscope.

@par Keywords

Timers, PWM, Cascade Synchronization, Master, Slave, Duty Cycle, Waveform, Oscilloscope, Output, Signal

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

  - TIM/TIM_CascadeSynchro/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - TIM/TIM_CascadeSynchro/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - TIM/TIM_CascadeSynchro/Inc/main.h                  Header for main.c module  
  - TIM/TIM_CascadeSynchro/Src/stm32f7xx_it.c          Interrupt handlers
  - TIM/TIM_CascadeSynchro/Src/main.c                  Main program
  - TIM/TIM_CascadeSynchro/Src/stm32f7xx_hal_msp.c     HAL MSP file
  - TIM/TIM_CascadeSynchro/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with STM32F769I-EVAL board and can be
    easily tailored to any other supported device and development board.      

  - STM32F769I-EVAL Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM2 CH1 (PA.00)
      - TIM3 CH1 (PC.06)
      - TIM4 CH1 (PB.06)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
