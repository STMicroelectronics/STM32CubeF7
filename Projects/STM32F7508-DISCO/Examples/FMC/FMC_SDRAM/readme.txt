/**
  @page FMC_SDRAM SDRAM memory functionalities example
  
  @verbatim
  ******************************************************************************
  * @file    FMC/FMC_SDRAM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FMC_SDRAM example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to configure the FMC controller to access the SDRAM memory.

The SDRAM is MT48LC4M32B2B5-6A.

In this example, the code is executed from QSPI external memory while data are in internal SRAM memory.
 
At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 200 MHz.
  
The goal of this example is to explain the different steps to configure the FMC 
and make the SDRAM device ready for access, without using the MSP layer.          

In this example, the SDRAM device is configured and initialized explicitly 
following all initialization sequence steps. After initializing the device, user 
can perform read/write operations on it. A data buffer is written to the SDRAM 
memory, then read back and checked to verify its correctness.

The user can define the FMC memory bus width by commenting/uncommenting the defines for  
desired configurations in "main.h" : 
FMC_SDRAM_MEM_BUS_WIDTH_8 or FMC_SDRAM_MEM_BUS_WIDTH_16
The SDRAM memory access are done in 32 bits whatever the configured FMC memory bus width.  

If the data is read correctly from SDRAM, the LED1 is ON,  otherwise LED1 blinks :
 - LED1 is Toggled with a period of 200 ms when there is a read or write error. 
 - LED1 is Toggled with a period of 1000 ms when there is an error in SDRAM Init process.

@note - The FMC mode register definition is configured using defines for the external 
        memory device mode register, defined in the main header file.
      - The function "BSP_SDRAM_Initialization_Sequence()" is used to program the SDRAM
        device. It is considered as a specific function which depends on the SDRAM device. 
        When changing the external device, you may have to apply some changes 
        in this function. 

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

Memory, FMC, SDRAM, Read, Write, Initialization, Access

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

  - FMC/FMC_SDRAM/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - FMC/FMC_SDRAM/Inc/main.h                  Header for main.c module  
  - FMC/FMC_SDRAM/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - FMC/FMC_SDRAM/Src/main.c                  Main program
  - FMC/FMC_SDRAM/Src/stm32f7xx_msp.c          HAL MSP module
  - FMC/FMC_SDRAM/Src/stm32f7xx_it.c          Interrupt handlers
  - FMC/FMC_SDRAM/Src/system_stm32f7xx.c      STM32F7xx system source file



@par Hardware and Software environment

  - This example runs on STM32F750xx devices.

  - This example has been tested with STMicroelectronics STM32F7508-DISCO
    boards and can be easily tailored to any other supported device
    and development board.   

@par How to use it ?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc.
     The default configuration is the right one: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with this example "Examples\FMC\FMC_SDRAM" (see below).
  4. Start debugging user example or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image
      
In order to load this LTDC example to the external memory :
 - Open your preferred toolchain 
 - Rebuild all files. 
 - Run & debug the program:
   - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
   - Using SW4STM32 :
       - Open the STM32CubeProgrammer tool
       - Select the QSPI external flash loader "N25Q128A_STM32F7508-DISCO" 
       - From Erasing & Programming menu, browse and open the output binary file relative to this example
       - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
