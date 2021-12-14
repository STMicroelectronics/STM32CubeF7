/**
  @page EEPROM_Emulation application to show an eeprom emulation

  @verbatim
  ******************************************************************************
  * @file    EEPROM_Emulation/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the EEPROM_Emulation application.
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

This application is a software solution that allows substituting a standalone EEPROM 
by emulating the EEPROM mechanism using the on-chip Flash devices. The emulation
is achieved by using at least two Flash sectors and the coherence mechanism is ensured 
by swapping between both sectors.

Please refer to AN3969 for further details regarding this application.

STM32F723E-DISCOVERY's LEDs can be used to monitor the application status:
  - LED6 toggles when the example is running successfully.
  - LED5 toggles in case of error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
	  
@note Before using the application, ensure that dedicated sectors for EEPROM (Sector2 and Sector3) 
      are erased to avoid a match with a valid page OPCODE.
	  
@par Keywords

EEPROM, Emulation, Flash, Program, Erase, Sectors, OPCODE

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


 - EEPROM_Emulation/Inc/stm32f7xx_hal_conf.h        HAL Configuration file
 - EEPROM_Emulation/Inc/eeprom.h                    Header for main.c module 
 - EEPROM_Emulation/Inc/stm32f7xx_it.h              Header for stm32f7xx_it.c
 - EEPROM_Emulation/Src/main.c                      Main program
 - EEPROM_Emulation/Src/eeprom.c                    EEPROM program
 - EEPROM_Emulation/Src/stm32f7xx_it.c              Interrupt handlers
 - EEPROM_Emulation/Src/system_stm32f7xx.c          STM32F7xx system clock configuration file 
    

@par Hardware and Software environment 

  - This example runs on STM32F722xx/STM32F723xx/STM32F732xx/STM32F733xx devices.
    
  - This example has been tested with STM32F723E-DISCOVERY board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
