/**
  @page TIM_OnePulse TIM One Pulse example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_OnePulse/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM One Pulse example.      
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

Use of the TIM peripheral to generate a single pulse when 
an external signal rising edge is received on the timer input pin.

Clock setup for TIM4
================================

  TIM4CLK = SystemCoreClock = 216 MHz.
  
  Prescaler = (TIM4CLK /TIM4 counter clock) - 1
  
  The prescaler value is computed in order to have TIM4 counter clock 
  set at 21600000 Hz.
  
  The Autoreload value is 65535 (TIM4->ARR), so the maximum frequency value to 
  trigger the TIM4 input is 21600000/65535 [Hz].
 
Configuration of TIM4 in One Pulse Mode
===================================================
 
  - The external signal is connected to TIM4_CH2 pin (PB.07), 
    and a rising edge on this input is used to trigger the Timer.
  - The One Pulse signal is output on TIM4_CH1 (PB.06).

  The delay value is fixed to:
   - Delay =  CCR1/TIM4 counter clock 
           = 16383 / 21600000 [sec]
           
  The pulse value is fixed to : 
   - Pulse value = (TIM_Period - TIM_Pulse)/TIM4 counter clock  
                 = (65535 - 16383) / 21600000 [sec]

  The one pulse waveform can be displayed using an oscilloscope and it looks
  like this.
  
                               ____
                               |   |
  CH2 _________________________|   |__________________________________________
 
                                             ___________________________
                                            |                           |
  CH1 ______________________________________|                           |_____
                               <---Delay----><------Pulse--------------->
  


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

  - TIM/TIM_OnePulse/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - TIM/TIM_OnePulse/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - TIM/TIM_OnePulse/Inc/main.h                  Header for main.c module  
  - TIM/TIM_OnePulse/Src/stm32f7xx_it.c          Interrupt handlers
  - TIM/TIM_OnePulse/Src/main.c                  Main program
  - TIM/TIM_OnePulse/Src/stm32f7xx_hal_msp.c     HAL MSP file
  - TIM/TIM_OnePulse/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

   - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
   - In this example, the clock is set to 216 MHz.
    
  - This example has been tested with STM32F769I-EVAL board and can be
    easily tailored to any other supported device and development board.

  - STM32F769I-EVAL Set-up
   - Connect the external signal to the TIM4_CH2 pin (PB.07) ()
   - Connect the TIM4_CH1 pin(PB.06) () to an oscilloscope to monitor the waveform.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
