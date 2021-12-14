/**
  @page FatFs_uSD_RTOS   FatFs with uSD card drive in RTOS mode application
 
  @verbatim
  ******************************************************************************
  * @file    FatFs/FatFs_uSD_RTOS/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FatFs with uSD card drive in RTOS mode application
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

This application provides a description on how to use STM32Cube firmware with FatFs 
middleware component as a generic FAT file system module, in order to develop an
application exploiting FatFs offered features with microSD drive in RTOS mode 
configuration. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 216 MHz and provide 50 MHz at the output PLL divided by PLL_Q. 
This frequency permit to reach 25 MHz clock needed for SD operation and in line 
with microSD specification. 

The application is based on writing and reading back a text file from a drive,
it creates a normal priority thread and it's performed using FatFs APIs to 
access the FAT volume as described in the following steps: 

 - Link the uSD disk I/O driver;
 - Register the file system object (mount) to the FatFs module for the uSD drive;
 - Create a FAT file system (format) on the uSD drive;
 - Create and Open new text file object with write access;
 - Write data to the text file;
 - Close the open text file;
 - Open text file object with read access;
 - Read back data from the text file;
 - Check on read data from text file;
 - Close the open text file;
 - Unlink the uSD disk I/O driver.

It is worth noting that the application manages any error occurred during the 
access to FAT volume, when using FatFs APIs. Otherwise, user can check if the
written text file is available on the uSD card.

It is possible to fine tune needed FatFs features by modifying defines values 
in FatFs configuration file “ffconf.h” available under the project includes 
directory, in a way to fit the application requirements. 

STM32 Eval board's LEDs can be used to monitor the application status:
  - LED1 is ON when the application runs successfully.
  - LED3 is ON when any error occurs.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set according to 
      the OS resources memory requirements of the application with +10% margin and rounded to the 
      upper Kbyte boundary.

For more details about FatFs implementation on STM32Cube, please refer to UM1721 "Developing Applications 
on STM32Cube with FatFs".


@par Keywords

FatFS, SD Card, File system, FAT Volume, Format, Mount, Read, Write,FreeRTOS, RTOS

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
 
  - FatFs/FatFs_uSD_RTOS/Inc/stm32f7xx_hal_conf.h             HAL configuration file
  - FatFs/FatFs_uSD_RTOS/Inc/stm32f7xx_it.h                   Interrupt handlers header file
  - FatFs/FatFs_uSD_RTOS/Inc/main.h                           Main program header file
  - FatFs/FatFs_uSD_RTOS/Inc/ffconf.h                         FAT file system module configuration file
  - FatFs/FatFs_uSD_RTOS/Inc/sd_diskio_dma_rtos.h             Header for sd_diskio_dma_rtos.c module
  - FatFs/FatFs_uSD_RTOS/Inc/FreeRTOSConfig.h                 FreeRTOS distribution file
  - FatFs/FatFs_uSD_RTOS/Src/stm32f7xx_hal_timebase_tim.c     HAL time base based on the hardware TIM Template.
  - FatFs/FatFs_uSD_RTOS/Src/sd_diskio_dma_rtos.c             SD Disk I/O driver  
  - FatFs/FatFs_uSD_RTOS/Src/stm32f7xx_it.c                   Interrupt handlers
  - FatFs/FatFs_uSD_RTOS/Src/main.c                           Main program
  - FatFs/FatFs_uSD_RTOS/Src/system_stm32f7xx.c               STM32F7xx system clock configuration file
 
 
@par Hardware and Software environment

  - This application runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This application has been tested with STMicroelectronics STM32F769I-EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32F769I-EVAL Set-up
    - Connect a uSD Card to the MSD connector (CN17).

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */
