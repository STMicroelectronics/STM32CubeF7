/**
  @page PWR_STOP PWR Example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_MIX/PWR/PWR_STOP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR_STOP example.
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
 
How to enter the system in STOP with Low power regulator mode and wake up from this mode by using external 
reset or wakeup interrupt (all the RCC function calls use RCC LL API 
for minimizing footprint and maximizing performance).

In the associated software, the system clock is set to 216 MHz, an EXTI line
is connected to the user button through PC.13 and configured to generate an 
interrupt on falling edge upon key press.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick 
interrupt handler, LED1 is toggled in order to indicate whether the MCU is in STOP with Low power regulator mode 
or RUN mode.

5 seconds after start-up, the system automatically enters STOP with Low power regulator mode and 
LED1 stops toggling.

The User push-button can be pressed at any time to wake-up the system. 
The software then comes back in RUN mode for 5 sec. before automatically entering STOP with Low power regulator mode again. 

Two leds LED1 and LED2 are used to monitor the system state as following:
- LED2 ON: configuration failed (system will go to an infinite loop)
- LED1 fast toggling: system in RUN mode
- LED1 off : system in STOP with Low power regulator mode
- LED1 slow toggling: system in RUN mode after exiting from Stop mode

These steps are repeated in an infinite loop.

@note To measure the current consumption in STOP with Low power regulator mode, remove JP5 jumper 
      and connect an amperemeter to JP5 to measure IDD current.     

@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M7 core is no longer clocked during low power mode 
      so debugging features are disabled.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@par Keywords

Power, PWR, Stop mode, Interrupt, EXTI, Wakeup, Low Power, External reset,

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

  - Examples_MIX/PWR/PWR_STOP/Inc/stm32f7xx_conf.h         HAL Configuration file
  - Examples_MIX/PWR/PWR_STOP/Inc/stm32f7xx_it.h           Header for stm32f7xx_it.c
  - Examples_MIX/PWR/PWR_STOP/Inc/main.h                   Header file for main.c
  - Examples_MIX/PWR/PWR_STOP/Src/system_stm32f7xx.c       STM32F7xx system clock configuration file
  - Examples_MIX/PWR/PWR_STOP/Src/stm32f7xx_it.c           Interrupt handlers
  - Examples_MIX/PWR/PWR_STOP/Src/main.c                   Main program
  - Examples_MIX/PWR/PWR_STOP/Src/stm32f7xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32F7xx devices
    

  - This example has been tested with STMicroelectronics NUCLEO-F767ZI
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-F767ZI set-up:
    - Use LED1 and LED2 connected respectively to PB.0 and PB.07 pins
    - User push-button connected to pin PC.13 (EXTI_Line15_10)
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
