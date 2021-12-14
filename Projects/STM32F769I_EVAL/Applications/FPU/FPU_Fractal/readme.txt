/**
  @page FPU_Fractal Floating Point Unit application.
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    FPU/FPU_Fractal/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FPU Fractal application.
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
  
  This application explains how to use, and demonstrates the benefits brought by, the STM32F7 floating-point
  units (FPU). The CortexM7 FPU is an implementation of the ARM FPv5-SP double-precision FPU.
  
  The application computes a simple mathematical fractal: the Julia set.
  The generation algorithm for such a mathematical object is quite simple: for each point of
  the complex plan, we are evaluating the divergence speed of a defined sequence. The Julia
  set equation for the sequence is: z(n+1) = z(n)^2 + c.
  This value is translated into a color, to show graphically the divergence speed of the points of the complex plan.
  
  Two workspaces are available to activate or not the FPU during the compilation phase:
  - Without using the FPU, these operations are done by software through the C compiler
  library and are not visible to the programmer; but the performances are very low.
  - When enabling the FPU, all of the real numbers calculations are entirely done by hardware in a
  single cycle, for most of the instructions. The C compiler does not use its own floating-point
  library but directly generates FPU native instructions.
  
  User might change the number of iterations done while calculating the fractal for richer pixels generated,
  by chngaing the ITERATION define in main.h
  
  STM32F769I-EVAL Eval board's LEDs can be used to monitor the application status:
  - LED1 is ON in case of success.

@note For more information on how to use floating-point units (FPU) refer to AN4044 found under:
      http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00047230.pdf

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@par Keywords

Display, Graphic, FPU, Fractal, ARM FPv5-SP, Double precision, Julia Set, 
      
@Note If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
      then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
      The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@Note In this application The SDRAM is configured using the MPU as "Write through/not shareable" to guarantee the cache coherence 
      at write access between the CPU and other masters (LTDC, DMA2D).
      The SDRAM is used to locate the LCD frame buffer and  the CPU only performs write access (no read access) to the SDRAM that is safe with the Write through configuration.

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
      In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
      Even though the user must manage the cache coherence for read accesses.
      Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
      Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

@par Directory contents

  - FPU/FPU_Fractal/Inc/stm32f7xx_hal_conf.h           HAL Configuration file 
  - FPU/FPU_Fractal/Inc/stm32f7xx_it.h                 Interrupt handlers header file
  - FPU/FPU_Fractal/Inc/main.h                         Main program header file
  - FPU/FPU_Fractal/Inc/button.h                       Pause/Play and zoom buttons header file   
  - FPU/FPU_Fractal/Src/stm32f7xx_it.c                 Interrupt handlers
  - FPU/FPU_Fractal/Src/main.c                         Main program
  - FPU/FPU_Fractal/Src/button.c                       Pause/Play and zoom buttons tables
  - FPU/FPU_Fractal/Src/system_stm32f7xx.c             STM32F7xx system source file

@par Hardware and Software environment

  - This application runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx Devices.
  
  - This application has been tested with STMicroelectronics STM32F769I-EVAL 
    evaluation boards and can be easily tailored to any other supported device and development board.


@par How to use it? 

      In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Select FPU On or FPU Off workspace
  - Rebuild all files and load your image into target memory
  - Run the application
    

 */
