/**
  @page FreeRTOS_ThreadCreation FreeRTOS Thread Creation example
 
  @verbatim
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_ThreadCreation/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS Thread Creation example.
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

@par Application Description

How to implement thread creation using CMSIS RTOS API. 

This example creates two threads with the same priority, which execute in 
a periodic cycle of 18 seconds. 

In the first 6 seconds, the thread 1 toggles LED5 each 200 ms and the 
thread 2 toggles LED6 each 500 ms.
In the following 6 seconds, the thread 1 suspends itself and the thread 2
continue toggling LED6.
In the last 6 seconds, the thread 2 resumes execution of thread 1 then 
suspends itself, the thread 1 toggles the LED5 each 400 ms.    

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in HAL time base ISR.
      This implies that if HAL_Delay() is called from a peripheral ISR process, then
      the HAL time base interrupt must have higher priority (numerically lower) than
      the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority()
      function.
 
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the 
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".


@par Keywords

RTOS, FreeRTOS, Threading

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
    - FreeRTOS/FreeRTOS_ThreadCreation/Src/main.c                         Main program
    - FreeRTOS/FreeRTOS_ThreadCreation/Src/stm32f7xx_hal_timebase_tim.c   HAL timebase file
    - FreeRTOS/FreeRTOS_ThreadCreation/Src/stm32f7xx_it.c                 Interrupt handlers
    - FreeRTOS/FreeRTOS_ThreadCreation/Src/system_stm32f7xx.c             STM32F7xx system clock configuration file
    - FreeRTOS/FreeRTOS_ThreadCreation/Inc/main.h                         Main program header file
    - FreeRTOS/FreeRTOS_ThreadCreation/Inc/stm32f7xx_hal_conf.h           HAL Library Configuration file
    - FreeRTOS/FreeRTOS_ThreadCreation/Inc/stm32f7xx_it.h                 Interrupt handlers header file
    - FreeRTOS/FreeRTOS_ThreadCreation/Inc/FreeRTOSConfig.h               FreeRTOS Configuration file

@par Hardware and Software environment

  - This example runs on STM32F722xx/STM32F723xx/STM32F732xx/STM32F733xx devices.
    
  - This example has been tested with STM32F723E-DISCOVERY board and can be
    easily tailored to any other supported device and development board.
    

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  

 */
