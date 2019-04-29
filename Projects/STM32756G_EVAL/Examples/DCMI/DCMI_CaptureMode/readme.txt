/**
  @page DCMI_CaptureMode DCMI Capture Mode example
  
  @verbatim
  ******************************************************************************
  * @file    DCMI/DCMI_CaptureMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32F7xx DCMI_CaptureMode example.
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

This example provides a short description of how to use the DCMI to interface with
a camera module and to display in continuous mode the picture on the LCD.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 200 MHz.

The Digital camera interface is configured to receive the capture from
the camera module mounted on STM327x6G-EVAL revB evaluation board.
The DMA is configured to transfer the picture from DCMI peripheral
to an external RAM used by the LCD as a frame buffer.   
When each line event callback is raised the pixels coming from sensor are converted from 
RGB565 to RGB0888 format using DMA2D and transferred to the LCD frame buffer.   

The camera module is configured to generate QVGA (320x240) image resolution
and the LCD is configured to display QVGA image resolution
It also possible to test other resolutions. To do, uncomment one of the
following lines in main.h:
/* #define TEST_RESOLUTION_R160x120         */
/* #define TEST_RESOLUTION_R320x240         */
/* #define TEST_RESOLUTION_R480x272         */
/* The following resolusion is valid only on MB1063 */
/* #define TEST_RESOLUTION_R640x480         */       

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

  - DCMI/DCMI_CaptureMode/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - DCMI/DCMI_CaptureMode/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - DCMI/DCMI_CaptureMode/Inc/main.h                  Header for main.c module  
  - DCMI/DCMI_CaptureMode/Src/stm32f7xx_it.c          Interrupt handlers
  - DCMI/DCMI_CaptureMode/Src/main.c                  Main program
  - DCMI/DCMI_CaptureMode/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F756xx/STM32F746xx devices.
    
  - This example has been tested with STM327x6G-EVAL board revB and can be
    easily tailored to any other supported device and development board.

  - STM32756G_EVAL Set-up: Make sure that jumper JP23 is on CAMERA position.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
