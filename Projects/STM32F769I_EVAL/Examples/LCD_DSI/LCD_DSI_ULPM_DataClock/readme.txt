/**
  @page LCD_DSI_ULPM_DataClock : LCD DSI example : enter and exit DSI ULPM Mode on data lanes 
  and clock lane while displaying a picture 800x480 (WVGA) 
  in landscape mode in DSI mode Video Burst on LCD screen.

  @verbatim
  ******************************************************************************
  * @file    LCD_DSI/LCD_DSI_ULPM_DataClock/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LCD DSI enter and exit DSI ULPM Mode on data lane 
  *          and clock lane example.
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

@par Example Description

How to use the embedded LCD DSI controller (using the LTDC and DSI Host IPs) to
drive the KoD LCD mounted on-board and manage entry and exit in DSI ULPM mode
on data and clock lanes.

In this mode, the DSI PHY state machine is entering a low power state
on data lane and clock lane.
The DSI Dphy_Clk should be derived from the clock tree PLL.PLLR (ck_plldsi) with a 60 MHz clock for instance
to allow a switch off of the PLL DPHY to save power during the ULPM phase.
When exiting from ULPM, the PLL DPHY is first switched back on and locked, then the DSI DPHY will exit
power down and display again as before.

In this example a WVGA landscape picture (800x480) is displayed on LCD in DSI Mode Video Burst.
On a Tamper button press from the user, the LCD display will be switched off. One second later, 
the DSI PHY Data and clock lane will enter ULPM mode. DphyClk is derived from PLL.PLLR, then
DPHY PLL is switched Off.

After 6 seconds in Off mode, The PLL DPHY is switched back on and locked,
then the ULPM on data and clock lanes will be exited in DPHY DSI state machine.
The LCD will then be switched back on and display the same image as before 
with a text notifying that the exit from ULPM was successful. 

The image candies_800x480_argb8888 is of format ARGB8888 and is initially copied from Flash to SDRAM Frame Buffer.
The LTDC is reading continuously the LCD Frame buffer from SDRAM, sent it to DSI Host which sends it in burst mode (DCS commands)
via the DSI DPHY to the KoD display that decodes DSI packets and refresh its internal Graphic RAM to display on glass.

The Frame number display is managed by Line Event callback in which the Frame number is incremented
each time a line event occurs. When entering to ULPM, The Frame number is unchanged until the
exit from this mode.

LED1 ON: DSI PHY Data and clock lane in ULPM mode
LED1 OFF: DSI PHY Data and clock lane in run mode
LED3 ON: an error occurred.
@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.
      
@par Keywords

Graphic, Display, LCD, DSI, MIPI Alliance, Ultra low power mode, Tearing effect, Partial refresh, Single buffer,
LTDC, WVGA, ARGB8888, SDRAM, Burst mode, Data lane, clock lane

@Note�If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
 �����then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
������The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
����� It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
������In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
������Even though the user must manage the cache coherence for read accesses.
������Please refer to the AN4838 �Managing memory protection unit (MPU) in STM32 MCUs�
������Please refer to the AN4839 �Level 1 cache on STM32F7 Series�

@par Directory contents

  - LCD_DSI/LCD_DSI_ULPM_DataClock/Inc/stm32f7xx_hal_conf.h          HAL configuration file
  - LCD_DSI/LCD_DSI_ULPM_DataClock/Inc/stm32f7xx_it.h                Interrupt handlers header file
  - LCD_DSI/LCD_DSI_ULPM_DataClock/Inc/main.h                        Header for main.c module
  - LCD_DSI/LCD_DSI_ULPM_DataClock/Inc/candies_800x480_argb8888.h    Image 800x480 in ARGB8888 to display on LCD
  - LCD_DSI/LCD_DSI_ULPM_DataClock/Src/stm32f7xx_it.c                Interrupt handlers
  - LCD_DSI/LCD_DSI_ULPM_DataClock/Src/main.c                        Main program
  - LCD_DSI/LCD_DSI_ULPM_DataClock/Src/stm32f7xx_hal_msp.c           HAL MSP file
  - LCD_DSI/LCD_DSI_ULPM_DataClock/Src/system_stm32f7xx.c            STM32F7xx system source file

@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.

  - This example has been tested with STMicroelectronics STM32F769I-EVAL
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example



 */
