/**
  @page FLASH_WriteProtection FLASH write protection
  
  @verbatim
  ******************************************************************************
  * @file    FLASH/FLASH_WriteProtection/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FLASH write protection example.
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

How to configure and use the FLASH HAL API to enable and disable the write 
protection of the internal Flash memory.

To run this example, user has to follow these steps:
 1. Use the ST-Link Utility (Target -> Options Bytes) to specify the FLASH mode: single bank or dual bank
 2. Depending on the selected FLASH mode, uncomment or keep commented the corresponding line inside the main.h file:
    @code
       /*#define DUAL_BANK*/
    @endcode          
 3. Load the example.
 
At the beginning of the main program the HAL_Init() function is called to 
reset all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) 
to run at 200 MHz.

Each time the Tamper push-button is pressed, the program will check the 
write protection status of FLASH_WRP_SECTORS (defined in main.c) 
  - If FLASH_WRP_SECTORS are write protected, the write protection will be 
    disabled.
    Then the following message will be displayed on LCD, if protection disable 
    operation is done correctly: 
      "Write protection is disabled"
    Otherwise the following message will be displayed on LCD:
      "Write protection is not disabled"
  - If FLASH_WRP_SECTORS are not write protected, the write protection will 
    be enabled.
    Then the following message will be displayed on LCD, if protection enable 
    operation is done correctly:
      "Write protection is enabled"
    Otherwise the following message will be displayed on LCD:
      "Write protection is not enabled"

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds) 
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from 
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower) 
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set 
      to 1 millisecond to have correct HAL operation.

@par Keywords

Memory, Flash, Write protection, Sector, Program, Erase

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

  - FLASH/FLASH_WriteProtection/Inc/stm32f7xx_hal_conf.h        HAL Configuration file  
  - FLASH/FLASH_WriteProtection/Inc/stm32f7xx_it.h              Header for stm32f7xx_it.c
  - FLASH/FLASH_WriteProtection/Inc/main.h                      Header for main.c module 
  - FLASH/FLASH_WriteProtection/Src/stm32f7xx_it.c              Interrupt handlers
  - FLASH/FLASH_WriteProtection/Src/main.c                      Main program
  - FLASH/FLASH_WriteProtection/Src/system_stm32f7xx.c          STM32F7xx system clock configuration file

@par Hardware and Software environment 
  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with STM32F769I-EVAL board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following:
 - Open STM32 ST-Link Utility, go to Target -> Options Bytes, then check "nDBANK" box and apply
 - Open your preferred toolchain
 - In main.h, keep commented the DUAL_BANK define: /*#define DUAL_BANK*/ 
 - Rebuild all files and load your image into target memory
 - Run the example
 - Using STM32 ST-Link Utility, go to Target -> Options Bytes, then uncheck "nDBANK" box and apply
 - In main.h, uncomment the DUAL_BANK define: #define DUAL_BANK
 - Rebuild all files and load your image into target memory
 - Run the example
   
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
