/**
  @page CRC_Example Cyclic Redundancy Check Example
  
  @verbatim
  ******************************************************************************
  * @file    CRC/CRC_Example/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of Cyclic Redundancy Check Example.
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

How to configure the CRC using the HAL API. The CRC (cyclic
redundancy check) calculation unit computes the CRC code of a given buffer of
32-bit data words, using a fixed generator polynomial (0x4C11DB7).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 216 MHz.

The CRC peripheral configuration is ensured by the HAL_CRC_Init() function.
This later is calling the HAL_CRC_MspInit()function which core is implementing
the configuration of the needed CRC resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function to change CRC configuration.

The calculated CRC code is compared to displayed on the debugger in uwCRCValue variable.
Once calculated, the CRC value is compared to the CRC expected value.

STM32 Eval board's LEDs can be used to monitor the example status:
  - LED1 (GREEN) is ON when a correct CRC value is calculated
  - LED3 (RED) is ON when an incorrect CRC value is calculated or when there is an initialization error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Security, CRC, CRC Polynomial, IEC 60870-5, hardware CRC, 

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
  
  - CRC/CRC_Example/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - CRC/CRC_Example/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - CRC/CRC_Example/Inc/main.h                  Header for main.c module
  - CRC/CRC_Example/Src/stm32f7xx_it.c          Interrupt handlers
  - CRC/CRC_Example/Src/main.c                  Main program
  - CRC/CRC_Example/Src/stm32f7xx_hal_msp.c     HAL MSP module 
  - CRC/CRC_Example/Src/system_stm32f7xx.c      STM32F7xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32F756xx/STM32F746xx devices.
  
  - This example has been tested with STMicroelectronics STM327x6G-EVAL revB
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 

 */
 