/**
  @page QSPI_PreInitConfig QSPI PreInit Config example
  
  @verbatim
  ******************************************************************************
  * @file    QSPI/QSPI_PreInitConfig/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the code execution from QSPI memory example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to configure the QSPI IP in order to have access to external memory just after reset.

This example describes how to execute a part of the code from a QSPI memory. To do this, 
a section is created where the function is stored.

The QSPI is configured in memory-mapped mode before to call main() function and QSPI memory 
is then available after the reset. 

The configuration of the QSPI is done in the file system_stm32f7xx.c file in the 
function SystemInit_ExtMemCtl(). It is called by the function SystemInit(), which
is the first function called by the Reset_Handler.

The code in executed in a forever loop. It is the same code as GPIO_IOToggle example 
with some code mapped in QSPI mode and other in Flash, to show the jump from one 
memory to the other.

LED1 toggles in this forever loop.

In this example, HCLK is configured at 216 MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on a variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise, the caller ISR process will be blocked.
      To change the SysTick interrupt priority, you have to use the HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond,
      to have correct HAL operation.

@par Keywords

Memory, QUADSPI, Erase, Write, DMA, section, memory-mapped mode, PreInit configuration, Jump

@Note�If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
 �����then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
������The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
����� It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
������In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
������Even though the user must manage the cache coherence for read accesses.
������Please refer to the AN4838 �Managing memory protection unit (MPU) in STM32 MCUs�
������Please refer to the AN4839 �Level 1 cache on STM32F7 Series�


@par Directory contents 

  - QSPI/QSPI_PreInitConfig/Inc/stm32f7xx_hal_conf.h HAL configuration file
  - QSPI/QSPI_PreInitConfig/Inc/stm32f7xx_it.h       Interrupt handlers header file
  - QSPI/QSPI_PreInitConfig/Inc/main.h               Header for main.c module  
  - QSPI/QSPI_PreInitConfig/Src/stm32f7xx_it.c       Interrupt handlers
  - QSPI/QSPI_PreInitConfig/Src/main.c               Main program
  - QSPI/QSPI_PreInitConfig/Src/system_stm32f7xx.c   STM32F7xx system source file
  - QSPI/QSPI_PreInitConfig/Src/stm32f7xx_hal_msp.c  HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F7xx devices.

  - This example has been tested on STM32746G-DISCOVERY rev B board containing the N25Q128A QSPI memory  
    and can be easily tailored to any other supported device and/or development board .
    However, it does not work  on the STM32746G-DISCOVERY Rev C03 board, as it contains the W25Q128J QSPI memory.


  - STM32746G-DISCOVERY Set-up :
    - Board is configured by default to access N25Q128A QSPI memory
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 - When referring to EWARM or SW4STM32 projects, load your image into target memory 
   using ST-Link Utility and external loader N25Q128A_STM32F746G-DISCO.


 */
