/**
  @page TIM_PrescalerSelection TIM Prescaler Selection example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_PrescalerSelection/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWM signals generation using TIM3
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

Configuration of the TIM peripheral in PWM (pulse width modulation) mode 
with clock prescaler selection feature activated using __HAL_RCC_TIMCLKPRESCALER(). 
This allows the doubling of the output frequency.

SystemCoreClock is set to 216 MHz for STM32F7xx Devices.

  In this example TIM3 input clock (TIM3CLK) is set to 4 * APB1 clock (PCLK1), since 
  Timer clock prescalers selection activated (TIMPRE bit from RCC_DCKCFGR register is set).   
  TIM3CLK = 4 * PCLK1  
  PCLK1 = HCLK / 4 
  => TIM3CLK = HCLK = SystemCoreClock

  For TIM3CLK equal to SystemCoreClock and prescaler equal to (5 - 1), TIM3 counter clock 
  is computed as follows:
  TIM3 counter clock = TIM3CLK / (Prescaler + 1)
                     = SystemCoreClock / (Prescaler + 1)
                     = 43.2MHz

  For ARR equal to (2000 - 1), the TIM3 output clock is computed as follows:
  TIM3 output clock = TIM3 counter clock / (ARR + 1)
                    = 21.6KHZ
                     
  The TIM3 CCR1 register value is equal to 1000, so the TIM3 Channel 1 generates a 
  PWM signal with a frequency equal to 21.6 KHz and a duty cycle equal to 50%:

  TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR + 1)* 100 = 50%


The PWM waveforms can be displayed using an oscilloscope.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timers, PWM, Prescaler, Duty Cycle, Waveform, Oscilloscope, Output, Signal,

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

  - TIM/TIM_PrescalerSelection/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - TIM/TIM_PrescalerSelection/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - TIM/TIM_PrescalerSelection/Inc/main.h                  Header for main.c module  
  - TIM/TIM_PrescalerSelection/Src/stm32f7xx_it.c          Interrupt handlers
  - TIM/TIM_PrescalerSelection/Src/main.c                  Main program
  - TIM/TIM_PrescalerSelection/Src/stm32f7xx_hal_msp.c     HAL MSP file
  - TIM/TIM_PrescalerSelection/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with STMicroelectronics STM32F769I-EVAL 
    board and can be easily tailored to any other supported device 
    and development board.      

  - STM32F769I-EVAL Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
        - TIM3_CH1 : PB.04



@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
