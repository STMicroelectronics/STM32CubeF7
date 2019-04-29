/**
  @page LCD_PicturesFromUSB LCD pictures from USBapplication
  
  @verbatim
  ******************************************************************************
  * @file    Display/LCD_PicturesFromUSB/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LCD pictures from USB card application.
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

This application describes how to display pictures on LCD saved under USB disk.
 
Prior to starting the application, the user has to create a "/Media" directory under the USB disk root and
copy the pictures stored in PC directory "Utilities/Media/Pictures".
    The images should have the following properties:
     file type    : *.bmp
     resolution   : up to 240x240.
     pixel format : RGB565
     file name    : up to 7 characters
 
@note : The user can add pictures of his choice but the file type, resolution
        and pixel format described below must be respected.
  
  Once the LCD, USB and file systems initialized and configured, 
  a content check of the "Media" directory is done and the number of ".BMP" files 
  is retained.
  
@note : An associate LCD discovery driver is used in this application 
  
@note : The maximum number of BMP files is fixed to 25. It can be raised 
        until reaching the maximum of USB memory space.

  The following steps are performed to scroll all the images stored in the 
  USB disk :
 
  Step1 :
  ------
  The image copied from USB disk to LCD frame buffer located in PSRAM memory. The
  image is the displayed when the lCD memory is refreshed.
  Each image is displayed 2 seconds
 
  Step2 :
  ------
  When all image are displayed, the image counter is reset and the images are
  displayed again.
      
@note 
  => If the "Media" directory is empty, a warning message is displayed on 
     the LCD : "  No Bitmap files...  

  => If the file type stored in the "Media" directory is not supported,
     a warning message is displayed on the LCD : " No Bitmap files... "
    
  => If the USB disk is removed while running the application, a warning message
     is displayed on the LCD. The application restart once the USB disk is pluged
     again.
   
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


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

    - Display/LCD_PicturesFromUSB/Inc/main.h                             Main configuration file
    - Display/LCD_PicturesFromUSB/Inc/stm32f7xx_it.h                     Interrupt handlers header file
    - Display/LCD_PicturesFromUSB/Inc/stm32f7xx_hal_conf.h               HAL Configuration file 
    - Display/LCD_PicturesFromUSB/Inc/ffconf.h                           FAT file system module configuration file
    - Display/LCD_PicturesFromUSB/Inc/fatfs_storage.h                    Header for fatfs_storage.c
    - Display/LCD_PicturesFromUSB/Inc/lcd_log_conf.h                     LCD log configuration file
    - Display/LCD_PicturesFromUSB/Src/usbh_diskio.h                      USB diskio interface header file  
    - Display/LCD_PicturesFromUSB/Inc/usbh_conf.h                        USB Host driver Configuration file    
    - Display/LCD_PicturesFromUSB/Src/main.c                             Main program 
    - Display/LCD_PicturesFromUSB/Src/fatfs_storage.c                    Storage (FatFs) driver
    - Display/LCD_PicturesFromUSB/Src/file_operations.c                  Write/read file on the disk
    - Display/LCD_PicturesFromUSB/Src/usbh_diskio.c                      USB diskio interface for FatFs 
    - Display/LCD_PicturesFromUSB/Src/usbh_conf.c                        General low level driver configuration
    - Display/LCD_PicturesFromUSB/Src/stm32f7xx_it.c                     Interrupt handlers
    - Display/LCD_PicturesFromUSB/Src/system_stm32f7xx.c                 STM32F7xx system clock configuration file


@par Hardware and Software environment  

  - This application runs on STM32F723xx/STM32F733xx devices.
    
  - This application has been tested with STMicroelectronics STM32F723E-DISCO
    board and can be easily tailored to any other supported device 
    and development board.
    
  - STM32F723E-DISCO Set-up
    - Plug the USB key into the STM32F723IE_DISCOVERY board through 'USB micro A-Male 
      to A-Female' cable (HS mode: connector CN19).
    - USB_HS jumper should be fitted on CN8 connector.


@par How to use it ?

In order to make the program work, you must do the following :
 - The bitmap images, available under ".Media/" 
   should be copied inside a  dedicated directory named "Media" at USB key root.
   The images should have the following properties:
     file type    : *.bmp
     resolution   : up to 240x240
     pixel format : RGB565    
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application 
                
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
                                   