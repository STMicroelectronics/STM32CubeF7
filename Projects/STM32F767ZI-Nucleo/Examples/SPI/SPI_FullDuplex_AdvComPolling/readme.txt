/**
  @page SPI_FullDuplex_AdvComIT SPI Full Duplex Polling example
  
  @verbatim
  ******************************************************************************
  * @file    SPI/SPI_FullDuplex_AdvComIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI Full Duplex IT example.
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
 
Configuration of the HAL SPI API to transmit/receive a data buffer using Polling mode
in an advanced communication mode: 
The master board always sends the command to the slave before performing any transmission; 
the slave board sends back an acknowledgement before proceeding.

- Hardware Description

To use this example, you need to load it on two STM32 boards (let's call them 
BoardA and BoardB) then connect these two boards through SPI lines and GND.
In the firmware example two project are provided to use the SPI peripheral as STM32
Master device or as STM32 Slave .
   _________________________                        _________________________
  |           ______________|                      |______________           |
  |          |SPI1          |                      |          SPI1    |      |
  |          |              |                      |                  |      |
  |          |     CLK(PB3) |______________________|(PB3)CLK  CN7.15  |      |
  |          |              |                      |                  |      |
  |          |     MISO(PA6)|______________________|(PA6)MISO CN7.D12 |      |
  |          |              |                      |                  |      |
  |          |     MOSI(PA7)|______________________|(PA7)MOSI CN7.D11 |      |
  |          |              |                      |                  |      |
  |          |______________|                      |__________________|      |
  |      __                 |                      |                         |
  |     |__|                |                      |                         |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32F7 Master _________|                      |_STM32F7 Slave __________|
  
@note The connection between the pins should use a short wires and a common Ground.

- Software Description  

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 100 MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change SPI configuration.

The SPI peripheral is configured in full duplex mode with baudrate = FCPU/256. 

If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.
                       
In Master board, when the user button is pressed (used for only synchronization at startup)
these steps are executed: 
 - Synchro with the Slave
 - Requests read operation by sending specific command to the slave that contain
   the master read command and the number of data to be read
 - Synchro with the Slave
 - Receive the ACK from the Slave
 - Synchro with the Slave  
 - Receive the requested data "aRxBuffer" from the slave
 - Synchro with the Slave
 - Send the ACK to the Slave
 - Checks the correctness of data and in case of mismatch between sent and received data,
   LED2 is On and communication is stopped (using infinite loop). Otherwise LED3 is Toggle
   indicate that the data is received correctly.
 - Synchro with the Slave
 - Requests write operation by sending specific command to the slave that contain
   the master write command and the number of data to be written
 - Synchro with the Slave 
 - Receive the ACK from the Slave
 - Synchro with the Slave 
 - Send the requested data "aTxMasterBuffer" to the slave
 - Synchro with the Slave 
 - Receive the ACK from the Slave
These previously steps are repeated in an infinitely loop and a delay 100ms is inserted at
the end of data transfer to permit the user to see LED1 toggling.

In Slave board, these steps are executed: 
 - Synchro with the Master
 - Receive requests from the master
 - Synchro with the Master
 - Send the ACK to the Master
 - Synchro with the Master
 - If the "ADDRCMD_MASTER_READ" command is received: 
   - Send the requested data "aTxSlaveBuffer" to the Master
   - Synchro with the Master
   - Receive the ACK from the Master
 - If the "ADDRCMD_MASTER_WRITE" command is received: 
   - Receive the requested data "aRxBuffer" from the Master
   - Synchro with the Master
   - Send the ACK to the Master
   - checks the correctness of data is performed and in case of mismatch between sent and
     received data, LED2 is On and communication is stopped (using infinite loop).
     Otherwise LED3 Toggles indicating that the data is received correctly.

These previously steps are repeated in an infinitely loop.

STM32F7-Nucleo  board's LEDs can be used to monitor the transfer status:
 - LED1 (green) toggles on Master board waiting for user button to be pushed.
 - LED1 (green) toggles on Master board at end of each loop.
 - LED2 (blue) toggles on both boards when the read comparison is OK.
 - LED3 (red) turns ON when there is an error in transmission/reception process.  

@note SPIx instance used and associated resources can be updated in "main.h"
       file depending hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, SPI, Full-duplex, Polling, Transmission, Reception, Master, Slave, MISO, MOSI,
Command, acknowledge

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

  - SPI/SPI_FullDuplex_AdvComIT/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - SPI/SPI_FullDuplex_AdvComIT/Inc/stm32f7xx_it.h          SPI interrupt handlers header file
  - SPI/SPI_FullDuplex_AdvComIT/Inc/main.h                  Main program header file  
  - SPI/SPI_FullDuplex_AdvComIT/Src/stm32f7xx_it.c          SPI interrupt handlers
  - SPI/SPI_FullDuplex_AdvComIT/Src/main.c                  Main program
  - SPI/SPI_FullDuplex_AdvComIT/Src/system_stm32f7xx.c      STM32F7xx system clock configuration file
  - SPI/SPI_FullDuplex_AdvComIT/Src/stm32f7xx_hal_msp.c     HAL MSP module
  

@par Hardware and Software environment
  
  - This example runs on STM32F767ZI devices.
      
  - This example has been tested with NUCLEO-F767ZI Rev B board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F767ZI Rev B Set-up
    - Use LED1, LED2 and LED3 connected respectively to PA.5, PB.7, PB.14
    - Use the User Push Button 
    - Connect Master board PB3 to Slave Board PB3 (Stuino CN7, pin 15)
    - Connect Master board PA6 to Slave Board PA6 (Arduino D12)
    - Connect Master board PA7 to Slave Board PA7 (Arduino D11)
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
