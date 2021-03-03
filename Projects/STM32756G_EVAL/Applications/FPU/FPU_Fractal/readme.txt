/**
  @page FPU_Fractal Floating Point Unit application.
  
  @verbatim
  ******************************************************************************
  * @file    FPU/FPU_Fractal/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FPU Fractal application.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. All rights reserved.
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
  
  This application explains how to use, and demonstrates the benefits brought by, the STM32F7 floating-point
  units (FPU). The CortexM7 FPU is an implementation of the ARM FPv5-SP Single-precision FPU.
  
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
  
  Optimization level is set to High-Balanced to strike a balance between code footprint and execution speed.
  
  STM32 Eval board's LEDs can be used to monitor the application status:
  - LED1 is ON in case of succes
  - LED3 is on in case of error.

@note For more information on how to use floating-point units (FPU)
      with STM32F405/07xx and STM32F415/417xx microcontrollers refer to AN4044 :  
      "Using floating-point unit (FPU) with STM32F405/07xx and STM32F415/417xx microcontrollers"
      Found under:
      http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00047230.pdf

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note to correctly display the fractals, the board should be equipped with the MB 1063 boad, (large LCD
      
@par Keywords

Display, Graphic, FPU, Fractal, ARM FPv5-SP, Single precision, Julia Set, 
      
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

  - FPU/FPU_Fractal/inc/stm32f7xx_hal_conf.h           HAL Configuration file 
  - FPU/FPU_Fractal/inc/stm32f7xx_it.h                 Interrupt handlers header file
  - FPU/FPU_Fractal/inc/main.h                         Main program header file
  - FPU/FPU_Fractal/Inc/button.h                       Pause/Play and zoom buttons header file  
  - FPU/FPU_Fractal/src/stm32f7xx_it.c                 Interrupt handlers
  - FPU/FPU_Fractal/src/main.c                         Main program
  - FPU/FPU_Fractal/src/system_stm32f7xx.c             STM32F7xx system source file
  - FPU/FPU_Fractal/Src/button.c                       Pause/Play and zoom buttons tables  
  - FPU/FPU_Fractal/src/stm32f7xx_hal_msp.c            HAL MSP file

@par Hardware and Software environment

  - This example runs on STM32F7xx Devices.
  
  - This application has been tested with STMicroelectronics STM327x6G-EVAL 
    evaluation boards and can be easily tailored to any other supported device and development board.
    
  - STM327x6G-EVAL Set-up   
    - To use LED1, ensure that JP24 is in position 2-3
    - To use LED3, ensure that JP23 is in position 2-3


@par How to use it? 
  In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Select FPU On or FPU Off workspace
  - Rebuild all files and load your image into target memory
  - Run the example
    
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
