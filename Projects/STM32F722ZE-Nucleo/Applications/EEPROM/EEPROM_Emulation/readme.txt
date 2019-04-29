/**
  @page EEPROM_Emulation application to show an eeprom emulation

  @verbatim
  ******************************************************************************
  * @file    EEPROM/EEPROM_Emulation/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the EEPROM_Emulation application.
  ******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Application Description 

This application shows how to emulate EEPROM on internal flash.

Please refer to AN3969 for futher details regarding this application.

NUCLEO-F722ZE board LEDs can be used to monitor the application status:
  - LED2 is On when the application runs successfully.
  - LED2 is toggle in case of error.
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note Before using the application, ensure that dedicated sectors for EEPROM are erased
      to avoid a match with a valid page OPCODE.

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

 - EEPROM/EEPROM_Emulation/Inc/stm32f7xx_hal_conf.h          HAL Configuration file
 - EEPROM/EEPROM_Emulation/Inc/main.h                        Header for main.c module
 - EEPROM/EEPROM_Emulation/Inc/eeprom.h                      Header for eeprom.c module 
 - EEPROM/EEPROM_Emulation/Inc/stm32f7xx_it.h                Header for stm32f7xx_it.c
 - EEPROM/EEPROM_Emulation/Src/main.c                        Main program
 - EEPROM/EEPROM_Emulation/Src/eeprom.c                      EEPROM program
 - EEPROM/EEPROM_Emulation/Src/stm32f7xx_it.c                Interrupt handlers
 - EEPROM/EEPROM_Emulation/Src/system_stm32f7xx.c            STM32F7xx system clock configuration file 
    

@par Hardware and Software environment

  - This application runs on STM32F722ZE devices.
    
  - This application has been tested with NUCLEO-F722ZE board and can be
    easily tailored to any other supported device and development board.

Table 1. EEPROM application implementation on NUCLEO-F722ZE 
/*** Platform ***|************* Implementation **************************|***** Configuration *****\
****************************************************************************************************
|    Firmware    | The EEPROM program is located at 0x08000000. The Flash|                         |
|                | routines (program/erase) are executed from the Flash  |                         |
|                | memory.                                               |                         |
|                | The size of this program is about 6 Kbytes and        |                         |
|                | programmed on:                                        | Sector 0                |  
|                | ------------------------------------------------------|-------------------------|
|                |                                                       |                         | 
|                | The flash zone used for the EEPROM emulation is       |         32KB            |
|                | located at 0x08008000                                 |  (Sector2 - Sector3)    |
|                |                                                       |                         | 
\**************************************************************************************************/

@note
Before using the application, ensure that dedicated sectors for EEPROM (FLASH_SECTOR_2 and FLASH_SECTOR_3) 
are erased to avoid a match with a valid page OPCODE.

Following picture illustrates the situation in program memory:

 Top Flash Memory address /-------------------------------------------\  0x0807FFFF
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |          Sector 4 - Sector 7              |
                          |-------------------------------------------|  0x08010000
    Sector 3 (16KB)       |                                           |
                          |                                           |
                          | Flash used for EEPROM emulation mechanism |  0x0800C000
    Sector 2 (16KB)       |                                           |
                          |                                           |
                          |-------------------------------------------|  0x08008000
    Sector 1 (16KB)       |                                           |
                          |                                           |
                          |-------------------------------------------|  0x08004000
                          |                                           |
    Sector 0 (16KB)       |   flash used for implement the EEPROM     |
                          |          emulation mechanism(6KB)         |
                          \-------------------------------------------/	 0x08000000

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
