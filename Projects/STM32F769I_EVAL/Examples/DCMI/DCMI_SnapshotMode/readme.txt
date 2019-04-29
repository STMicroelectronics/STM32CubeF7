/**
  @page DCMI_SnapshotMode DCMI Capture Mode example
  
  @verbatim
  ******************************************************************************
  * @file    DCMI/DCMI_SnapshotMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32F7xx DCMI_SnapshotMode example.
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

How to use the DCMI to interface with 
a camera module, capture a single image in Camera Frame Buffer (320x240 in RGB565) 
and once full frame camera is captured display it on the LCD in ARGB8888 format.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 200 MHz.

The Digital camera interface (DCMI) is configured to receive the capture from
the camera module mounted on STM32F769I-EVAL evaluation board.
A DMA Peripheral to Memory is programmed between DCMI and Camera Frame buffer to receive a 320x240 RGB565
in camera frame buffer in SDRAM or SRAM on a line by line basis.
When the DCMI End frame event callback is raised the picture is transferred to the LCD frame buffer.   

The camera module is configured to generate QVGA (320x240) image resolution
and the LCD is configured to display QVGA image resolution

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, DCMI, Camera, Snapshot, Frame Buffer, LCD, ARGB8888, DMA, RGB565, SDRAM, DMA2D, QQVGA

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

  - DCMI/DCMI_SnapshotMode/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - DCMI/DCMI_SnapshotMode/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - DCMI/DCMI_SnapshotMode/Inc/main.h                  Header for main.c module  
  - DCMI/DCMI_SnapshotMode/Src/stm32f7xx_it.c          Interrupt handlers
  - DCMI/DCMI_SnapshotMode/Src/main.c                  Main program
  - DCMI/DCMI_SnapshotMode/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with STM32F769I-EVAL board and can be
    easily tailored to any other supported device and development board.      

  - STM32F769I-EVAL Set-up :
       - JP10 must be removed

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
