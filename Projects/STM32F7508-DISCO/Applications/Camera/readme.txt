/**
  @page Camera Capture Modes application
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 STMicroelectronics *******************
  * @file    Camera/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32F7xx Camera Capture Modes application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

This example provides a short description of how to use the DCMI to interface with
a camera module and to display in continuous or snapshot modes the picture on the LCD.

In this application, the code is executed from QSPI external memory while data are in internal
SRAM memory.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 200 MHz.

The LCD is initialized to be used for pictures' display.
The TouchScreen module is initialized to allow user to switch between snapshot and
continous mode.

The Digital camera interface is configured to receive the capture, in continous mode,
from the OV9655 camera module that should be connected to STM32F7508-DISCO board.
The DMA is configured to transfer the picture from DCMI peripheral
to the external SDRAM memory used by the LCD as a frame buffer.   
When each line event callback is raised the pixels coming from sensor are converted from 
RGB565 to RGB0888 format using DMA2D and transferred to the LCD frame buffer.   

The camera module is configured to generate QVGA (320x240) image resolution.

To switch between snapshot and continuous modes, user have to ouch the right area
displayed on the LCD.

If the camera module is not plugged, a warning message is displayed on the LCD. User
have to plug OV9655 camera sensor and restart the application.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.
      
@par Keywords

Display, DCMI, Camera, Capture, Frame Buffer, LCD, ARGB8888, DMA, RGB565, SDRAM, DMA2D, QQVGA

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

  - Camera/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - Camera/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - Camera/Inc/main.h                  Header for main.c module  
  - Camera/Src/stm32f7xx_it.c          Interrupt handlers
  - Camera/Src/main.c                  Main program
  - Camera/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F750xx devices.
    
  - This example has been tested with STM32F7508-DISCO board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc.
     The default configuration is the right one: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with this application "Application/Camera" (see below).
  4. Start debugging user example or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image
      
In order to load the Camera application to the external memory :
 - Open your preferred toolchain 
 - Rebuild all files. 
 - Run & debug the program:
   - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
   - Using SW4STM32 :
       - Open the STM32CubeProgrammer tool
       - Select the QSPI external flash loader "N25Q128A_STM32F7508-DISCO" 
       - From Erasing & Programming menu, browse and open the output binary file relative to this application
       - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
