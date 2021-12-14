/**
  @page LCD_AnimatedPictureFromSDCard LCD animated picture from SD card application

  @verbatim
  ******************************************************************************
  * @file    Display/LCD_AnimatedPictureFromSDCard/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LCD animated picture from SD card application.
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

  This application describes how to display an animated picture on LCD saved under
  microSD.

  The animated picture is the display of an images sequence with a determined
  frequency to be seen like a animated image.

  The user has to copy the two directories "BACK" and "TOP" available under
  "/Utilities/Media/Pictures/Animated" to the micro SD card root.

  * Background picture display
  ----------------------------
  At the beginning, the program checks the existence of an SD card. If the SD card
  is not inserted, a warning message is displayed on the LCD :" Please insert SD Card. "

  Once the LCD, SD card and file system initialized and configured, a check
  of the existence and the content of the "/BACK" directory is done.

  * Foreground animated pictures display
  -------------------------------------
  A content check of the "/TOP" directory is done and the number of ".BMP" files
  is retained.

@note : The maximum number of BMP files is fixed at 25. It can be raised
        until reaching the maximum of SD card memory space.

@note : the system clock (SYSCLK) is configured to run at 200 MHz and 50 MHz is provided
        at the output PLL divided by PLL_Q. This frequency permits to reach 25 MHz clock
        needed for SD operation and in line with microSD specification.

  The following steps are performed to scroll all the images stored in the
  SD Card :

  - The foreground layer is set, the image copied from SD card (from "/TOP" directory)
    to intermediate SDRAM memory space and then copied to LCD frame buffer.

  - The color keying feature is applied to remove the bottom of foreground layer (transparent)
    and then replaced by the background layer.

  - Jump to next image

@note :
  => If the SD card is not inserted, a warning message is displayed on the LCD :" open directory.. fails. "

  => If the file type stored in the "/BACK" or "/TOP" directories is not supported,
     a warning message is displayed on the LCD : " file type not supported. "

  => If the SD card is removed while running the application, A reset is needed.

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

-  LED3 is ON when the uSD disk I/O driver is not Linked;

@par Keywords

Display, Graphic, LCD, Pictures, SD Card, BMP, Background, Foreground, Layers,

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

    - Display/LCD_AnimatedPictureFromSDCard/Inc/main.h                 Main configuration file
    - Display/LCD_AnimatedPictureFromSDCard/Inc/stm32f7xx_it.h         Interrupt handlers header file
    - Display/LCD_AnimatedPictureFromSDCard/Inc/stm32f7xx_hal_conf.h   HAL Configuration file
    - Display/LCD_AnimatedPictureFromSDCard/Inc/ffconf.h               FAT file system module configuration file
    - Display/LCD_AnimatedPictureFromSDCard/Inc/fatfs_storage.h        Header for fatfs_storage.c
    - Display/LCD_AnimatedPictureFromSDCard/Src/sd_diskio.h            FatFS sd diskio driver header file
    - Display/LCD_AnimatedPictureFromSDCard/Src/main.c                 Main program
    - Display/LCD_AnimatedPictureFromSDCard/Src/fatfs_storage.c        Storage (FatFs) driver
    - Display/LCD_AnimatedPictureFromSDCard/Src/sd_diskio.c            FatFS sd diskio driver implmentation
    - Display/LCD_AnimatedPictureFromSDCard/Src/stm32f7xx_it.c         Interrupt handlers
    - Display/LCD_AnimatedPictureFromSDCard/Src/system_stm32f7xx.c     STM32F7xx system clock configuration file


@par Hardware and Software environment

  - This application runs on STM32F769xx and STM32F779xx devices.

  - This application has been tested with STMicroelectronics STM32F769I-EVAL
    evaluation boards and can be easily tailored to any other supported device
    and development board.

  - STM32F769I-EVAL Set-up
    - Plug a micro SD card into CN17 connector.

@par How to use it ?

In order to make the program work, you must do the following :
 - The two directories "BACK" and "TOP" under "/Utilities/Media/Pictures/Animated"
   folder must be copied at the micro SD card root.
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application


 */
