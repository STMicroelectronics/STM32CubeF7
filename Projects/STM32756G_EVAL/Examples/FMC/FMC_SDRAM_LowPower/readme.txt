/**
  @page FMC_SDRAM_LowPower SDRAM memory functionnalities example
  
  @verbatim
  ******************************************************************************
  * @file    FMC/FMC_SDRAM_LowPower/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FMC_SDRAM_LowPower example.
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

How to configure the FMC controller to access the SDRAM memory in low power
mode (SDRAM Self Refresh mode).

It gives a simple application of the FMC SDRAM low power mode use (self refresh mode) 
while the MCU is in a low power mode (STOP mode).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 200 MHz.
  
The purpose is to show how the SDRAM can retain data written after entering STOP mode. 
STOP mode is a CPU low power mode which stops all peripherals clocks in the 1.2V 
domain. Only internal SRAM and registers content are preserved in this mode.
  
After SDRAM initialization, the data is written to the memory and a "self refresh" 
command is sent to the SDRAM. The program waits for TAMPER/KEY button to be pushed 
to enter the CPU in STOP mode, the LED1 and LED3 are then turned ON. 
The wakeup from STOP mode is done when pushing WAKEUP Button and the CPU returns 
to RUN mode. At this time, the system clock is reconfigured using the function 
SystemClock_Config(). 

Finally, a "normal mode" command is send to SDRAM memory to exit self refresh mode. 
The data written to SDRAM is read back and checked.  
   
LEDs are used to indicate the system state as following:
  - LED1 and LED3 ON: CPU enters STOP mode.
  - LED1 and LED3 OFF: the CPU is in RUN mode.
  - LED1 ON: correct data transfer (PASS). 
  - LED3 ON: incorrect data transfer (FAIL).
  - LED3 toggles in a forever loop as soon as an error is returned by BSP API. 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.
      
@par Keywords

Memory, FMC, SDRAM, Read, Write, Initialization, Access, Low power, Self Refresh, Stop mode, 

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

  - FMC/FMC_SDRAM_LowPower/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - FMC/FMC_SDRAM_LowPower/Inc/main.h                  Header for main.c module  
  - FMC/FMC_SDRAM_LowPower/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - FMC/FMC_SDRAM_LowPower/Src/main.c                  Main program
  - FMC/FMC_SDRAM_LowPower/Src/stm32f7xx_it.c          Interrupt handlers
  - FMC/FMC_SDRAM_LowPower/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F756xx/STM32F746xx devices.
    
  - This example has been tested with STM327x6G-EVAL board revB and can be
    easily tailored to any other supported device and development board.
  - To use LED1, ensure that JP24 is in position 2-3
  - To use LED3, ensure that JP23 is in position 2-3

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
