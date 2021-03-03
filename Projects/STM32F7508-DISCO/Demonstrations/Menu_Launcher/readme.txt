/**
  @page Menu_Launcher Firmware  Readme file

  @verbatim
  ******************************************************************************
  * @file    Demonstrations/Menu_Launcher/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the Menu-Launcher Firmware 
  ******************************************************************************
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. All rights reserved.
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

The Menu Launcher firmware contains a boot code which enable SDRAM and QSPI memory
used for the Demo (STemWin & TouchGFx) and run a startup graphic Menu with two button
to allow user the selection between the two Demonstration (StemWin & TouchGFX).

@note You must not touch any part of the system that been initialized by the
      Menu Launcher firmware to avoid system failure.

The STM32F750xx value line come with only 64KB internal flash.
This value line are intended for code execution from external memories.
This application shows how to boot from internal flash, configure external memories
then jump to the selected Demo application located on external memory.

@par Keywords

MenuLauncher, Demo, External Memory Boot, Loader, Boot

@Note If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
      then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
      In case of constraints it is possible to configure the MPU as "Write through/not shareable" to guarantee the cache coherence at write access but the user
      has to ensure the cache maintenance at read access though.
      The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
      In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
      Even though the user must manage the cache coherence for read accesses.
      Please refer to the AN4838 (Managing memory protection unit (MPU) in STM32 MCUs)
      Please refer to the AN4839 (Level 1 cache on STM32F7 Series)

(1) Demo location address is defined in the main.c file as:
          #define STEMWIN_DEMO_ADDRESS   (uint32_t)(0x90000000)
          #define TOUCHGFX_DEMO_ADDRESS  (uint32_t)(0x90780000)

@par How to rebuild and flash the OoB Demo from source code?

In order to rebuild from source code and load the Out of the Box Demonstration to the STM32 target :
 - Open your preferred toolchain for all below sub modules projects :
   - Menu_Launcher
   - STemWin
   - TouchGFX
 - Rebuild all projects.
 - Open the STM32CubeProgrammer tool
 - Select the QSPI external flash loader "N25Q128A_STM32F7508-DISCO"
 - From Erasing & Programming menu :
   - Browse and open the output binary file relative to STemWin Sub-Demo project
   - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)
   - Browse and open the output binary file relative to TouchGFX Sub-Demo project
   - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90780000)
   - Browse and open the output binary file relative to Menu_Launcher project
   - Load the file into the internal flash


@par How to regenerate and flash the OoB Demo Full HEX file?

In order to regenerate the OoB Demo Full HEX file you will need to concatenate the Menu_Launche, STemWin and TouchGFX HEX files.
For instance you can use the provided windows batch script 'generate_OoB_hexfiles.bat' located in the Menu_Launcher Binary folder.
This script is based on the free 'SRecord' tools and require to have ALL three HEX files in same location. Please make sure that you
copy each rebuilt HEX file from each project before generating the Full HEX file. Below steps are :
 - Download 'SRecord' tool from http://srecord.sourceforge.net/windows.html
 - Copy the tool in CubeFW/Firmware/Utilities/PC_Software/HexTools location
 - Copy All rebuilt HEX files from each Sub-Demo project folder to 'Binary' folder.
 - Run 'generate_OoB_hexfiles.bat' script.
 - Open the STM32CubeProgrammer tool
 - Select the QSPI external flash loader "N25Q128A_STM32F7508-DISCO"
 - From Erasing & Programming menu browse and open the new generated STM32F7508-DISCO_Full_Demo_VX.Y.Z.hex
 - Load the file into the internal flash and external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)


@par How to use it?

Rebuild and then flash the OoB Demo from source (refer to below section)
Reset the board and then select one of the STemWin or TouchGFX Sub-Demo to run by clicking on corresponding icon on the screen.
The target should be then automatically reset and the main screen for the selected Sub-Demo should appear.
Please refer to readme.txt file on each Sub-Demo project folder for more details on how to use it.

@Note Please make sure that ALL modules (Menu_Launcher, STemWin and TouchGFX) were successfully loaded on the target before using the demo.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
