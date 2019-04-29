    /**
  @page CORTEXM_ProcessStack Cortex-M7 Process Stack example

  @verbatim
  ******************************************************************************
  * @file    CORTEXM/CORTEXM_ProcessStack/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Cortex-M7 Process Stack example.
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

How to modify the Thread mode stack. Thread mode is entered on reset, and can be
entered as a result of an exception return.  

The associated program is used to:

1. Switch the Thread mode stack from Main stack to Process stack

2. Generate ISR (SVC) to confirm Stack pointer used under exception is Main Stack

3. Returning to main(), check is performed on current Stack to show cortex has switched back to Process
automatically.
In case of error, LED5 is blinking fast. In case of success, LED6 is turned on.

To monitor the stack used  a set of variables is available within the program. It is also
possible to use the 'Cortex register' window of the debugger.
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, Cortex, Process Stack, HCLK, Thread mode Stack, main stack, process stack, ISR, 

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

  - CORTEXM/CORTEXM_ProcessStack/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - CORTEXM/CORTEXM_ProcessStack/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - CORTEXM/CORTEXM_ProcessStack/Inc/main.h                  Header for main.c module  
  - CORTEXM/CORTEXM_ProcessStack/Src/stm32f7xx_it.c          Interrupt handlers
  - CORTEXM/CORTEXM_ProcessStack/Src/main.c                  Main program
  - CORTEXM/CORTEXM_ProcessStack/Src/system_stm32f7xx.c      STM32F7xx system source file

@par Hardware and Software environment

  - This example runs on STM32F722xx/STM32F723xx/STM32F732xx/STM32F733xx devices.
    
  - This example has been tested with STM32F723E-DISCOVERY board and can be
    easily tailored to any other supported device and development board.      

@par How to use it ? 
In order to make the program work, you must do the following :
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
- Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
