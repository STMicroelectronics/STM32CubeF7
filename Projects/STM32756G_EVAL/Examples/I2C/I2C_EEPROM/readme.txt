/**
  @page I2C_EEPROM I2C EEPROM DMA example
  
  @verbatim
  ******************************************************************************
  * @file    I2C/I2C_EEPROM/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C EEPROM DMA example.
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

How to handle I2C data buffer transmission/reception with DMA. In the example, 
the device communicates with an I2C EEPROM memory.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 216 MHz.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function to change I2C configuration.

The I2C/EEPROM communication is then initiated.
The HAL_I2C_Mem_Read_DMA() and the HAL_I2C_Mem_Write_DMA() functions allow respectively 
the reception of Data from EEPROM and the transmission of a predefined data 
buffer.

For this example the TxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step the I2C writes the aTxBuffer by group of 4 bytes (RF EEPROM 
Page size) using HAL_I2C_Mem_Write_DMA() then read back the data through aRxBuffer
using HAL_I2C_Mem_Read_DMA(). 
The end of this two steps are monitored through the HAL_I2C_GetState() function
result.
Finally, aRxBuffer and aRxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

STM32 Eval board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the transmission process is complete.
 - LED2 is ON when the reception process is complete.
 - LED3 is ON when there is an error in transmission/reception process.  

 @note I2Cx instance used and associated resources can be updated in "main.h"
       file depending hardware configuration used.


SCL Pin: PB.08
SDA Pin: PB.09

   __________________________________________________________________________                        
  |           ______________                        ______________           |
  |          | I2C1         |                      |    I2C_EEPROM|          |
  |          |              |                      |              |          |
  |          |           SCL|______________________|CLOCK         |          |
  |          |              |                      |              |          |
  |          |              |                      |              |          |
  |          |              |                      |              |          |
  |          |           SDA|______________________|DATA          |          |
  |          |              |                      |              |          |
  |          |______________|                      |______________|          |
  |      __                                                                  |
  |     |__|                                                                 |
  |    TAMPER                                                                |
  |                                                                          |
  |__________________________________________________________________________|
      STM327x6G-EVAL

@par Keywords

Connectivity, Memory, EEPROM, I2C, SCL, SDA, DMA, Read, Write

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

  - I2C/I2C_EEPROM/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - I2C/I2C_EEPROM/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - I2C/I2C_EEPROM/Inc/main.h                  Header for main.c module  
  - I2C/I2C_EEPROM/Src/stm32f7xx_it.c          Interrupt handlers
  - I2C/I2C_EEPROM/Src/main.c                  Main program
  - I2C/I2C_EEPROM/Src/system_stm32f7xx.c      STM32F7xx system source file
  - I2C/I2C_EEPROM/Src/stm32f7xx_hal_msp.c     HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F756xx/STM32F746xx devices.
    
  - This example has been tested with STM327x6G-EVAL board revB and can be
    easily tailored to any other supported device and development board.      

  - STM327x6G-EVAL Set-up 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
