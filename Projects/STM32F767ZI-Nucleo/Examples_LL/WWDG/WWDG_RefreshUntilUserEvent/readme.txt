/**
  @page WWDG_RefreshUntilUserEvent WWDG example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/WWDG/WWDG_RefreshUntilUserEvent/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the WWDG_RefreshUntilUserEvent example.
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

Configuration of the WWDG to periodically update the counter and 
generate an MCU WWDG reset when a user button is pressed. The peripheral initialization 
uses the LL unitary service functions for optimization purposes (performance and size).

Example Configuration:
Configure the WWDG (Window, Prescaler & Counter) and enable it.
Refresh the WWDG downcounter in the main loop - Led is blinking fastly & continuously

Example Execution:
When User Button is pressed, the Downcounter automatic refresh mechanism is disable and thus, reset will occur.
After a reset when re-entering in the main, RCC WWDG Reset Flag will be checked and if we are back from a WWDG reset the led will be switch ON.

Waiting a new user button pressed to re-activate the WWDG

@par Keywords

System, WWDG, Timeout, Refresh, Counter update, MCU Reset, Downcounter, Event, Window

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

  - WWDG/WWDG_RefreshUntilUserEvent/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - WWDG/WWDG_RefreshUntilUserEvent/Inc/main.h                  Header for main.c module
  - WWDG/WWDG_RefreshUntilUserEvent/Inc/stm32_assert.h          Template file to include assert_failed function
  - WWDG/WWDG_RefreshUntilUserEvent/Src/stm32f7xx_it.c          Interrupt handlers
  - WWDG/WWDG_RefreshUntilUserEvent/Src/main.c                  Main program
  - WWDG/WWDG_RefreshUntilUserEvent/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx devices.
    
  - This example has been tested with NUCLEO-F767ZI board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
