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
a camera module to capture continuously camera images in a Camera Frame Buffer 
in external RAM and each time a full frame camera image is captured display it 
on the LCD in ARGB8888 format.
This use example implements a camera preview mode.

The Digital camera interface is configured to receive the capture from
the camera module mounted on STM32F769I-EVAL evaluation board.
A DMA Peripheral to Memory is programmed between DCMI and Camera Frame buffer 
to receive a several RGB565 resolutions in camera frame buffer 
in SDRAM in continuous mode.

On each DCMI frame event callback, the DMA2D is used to copy and convert 
a RGB565 frame from Camera frame buffer in a ARGB8888 frame in LCD Frame buffer also in SDRAM. 
The obtained LCD Frame buffer is displayed on LCD. DSI command mode is used to
control LCD refresh. The LCD is divided into tow area LEFT and RIGHT, once the
frame is ready to be displayed, the LEFT and RIGHT areas are refreshed. This is 
to avoid tearing effect issue that may happen when using DSI video mode.

The camera module is initially configured to generate QQVGA (160x120) image resolution
and the LCD is configured to display QQVGA image resolution. Press Tamper button
to test other resolutions:
/* RESOLUTION_R320x240         */
/* RESOLUTION_R480x272         */
/* RESOLUTION_R640x480         */

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 200 MHz.



@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

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
