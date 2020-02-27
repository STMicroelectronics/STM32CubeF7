/**
  @page SPI_TwoBoards_FullDuplex_IT SPI example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/SPI/SPI_TwoBoards_FullDuplex_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI_TwoBoards_FullDuplex_IT example.
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

Data buffer transmission and receptionvia SPI using Interrupt mode. This 
example is based on the STM32F7xx SPI LL API. The peripheral 
initialization uses LL unitary service functions for optimization purposes (performance and size).

The communication is done with 2 boards through SPI.
   _________________________                        _________________________
  |       ___________ ______|                      |__________________       |
  |      |SPI1              |                      |             SPI1 |      |
  |      |                  |                      |                  |      |
  |      |         CLK(PA5) |______________________|(PA5)CLK          |      |
  |      |                  |                      |                  |      |
  |      |         MISO(PA6)|______________________|(PA6)MISO         |      |
  |      |                  |                      |                  |      |
  |      |         MOSI(PA7)|______________________|(PA7)MOSI         |      |
  |      |                  |                      |                  |      |
  |      |__________________|                      |__________________|      |
  |      __                 |                      |                         |
  |     |__|                |                      |                         |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32F7xx Master _______|                      |_STM32F7xx Slave ________|

This example shows how to configure GPIO and SPI peripherals
to use a Full-Duplex communication using IT mode through the STM32F7xx COM_INSTANCE1_TYPE LL API.

The project is splitted in two parts the Master Board and the Slave Board:

- Master Board
  SPI1 Peripheral is configured in Master mode.
  SPI1_IRQn activated. 
  RXNE and TXE Interrupts SPI peripheral activated.
  GPIO associated to User push-button is linked with EXTI. 
  
- Slave Board
  SPI1 Peripheral is configured in Slave mode.
  SPI1_IRQn activated. 
  RXNE and TXE Interrupts SPI peripheral activated.

The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file:
- Uncomment "#define MASTER_BOARD" to select Master board.
- Comment "#define MASTER_BOARD" to select Slave board.

Example execution:
On BOARD MASTER, LED1 is blinking Fast (200ms) and wait User push-button action.
Press User push-button on BOARD MASTER start a Full-Duplex communication through IT.
On MASTER side, Clock will be generated on SCK line, Transmission(MOSI Line) and reception (MISO Line) 
will be done at the same time. 
SLAVE SPI will received  the Clock (SCK Line), so Transmission(MISO Line) and reception (MOSI Line) will be done also.

LED1 is On if data is well received.

In case of errors, LED1 is blinking Slowly (1s).

@note You need to perform a reset on Master board, then perform it on Slave board
      to have the correct behaviour of this example.

@par Keywords

Connectivity, SPI, Full-duplex, Advanced communication, Transmission, Reception, Master, Slave, MISO, MOSI

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

  - SPI/SPI_TwoBoards_FullDuplex_IT/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - SPI/SPI_TwoBoards_FullDuplex_IT/Inc/main.h                  Header for main.c module
  - SPI/SPI_TwoBoards_FullDuplex_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - SPI/SPI_TwoBoards_FullDuplex_IT/Src/stm32f7xx_it.c          Interrupt handlers
  - SPI/SPI_TwoBoards_FullDuplex_IT/Src/main.c                  Main program
  - SPI/SPI_TwoBoards_FullDuplex_IT/Src/system_stm32f7xx.c      STM32F7xx system source file

@par Hardware and Software environment

  - This example runs on STM32F767xx devices.

  - This example has been tested with NUCLEO-F767ZI board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F767ZI Set-up
    - Connect Master board PA5 to Slave Board PA5 (connected to pin 10 of CN7 connector)
    - Connect Master board PA6 to Slave Board PA6 (connected to pin 12 of CN7 connector)
    - Connect Master board PA7 to Slave Board PA7 (connected to pin 14 of CN7 connector)
    - Connect Master board GND to Slave Board GND

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
