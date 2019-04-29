/**
  @page PWR_STOP Power Stop Mode Example
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_STOP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Power Stop Mode example.
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
 
How to enter the Stop mode and wake up from this mode by using the RTC wakeup 
timer event or an interrupt.

In the associated software
  - the system clock is set to 216 MHz
  - the EXTI15_10 is configured to generate interrupt on falling edge
  - the EXTI_Line22 connected internally to the RTC Wakeup event is configured
    to generate an interrupt on rising edge each 4s
  - the SysTick is programmed to generate an interrupt each 1 ms 

Initially LED4 is turned ON. 
The system enters STOP mode after 5 seconds and will wait for the RTC Wakeup event to be 
generated each 4s, or Tamper push-button is pressed. 
 - If the RTC WakeUp event (EXTI_Line22) is the source of wakeup from STOP, LED1 is ON.
 - If the Tamper push-button (EXTI15_10) is the source of wakeup from STOP, LED2 is ON.

This behavior is repeated in an infinite loop.

LEDs are used to monitor the system state as following:
 - LED1 and LED4 ON: system woken up from STOP using RTC WakeUp Interrupt
 - LED2 and LED4 ON: system woken up from STOP using EXTI15_10 (Tamper push-button)
 - LED4 ON: system in RUN mode
 - LED3 ON: Initialization error (RTC, RCC,...)

@note The system could be entered to STOP mode with the under drive mode.
      To enable the STOP mode with the under drive mode, the "#define UNDERDRIVE_MODE"
      in the "main.c" file must be uncommented.

@note This example can not be used in DEBUG mode, this is due to the fact 
      that the Cortex-M7 core is no longer clocked during low power mode 
      so debugging features are disabled

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select the RTC clock source; in this 
       case the Backup domain will be reset in order to modify the RTC Clock source, as consequence RTC  
       registers (including the backup registers) and RCC_BDCR register are set to their reset values.

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

  - PWR/PWR_STOP/Inc/stm32f7xx_hal_conf.h         HAL Configuration file
  - PWR/PWR_STOP/Inc/stm32f7xx_it.h           Header for stm32f7xx_it.c
  - PWR/PWR_STOP/Inc/main.h                   Header file for main.c
  - PWR/PWR_STOP/Src/system_stm32f7xx.c       STM32F7xx system clock configuration file
  - PWR/PWR_STOP/Src/stm32f7xx_it.c           Interrupt handlers
  - PWR/PWR_STOP/Src/main.c                   Main program
  - PWR/PWR_STOP/Src/stm32f7xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32F756xx/STM32F746xx devices

  - This example has been tested with STMicroelectronics STM327x6G-EVAL revB
    evaluation board and can be easily tailored to any other supported device 
    and development board.

  - STM327x6G-EVAL revB Set-up
    - Use the Tamper push-button connected to pin PC.13 (EXTI15_10)
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
