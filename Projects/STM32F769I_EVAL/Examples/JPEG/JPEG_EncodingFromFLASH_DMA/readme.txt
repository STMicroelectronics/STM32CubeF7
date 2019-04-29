/**
  @page JPEG_EncodingFromFLASH_DMA : JPEG example : JPEG encoding with DMA of an image stored in FLASH example

  @verbatim
  ******************************************************************************
  * @file    JPEG/JPEG_EncodingFromFLASH_DMA/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the JPEG encoding from FLASH and with DMA example.
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

This example demonstrates how to read an RGB image stored in the internal FLASH, encode it
using the JPEG HW encoder in DMA mode and save it in SDCard.

The example performs the following actions :
  - Link the uSD disk I/O driver.
  - Register the file system object (mount) to the FatFs module for the uSD drive.
  - Create the JPEG file with write access.
  - Call the function "JPEG_Encode_DMA" to start encoding the jpg file with DMA (Non blocking method).
    the RGB input image nameis automatically selected according to the selected RGB format as follow :
    - Image_RGB888 for RGB888 (#define JPEG_RGB_FORMAT      JPEG_RGB888)
    - Image_RGB565 for RGB565 (#define JPEG_RGB_FORMAT      JPEG_RGB565)
    - Image_ARGB8888 for ARGB8888 (#define JPEG_RGB_FORMAT      JPEG_ARGB8888)
    Function "JPEG_Encode_DMA" is implemented in a separate module encode_dma.c.
    This module is a wrapper on top of the JPEG HAL driver
  - Save encoded data in jpg file
  - Once the encoding ends, close the jpg file.
  - The Green LED1 is On if the test ends properly.
  - The Red LED3 is On in case of error.

@note The jpeg HW encoder inputs are organized in YCbCr blocks.
      The jpeg utilities functions implemented in "jpeg_utils.c" file are used to convert the input RGB pixels
      to YCbCr blocks.
      The JPEG utilities files allowing to convert RGB pixels to YCbCr blocks are available under \Utilities\JPEG

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed
      at a maximum system clock frequency of 200MHz.

@note The compiler optimizations level could be set to High/Balanced giving a good tradeoff between the code size and the performance (decode time).

@note In case of EVAL's LCD use in example code, the DATA_IN_ExtSDRAM define
      need to be added  in the project preprocessor to avoid SDRAM multiple configuration
     (the LCD uses SDRAM as frame buffer, and its configuration is done by the BSP_SDRAM_Init()).

@par Keywords

Display, Graphic, JPEG, Encoding, Flash memory, DMA, ARGB8888, Hardware Encoding, LCD, HDMI, YCbCr, RGB,
SD Card, SDRAM

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

  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/stm32f7xx_hal_conf.h          HAL configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/stm32f7xx_it.h                Interrupt handlers header file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/ffconf.h                      FAT file system module configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/main.h                        Header for main.c module
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/encode_dma.h                  Header for encode_dma.c module
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/jpeg_utils_conf.h             JPEG utilties module configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Inc/image_320_240_rgb.h           Specification of RGB image to load in FLASH

  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/stm32f7xx_it.c                Interrupt handlers
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/main.c                        Main program
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/encode_dma.c                  JPEG Encoding with DMA module
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/stm32f7xx_hal_msp.c           HAL MSP module
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/system_stm32f7xx.c            STM32F7xx system source file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/Image_argb_8888.c             Example of ARGB888 image to load in FLASH
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/Image_rgb_565.c               Example of RGB565 image to load in FLASH
  - JPEG/JPEG_EncodingFromFLASH_DMA/Src/Image_rgb_888.c               Example of RGB888 image to load in FLASH


@par Hardware and Software environment

  - This example runs on STM32F769xx/STM32F779xx devices.

  - This example has been tested with STMicroelectronics STM32F769I_EVAL revB
    evaluation boards and can be easily tailored to any other supported device
    and development board.

  - STM32F769I_EVAL Set-up
  - Connect the uSD Card to the uSD1 connector (CN17).

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */