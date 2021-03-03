/**
  @page TIM_OCToggle TIM_OCToggle example
  
  @verbatim
  ******************************************************************************
  * @file    TIM/TIM_OCToggle/readme.txt 
  * @author  MCD Application Team
  * @brief   This example shows how to configure the Timer to generate four different 
  *          signals with four different frequencies.
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

Configuration of the TIM peripheral to generate four different 
signals at four different frequencies.

  The TIM3 frequency is set to SystemCoreClock, and the objective is
  to get TIM3 counter clock at 18 MHz so the Prescaler is computed as following:
     - Prescaler = (TIM3CLK /TIM3 counter clock) - 1
   
  SystemCoreClock is set to 216 MHz for STM32F7xx Devices.

  The TIM3 CCR1 register value is equal to 46153:
  CC1 update rate = TIM3 counter clock / CCR1_Val = 390 Hz,
  so the TIM3 Channel 1 generates a periodic signal with a frequency equal to 195 Hz.

  The TIM3 CCR2 register value is equal to 23076:
  CC2 update rate = TIM3 counter clock / CCR2_Val = 780 Hz,
  so the TIM3 channel 2 generates a periodic signal with a frequency equal to 390 Hz.

  The TIM3 CCR3 register value is equal to 11538:
  CC3 update rate = TIM3 counter clock / CCR3_Val = 1560 Hz,
  so the TIM3 channel 3 generates a periodic signal with a frequency equal to 780 Hz.

  The TIM3 CCR4 register value is equal to 5769:
  CC4 update rate = TIM3 counter clock / CCR4_Val =  3120 Hz,
  so the TIM3 channel 4 generates a periodic signal with a frequency equal to 1560 Hz.
  
  - LED1 is always ON because is configured on PB.00.
  - LED3 is ON when an error occurs.
  

@note PWM signal frequency values mentioned above are theoretical (obtained when the system clock frequency
      is exactly 216 MHz). Since the generated system clock frequency may vary from one board to another observed
      PWM signal frequency might be slightly different.
	  
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Timers, Output, signals, Output compare toggle, PWM, Oscilloscope

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

  - TIM/TIM_OCToggle/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - TIM/TIM_OCToggle/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - TIM/TIM_OCToggle/Inc/main.h                  Header for main.c module  
  - TIM/TIM_OCToggle/Src/stm32f7xx_it.c          Interrupt handlers
  - TIM/TIM_OCToggle/Src/main.c                  Main program
  - TIM/TIM_OCToggle/Src/stm32f7xx_hal_msp.c     HAL MSP file
  - TIM/TIM_OCToggle/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F746xG devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-746ZG Rev.B
    Rev.B board and can be easily tailored to any other supported device 
    and development board.      

  - NUCLEO-746ZG Rev.B board Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
      - PB.04: (TIM3_CH1) (connected to PB.04 (pin 19 in CN7 connector))
      - PB.05: (TIM3_CH2) (connected to PB.05 (pin 13 in CN7 connector))
      - PB.00: (TIM3_CH3) (connected to PB.00 (pin 31 in CN10 connector))
      - PB.01: (TIM3_CH4) (connected to PB.01 (pin 7 in CN10 connector))

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
