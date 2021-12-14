/**
  @page LTDC_PicturesFromSDCard LTDC pictures from SD card application
  
  @verbatim
  ******************************************************************************
  * @file    Display\LTDC_PicturesFromSDCard/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LTDC pictures from SD card application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description

This application describes how to display pictures saved in SD card, on LCD .
 
The user has to create a "Media" directory under the micro SD card's root and
copy inside it the pictures stored in "/BMP_320x240" and "/BMP_480x272" folders provided under 
"/Utilities/Media/Pictures"
   
 
@note : The user can add pictures of his choice but the file type, resolution
        and pixel format described below must be respected.
  
  At the beginning, the program checks the existence of an SD card. If the SD card
  is not inserted, a warning message is displayed on the LCD :" Please insert SD Card. "
  Once the LCD, SD card, and file systems are initialized and configured, 
  a content check of the "Media" directory is done and the number of ".BMP" files 
  is retained.
  
@note : An associate LCD discovery driver is used in this application 
  
@note : The maximum number of BMP files is fixed to 25. It can be raised 
        until reaching the maximum of SD card memory space.

  The following steps are performed to scroll all the images stored in the 
  SD Card :
 
  Step1 :
  ------
  The foreground layer is set. Afterwards, The image is copied from SD card to intermediate
  SDRAM memory and then copied to LCD frame buffer. The image is fully visible when 
  the level of transparency is increased until it becomes totally opaque (reaches 255).
 
  Press user button to display next image.
 
  Step2 :
  ------
  When the user button is pressed, the transparency of the foreground layer
  begins to decrease until becoming totally transparent and the background layer 
  is set. The image is copied from SD card to intermediate SDRAM memory and then 
  copied to LCD frame buffer. Finally, the level of transparency of the 
  background layer increases until it reaches 255 (totally opaque). 
            
  Press user button to display next image.
 
  Step3 :
  ------
  When the Tamper button is pressed the transparency of the background layer
  begins to decrease until becoming totally transparent and returns to step 1.
  
@note the system clock (SYSCLK) is configured to run at 200 MHz. 50 MHz are provided 
      at the output PLL divided by PLL_Q. This frequency permits to reach the 25 MHz clock frequency
      needed for SD operation and in line with microSD specification. 
      
@note 
  => If the "Media" directory is empty, a warning message is displayed on 
     the LCD : "  No Bitmap files...  "
    
  => If the file type stored in the "Media" directory is not supported,
     a warning message is displayed on the LCD : " file type not supported. "
    
  => If the SD card is removed while running the application, no warning message
     is displayed on the LCD. A reset is needed.
   
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this application uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.
      
- LED1 is ON when an error occurs.

@par Keywords

Graphic, Display, LTDC, LCD, Pictures, SD Card, BMP, RGB565, RGB888, ARGB8888

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

    - Display/LTDC_PicturesFromSDCard/Inc/main.h                             Main configuration file
    - Display/LTDC_PicturesFromSDCard/Inc/stm32f7xx_it.h                     Interrupt handlers header file
    - Display/LTDC_PicturesFromSDCard/Inc/stm32f7xx_hal_conf.h               HAL Configuration file 
    - Display/LTDC_PicturesFromSDCard/Inc/ffconf.h                           FAT file system module configuration file
    - Display/LTDC_PicturesFromSDCard/Inc/fatfs_storage.h                    Header for fatfs_storage.c
    - Display/LTDC_PicturesFromSDCard/Inc/sd_diskio.h                        SD interface for FatFS header file
    - Display/LTDC_PicturesFromSDCard/Src/main.c                             Main program 
    - Display/LTDC_PicturesFromSDCard/Src/fatfs_storage.c                    Storage (FatFs) driver
    - Display/LTDC_PicturesFromSDCard/Src/stm32f7xx_it.c                     Interrupt handlers
    - Display/LTDC_PicturesFromSDCard/Src/system_stm32f7xx.c                 STM32F7xx system clock configuration file
    - Display/LTDC_PicturesFromSDCard/Src/sd_diskio.c                        SD interface for FatFS source file
    

@par Hardware and Software environment  

  - This application runs on STM32F756xx/STM32F746xx devices.
    
  - This application has been tested with STMicroelectronics STM32746G-DISCOVERY boards and can be
    easily tailored to any other supported device and development board.
    
  - STM32746G-DISCOVERY Set-up
    - Connect a uSD Card to the MSD connector (CN3).

@par How to use it ?

In order to make the program work, you must do the following :
 - The bitmap images, available under "/BMP_320x240" and "/BMP_480x272" folders 
   provided under "/Utilities/Media/Pictures" should be copied inside a 
   dedicated directory named "Media" at the micro SD card root.
   The images should have the following properties:
     file type    : *.bmp
     resolution   : up to 480x272
     pixel format : RGB565, RGB888 or ARGB8888     
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application 
                

 */
                                   
