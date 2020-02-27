/**
  @page SPI_HalfDuplex_ComPollingIT SPI Half Duplex IT example
  
  @verbatim
  ******************************************************************************
  * @file    SPI/SPI_HalfDuplex_ComPollingIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI Half Duplex IT example.
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

Data buffer transmission/reception between 
two boards via SPI using Polling (LL driver) and Interrupt modes (HAL driver).

   _________________________                        _________________________
  |           _______CN11___|                      |___CN11_______           |
  |          |SPI3          |                      |          SPI3|          |
  |          |              |                      |              |          |
  |          |     CLK(PC10)|______________________|(PC10)CLK     |          |
  |          |              |                      |              |          |
  |          |    MOSI(PC12)|______________________|(PC11)MISO    |          |
  |          |              |                      |              |          |
  |          |______________|                      |______________|          |
  |      __                 |                      |                         |
  |     |__|                |                      |                         |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32F7 Master _________|                      |_STM32F7 Slave __________|


At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 216 MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK & 
GPIO). You may update this function to change SPI configuration.
The Half-Duplex SPI transmission (8bit) is done using LL Driver on Master board (Tx) by using function 
LL_SPI_TransmitData8.
The The Half-Duplex SPI reception (8bit) is done using HAL Driver on Slave board (Rx) by using function 
HAL_SPI_Receive_IT.

Example execution:
First step, press the User push-button, this action initiates a Half-Duplex transfer 
between Master and Slave.
After end of transfer, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file.
If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.

STM32 board's LEDs can be used to monitor the transfer status:
 - LED1 toggles quickly on master board waiting User push-button to be pressed.
 - LED2 turns ON on slave board when the reception process is complete.
 - LED3 turns ON when there is an error in reception process. 

@note SPIx instance used and associated resources can be updated in "main.h"
      file depending hardware configuration used.

@note You need to perform a reset on Slave board, then perform it on Master board
      to have the correct behaviour of this example.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, SPI, Full-duplex, Polling, Transmission, Reception, Master, Slave, MISO, MOSI

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

  - Examples_MIX/SPI/SPI_HalfDuplex_ComPollingIT/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - Examples_MIX/SPI/SPI_HalfDuplex_ComPollingIT/Inc/stm32f7xx_it.h          SPI interrupt handlers header file
  - Examples_MIX/SPI/SPI_HalfDuplex_ComPollingIT/Inc/main.h                  Header for main.c module  
  - Examples_MIX/SPI/SPI_HalfDuplex_ComPollingIT/Src/stm32f7xx_it.c          SPI interrupt handlers
  - Examples_MIX/SPI/SPI_HalfDuplex_ComPollingIT/Src/main.c                  Main program
  - Examples_MIX/SPI/SPI_HalfDuplex_ComPollingIT/Src/system_stm32f7xx.c      STM32F7xx system source file
  - Examples_MIX/SPI/SPI_HalfDuplex_ComPollingIT/Src/stm32f7xx_hal_msp.c     HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F767xx devices.

  - Take care to cable connection between Master and Slave Board:
    Cable shall be shorter than 5 cm and rigid if possible.

  - This example has been tested with NUCLEO-F767ZI board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F767ZI Set-up
    - Connect Master board PC10 to Slave Board PC10 (pin 1 in CN11 connector)
    - Connect Master board PC12 to Slave Board PC11 (pin 3 and pin 2 in CN11 connector)
    - Connect Master board GND  to Slave Board GND

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 