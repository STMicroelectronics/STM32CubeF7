/**
  ******************************************************************************
  * @file    Templates/ExtMem_Boot/Src/fmc.c
  * @author  MCD Application Team
  * @brief   This file includes the fmc-memories supported 
  *          STM32F7308-Discovery evaluation boards.
  @verbatim
  PartNumber supported by the file:
  -----------------------
   - IS61WV51216BLL-10M  : PSRAM external memory mounted on STM32F7308 Discovery board.

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "memory.h"
#include "memory_msp.h"

/** @addtogroup FMC
  * @{
  */ 
  
/** @defgroup FMC
  * @{
  */ 

/** @defgroup FMC_Private_Types_Definitions FMC Private Types Definitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup FMC_Private_Defines FMC Private Defines
  * @{
  */
/**
  * @}
  */ 

/** @defgroup FMC_Private_Macros FMC Private Macros
  * @{
  */  
/**
  * @}
  */ 

/** @defgroup FMC_Private_Variables FMC Private Variables
  * @{
  */       
/**
  * @}
  */ 
    
/** @defgroup FMC_Private_Functions FMC Private Functions
  * @{
  */

#if (DATA_AREA == USE_EXTERNAL_PSRAM) || (CODE_AREA == USE_EXTERNAL_PSRAM)
/** @defgroup PSRAM_Constants PSRAM Defines
  * @{
  */
/* #define PSRAM_MEMORY_WIDTH    FMC_NORSRAM_MEM_BUS_WIDTH_8*/
#define PSRAM_MEMORY_WIDTH    FMC_NORSRAM_MEM_BUS_WIDTH_16

#define PSRAM_BURSTACCESS     FMC_BURST_ACCESS_MODE_DISABLE
/* #define PSRAM_BURSTACCESS     FMC_BURST_ACCESS_MODE_ENABLE*/
  
#define PSRAM_WRITEBURST      FMC_WRITE_BURST_DISABLE
/* #define PSRAM_WRITEBURST     FMC_WRITE_BURST_ENABLE */
 
#define PSRAM_CONTINUOUSCLOCK    FMC_CONTINUOUS_CLOCK_SYNC_ONLY 
/* #define PSRAM_CONTINUOUSCLOCK     FMC_CONTINUOUS_CLOCK_SYNC_ASYNC */  
/**
  * @}
  */
/**
  * @brief  Initializes the PSRAM device.
  * @retval PSRAM status
  */
uint32_t PSRAM_Startup(void)
{
  static FMC_NORSRAM_TimingTypeDef Timing;  
  SRAM_HandleTypeDef psramHandle;
  
  /* PSRAM device configuration */
  psramHandle.Instance = FMC_NORSRAM_DEVICE;
  psramHandle.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  
  /* PSRAM device configuration */
  /* Timing configuration derived from system clock (up to 216Mhz)
     for 108Mhz as PSRAM clock frequency */
  Timing.AddressSetupTime      = 9;
  Timing.AddressHoldTime       = 2;
  Timing.DataSetupTime         = 6;
  Timing.BusTurnAroundDuration = 1;
  Timing.CLKDivision           = 2;
  Timing.DataLatency           = 2;
  Timing.AccessMode            = FMC_ACCESS_MODE_A;
  
  psramHandle.Init.NSBank             = FMC_NORSRAM_BANK1;
  psramHandle.Init.DataAddressMux     = FMC_DATA_ADDRESS_MUX_DISABLE;
  psramHandle.Init.MemoryType         = FMC_MEMORY_TYPE_SRAM;
  psramHandle.Init.MemoryDataWidth    = PSRAM_MEMORY_WIDTH;
  psramHandle.Init.BurstAccessMode    = PSRAM_BURSTACCESS;
  psramHandle.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  psramHandle.Init.WaitSignalActive   = FMC_WAIT_TIMING_BEFORE_WS;
  psramHandle.Init.WriteOperation     = FMC_WRITE_OPERATION_ENABLE;
  psramHandle.Init.WaitSignal         = FMC_WAIT_SIGNAL_DISABLE;
  psramHandle.Init.ExtendedMode       = FMC_EXTENDED_MODE_DISABLE;
  psramHandle.Init.AsynchronousWait   = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  psramHandle.Init.WriteBurst         = PSRAM_WRITEBURST;
  psramHandle.Init.WriteFifo          = FMC_WRITE_FIFO_DISABLE;
  psramHandle.Init.PageSize           = FMC_PAGE_SIZE_NONE;  
  psramHandle.Init.ContinuousClock    = PSRAM_CONTINUOUSCLOCK;
  
  /* PSRAM controller initialization */
  PSRAM_MspInit();
  if (HAL_SRAM_Init(&psramHandle, &Timing, &Timing) != HAL_OK)
  {
    return MEMORY_ERROR;
  }
  return MEMORY_OK;
}
#endif /*(DATA_AREA == USE_EXTERNAL_PSRAM) || (CODE_AREA == USE_EXTERNAL_PSRAM)*/

/**
  * @}
  */  
  
/**
  * @}
  */ 
  
/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
