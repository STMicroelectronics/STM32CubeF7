/**
  @page Camera_To_USBDisk Camera to USB Disk application
  
  @verbatim
  ******************************************************************************
  * @file    Camera/Camera_To_USBDisk/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Camera to USB Disk application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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

This application provides a short description of how to use the DCMI to interface with
camera module and display in continuous mode the picture on LCD and to save a picture 
in USB device.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 200 MHz.

The Digital camera interface is configured to receive the capture from
the camera module mounted on STM32F769I_EVAL evaluation board.
DMA2 Stream1 channel1 is configured to transfer the picture from DCMI peripheral
to an external RAM used by the LCD as a frame buffer.   

The camera module is configured to generate (480x272) image resolution
and the LCD is configured to display in resolution WVGA(800x480) in orientation landscape.

When the tamper button is pressed an image is saved under USBdisk.
  - LED1 is on to indicate the end of saving operation and a message is displayed 
    on LCD to indicate the beginning and the end of the saving operation.
  - LED3 is ON when any error occurs.
  
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, Camera, USB Disk, DCMI, LCD, DMA, Frame buffer, WVGA

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

    - Camera/Camera_To_USBDisk/Inc/stm32f7xx_hal_conf.h    HAL configuration file
    - Camera/Camera_To_USBDisk/Inc/main.h                  Main program header file
    - Camera/Camera_To_USBDisk/Inc/stm32f7xx_it.h          Interrupt handlers header file
    - Camera/Camera_To_USBDisk/Inc/ffconf.h                FAT file system module configuration file 
    - Camera/Camera_To_USBDisk/Inc/usbh_conf.h             Header for usbh_conf.c
    - Camera/Camera_To_USBDisk/Src/usbh_diskio.h           USB host diskio header file
    - Camera/Camera_To_USBDisk/Src/main.c                  Main program  
    - Camera/Camera_To_USBDisk/Src/stm32f7xx_it.c          Interrupt handlers   
    - Camera/Camera_To_USBDisk/Src/system_stm32f7xx.c      STM32F7xx system clock configuration file
    - Camera/Camera_To_USBDisk/Src/usbh_conf.c             USB configuration file.
    - Camera/Camera_To_USBDisk/Src/usbh_diskio.c           USB host diskio file


@par Hardware and Software environment
  
  - This application runs on STM32F769xx and STM32F779xx devices.
  
  - This application has been tested with STM32F769I-EVAL board which includes
    the KoD DSI LCD and S5K5CAG camera module.

  - STM32F769I-EVAL Set-up
    - Plug the USB key into the STM32F769I-EVAL board through 'USB micro A-Male 
      to A-Female' cable (HS mode: connector CN8).
    - Make sure jumper JP10 of STM32F769I-EVAL board is not fitted.
    
    
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
                                   
