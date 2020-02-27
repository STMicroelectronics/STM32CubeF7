/**
  @page PWR_EnterStandbyMode PWR standby example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_EnterStandbyMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR STANDBY mode example.
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

How to enter the Standby mode and wake up from this mode by using an external 
reset or a wakeup interrupt.

In the associated software, the system clock is set to 216 MHz, an EXTI line
is connected to the user button thru PC.13 and configured to generate an 
interrupt on rising edge upon key press.
LED1 is toggled each 200 ms in order to indicate that MCU is in RUN mode.

When a rising edge is detected on the EXTI line, an interrupt is generated. 
In the EXTI handler routine, the wake-up pin LL_PWR_WAKEUP_PIN4 is enabled and the 
corresponding wake-up flag cleared. Then, the system enters STANDBY mode causing 
LED1 to stop toggling. 

Next, the user can wake-up the system in pressing the User push-button which is 
connected to the wake-up pin LL_PWR_WAKEUP_PIN4.
A rising edge on WKUP pin will wake-up the system from STANDBY.
Alternatively, an external RESET of the board will lead to a system wake-up as well.

After wake-up from STANDBY mode, program execution restarts in the same way as 
after a RESET and LED1 restarts toggling.

LED1 is used to monitor the system state as follows:
 - LED1 fast toggling: system in RUN mode
 - LED1 slow toggling: system in RUN mode after exiting from standby mode
 - LED1 off : system in STANDBY mode

These steps are repeated in an infinite loop.

@note To measure the current consumption in STANDBY mode, remove JP5 jumper 
      and connect an ampere meter to JP5 to measure IDD current.     

@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M7 core is no longer clocked during low power mode 
      so debugging features are disabled.

@par Keywords

Power, PWR, Standby mode, Interrupt, EXTI, Wakeup, Low Power, External reset,

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

  - PWR/PWR_EnterStandbyMode/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - PWR/PWR_EnterStandbyMode/Inc/main.h                  Header for main.c module
  - PWR/PWR_EnterStandbyMode/Inc/stm32_assert.h          Template file to include assert_failed function
  - PWR/PWR_EnterStandbyMode/Src/stm32f7xx_it.c          Interrupt handlers
  - PWR/PWR_EnterStandbyMode/Src/main.c                  Main program
  - PWR/PWR_EnterStandbyMode/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-F767ZI
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-F767ZI Set-up
    - LED1 connected to PB.0 pin
    - User push-button connected to pin PC.13 (External line 15 to 10)
    - WakeUp Pin LL_PWR_WAKEUP_PIN4 connected to PC.13

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
