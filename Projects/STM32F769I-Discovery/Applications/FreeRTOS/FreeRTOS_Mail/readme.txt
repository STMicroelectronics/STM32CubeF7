/**
  @page FreeRTOS_Mail FreeRTOS mail example
 
  @verbatim
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_Mail/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS Mail example.
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

@par Description

How to use mail queues with CMSIS RTOS API.

This example creates two threads that send and receive mail
the mail to send/receive is a structure that holds three variables (var1 and var2 are uint32, var3 is a uint8)

One thread acts as a producer and the other as the consumer.  
The consumer is a higher priority than the producer and is set to block on mail receiving.

The Mail queue has space for one item. The producer allocates the mail and put it on the mail queue. 
As soon as the producer posts a mail on the queue the consumer will unblock, preempt the producer, 
get the mail and free it.  

Add the following variables to LiveWatch, the three producer values must respectively remain equals to the three consumer values all the time:
 - ConsumerValue1 must remain equal to ProducerValue1
 - ConsumerValue2 must remain equal to ProducerValue2
 - ConsumerValue3 must remain equal to ProducerValue3
  
LEDs can be used to monitor the example status:
  - LED2(Green Led) should toggle when the example runs successfully.
  - LED1(Red Led) toggle when any error occurs.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the 
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".

@par Keywords

RTOS, FreeRTOS, Thread, Mail, Queues, 

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

    - FreeRTOS/FreeRTOS_Mail/Src/main.c                         Main program
    - FreeRTOS/FreeRTOS_Mail/Src/stm32f7xx_hal_timebase_tim.c   HAL time base configuration
    - FreeRTOS/FreeRTOS_Mail/Src/stm32f7xx_it.c                 Interrupt handlers
    - FreeRTOS/FreeRTOS_Mail/Src/system_stm32f7xx.c             STM32F7xx system clock configuration file
    - FreeRTOS/FreeRTOS_Mail/Inc/main.h                         Main program header file
    - FreeRTOS/FreeRTOS_Mail/Inc/stm32f7xx_hal_conf.h           HAL Library Configuration file
    - FreeRTOS/FreeRTOS_Mail/Inc/stm32f7xx_it.h                 Interrupt handlers header file
    - FreeRTOS/FreeRTOS_Mail/Inc/FreeRTOSConfig.h               FreeRTOS Configuration file

@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with STM32F769I-DISCO board and can be
    easily tailored to any other supported device and development board. 
    

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
