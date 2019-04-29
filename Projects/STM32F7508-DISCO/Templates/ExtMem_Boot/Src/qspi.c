/**
  ******************************************************************************
  * @file    qspi.c
  * @author  MCD Application Team
  * @brief   This file includes a driver for QSPI flashes support mounted on
  *          STM32F7508-Discovery boards.
  @verbatim
  PartNumber supported by the file:
  -----------------------
   - N25Q128A13EF840E : QSPI Flash memory mounted on STM32F7508-Discovery board.
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
#include "../Components/n25q128a/n25q128a.h"
#if (CODE_AREA == USE_QSPI) || (BINARY_AREA == USE_SPI_NOR)
/** @addtogroup QSPI
  * @{
  */

/** @defgroup QSPI
  * @{
  */


/* Private variables ---------------------------------------------------------*/

/** @defgroup QSPI_Private_Variables QSPI Private Variables
  * @{
  */
QSPI_HandleTypeDef       QSPIHandle;
QSPI_CommandTypeDef      sCommand;
QSPI_AutoPollingTypeDef  sConfig;
/**
  * @}
  */

/* Private Macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup QSPI_Private_Functions QSPI Private Functions
  * @{
  */
static uint32_t QSPI_ResetMemory(QSPI_HandleTypeDef *hqspi);
#if (CODE_AREA == USE_QSPI)
static uint32_t QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi);
static uint32_t QSPI_EnterMemory_QPI(QSPI_HandleTypeDef *hqspi);
static uint32_t QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi);
static uint32_t QSPI_EnableMemoryMappedMode(QSPI_HandleTypeDef *hqspi);
#elif  (BINARY_AREA == USE_SPI_NOR)
static uint32_t QSPI_Shutdown(void);
static uint32_t QSPI_Copy(uint32_t WriteAddr, uint32_t ReadAddr, uint32_t Size);
#endif

/**
  * @}
  */

/** @defgroup QSPI_Exported_Functions QSPI Exported Functions
  * @{
  */

/**
  * @brief  Initializes and configure the QSPI interface.
  * @retval QSPI memory status
  */
uint32_t QSPI_Startup(void)
{
  QSPIHandle.Instance = QUADSPI;

  /* Call the DeInit function to reset the driver */
  if (HAL_QSPI_DeInit(&QSPIHandle) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* System level initialization */
  QSPI_MspInit();

  /* sCommand initialize common parameter */
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
  sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;

  /* sConfig initialize common parameter */
  sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
  sConfig.StatusBytesSize = 1;
  sConfig.Interval        = 0x10;
  sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  /* QSPI initialization */
  /* QSPI freq = SYSCLK /(1 + ClockPrescaler) = 216 MHz/(1+1) = 108 Mhz */
  QSPIHandle.Init.ClockPrescaler     = 1;   /* QSPI freq = 216 MHz/(1+1) = 108 Mhz */
  QSPIHandle.Init.FifoThreshold      = 4;
  QSPIHandle.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  QSPIHandle.Init.FlashSize          = POSITION_VAL(N25Q128A_FLASH_SIZE) - 1;
  QSPIHandle.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_6_CYCLE; /* Min 30ns for nonRead */
  QSPIHandle.Init.ClockMode          = QSPI_CLOCK_MODE_0;
  QSPIHandle.Init.FlashID            = QSPI_FLASH_ID_1;
  QSPIHandle.Init.DualFlash          = QSPI_DUALFLASH_DISABLE;

  if (HAL_QSPI_Init(&QSPIHandle) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* QSPI memory reset */
  if (QSPI_ResetMemory(&QSPIHandle))
  {
    return MEMORY_ERROR;
  }

#if (CODE_AREA == USE_QSPI)
  /* Put QSPI memory in QPI mode */
  if( QSPI_EnterMemory_QPI( &QSPIHandle )!=MEMORY_OK )
  {
    return MEMORY_ERROR;
  }

  /* Configuration of the dummy cycles on QSPI memory side */
  if (QSPI_DummyCyclesCfg(&QSPIHandle) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }

  /* Enable MemoryMapped mode */
  if( QSPI_EnableMemoryMappedMode( &QSPIHandle ) != MEMORY_OK )
  {
    return MEMORY_ERROR;
  }
#elif (BINARY_AREA == USE_SPI_NOR)
  if (QSPI_Copy(APPLICATION_ADDRESS, BINARY_BASE_OFFSET, BINARY_SIZE) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }

  if (QSPI_Shutdown() != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }
#endif /* (CODE_AREA == USE_QSPI) */

  return MEMORY_OK;
}

#if (BINARY_AREA == USE_SPI_NOR)
/**
  * @brief  De-Initializes and the QSPI interface.
  * @retval None
  */
static uint32_t QSPI_Shutdown(void)
{
  /* Call the DeInit function to reset the driver */
  if (HAL_QSPI_DeInit(&QSPIHandle) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* System level De-initialization */
  QSPI_MspDeInit();

  return MEMORY_OK;
}

/**
  * @brief  Copy an amount of data from the QSPI memory to destination memory.
  * @param  WriteAddr: Pointer to data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: Size of data to read
  * @retval QSPI memory status
  */
static uint32_t QSPI_Copy(uint32_t WriteAddr, uint32_t ReadAddr, uint32_t Size)
{
  QSPI_CommandTypeDef s_command;
  
  /* Initialize the read command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = FAST_READ_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
  s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_1_LINE;
  s_command.DummyCycles       = N25Q128A_DUMMY_CYCLES_READ;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  while (Size)
  {
    s_command.NbData            = (Size<256)?Size:256;
    s_command.Address           = ReadAddr;

    /* Configure the command */
    if (HAL_QSPI_Command(&QSPIHandle, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return MEMORY_ERROR;
    }

    /* Reception of the data */
    if (HAL_QSPI_Receive(&QSPIHandle, (uint8_t *)WriteAddr, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return MEMORY_ERROR;
    }

    if (Size > 256)
    {
      Size      -= 256;
      WriteAddr += 256;
      ReadAddr  += 256;
    }
    else
    {
      Size = 0;
    }
  }

  return MEMORY_OK;
}
#endif /* (BINARY_AREA == USE_SPI_NOR) */
/**
  * @}
  */

/** @addtogroup QSPI_Private_Functions
  * @{
  */

/**
  * @brief  This function reset the QSPI memory.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint32_t QSPI_ResetMemory(QSPI_HandleTypeDef *hqspi)
{
  /* Send command RESET command in SPI mode */
  /* Initialize the reset enable command */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = RESET_ENABLE_CMD;
  sCommand.DataMode          = QSPI_DATA_NONE;
  /* Send the command */
  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* Send the reset memory command */
  sCommand.Instruction = RESET_MEMORY_CMD;
  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  return MEMORY_OK;
}

#if (CODE_AREA == USE_QSPI)
/**
  * @brief  Configure the QSPI in memory-mapped mode
  * @retval QSPI memory status
  */
static uint32_t QSPI_EnableMemoryMappedMode(QSPI_HandleTypeDef *hqspi)
{
  QSPI_MemoryMappedTypeDef s_mem_mapped_cfg;

  /* Configure the command for the read instruction */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  sCommand.Instruction       = QUAD_INOUT_FAST_READ_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_4_LINES;
  sCommand.DataMode          = QSPI_DATA_4_LINES;
  sCommand.DummyCycles       = N25Q128A_DUMMY_CYCLES_READ_QUAD;

  /* Configure the memory mapped mode */
  s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
  s_mem_mapped_cfg.TimeOutPeriod     = 0;

  if (HAL_QSPI_MemoryMapped(hqspi, &sCommand, &s_mem_mapped_cfg) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  return MEMORY_OK;
}

/**
  * @brief  This function configure the dummy cycles on memory side.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint32_t QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef s_command;
  uint8_t reg;

  /* Initialize the read volatile configuration register command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = READ_VOL_CFG_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_1_LINE;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 1;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(hqspi, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* Enable write operations */
  if (QSPI_WriteEnable(hqspi) != MEMORY_OK)
  {
    return MEMORY_ERROR;
  }

  /* Update volatile configuration register (with new dummy cycles) */
  s_command.Instruction = WRITE_VOL_CFG_REG_CMD;
  MODIFY_REG(reg, N25Q128A_VCR_NB_DUMMY, (N25Q128A_DUMMY_CYCLES_READ_QUAD << POSITION_VAL(N25Q128A_VCR_NB_DUMMY)));

  /* Configure the write volatile configuration register command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* Transmission of the data */
  if (HAL_QSPI_Transmit(hqspi, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  return MEMORY_OK;
}
#define N25Q128A_SR_QUADEN                   ((uint8_t)0x40)    /*!< Quad IO mode enabled if =1 */
#define N25Q128A_SR_SRWREN                   ((uint8_t)0x80)    /*!< Status register write enable/disable */
/**
  * @brief  This function put QSPI memory in QPI mode (quad I/O).
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint32_t QSPI_EnterMemory_QPI( QSPI_HandleTypeDef *hqspi )
{
  uint8_t reg;
  QSPI_CommandTypeDef     sCommand;
  QSPI_AutoPollingTypeDef sConfig;

  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  sConfig.Interval           = 0x10;
  sConfig.MatchMode          = QSPI_MATCH_MODE_AND;
  sConfig.AutomaticStop      = QSPI_AUTOMATIC_STOP_ENABLE;

  /* Read Enhanced Volatile Configuration register */
  sCommand.Instruction = READ_ENHANCED_VOL_CFG_REG_CMD;
  sCommand.AddressMode = QSPI_ADDRESS_NONE;
  sCommand.DataMode    = QSPI_DATA_1_LINE;
  sCommand.NbData      = 1;

 if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  if (HAL_QSPI_Receive(&QSPIHandle, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* Enable write operations */
  sCommand.Instruction = WRITE_ENABLE_CMD;
  sCommand.DataMode    = QSPI_DATA_NONE;

  if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
   return MEMORY_ERROR;
  }

  /* Reconfigure QSPI to automatic polling mode to wait for write enabling */
  sConfig.Match           = 0x02;
  sConfig.Mask            = 0x02;
  sConfig.StatusBytesSize = 1;

  sCommand.Instruction    = READ_STATUS_REG_CMD;
  sCommand.DataMode       = QSPI_DATA_1_LINE;

  if (HAL_QSPI_AutoPolling(&QSPIHandle, &sCommand, &sConfig, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* Write Enhanced Volatile Configuration register (with Quad I/O SPI protocol) */
  sCommand.Instruction = WRITE_ENHANCED_VOL_CFG_REG_CMD;
  sCommand.NbData      = 1;
  MODIFY_REG(reg, 0xC0, 0x40);

  if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  if (HAL_QSPI_Transmit(&QSPIHandle, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
   return MEMORY_ERROR;
  }

  return MEMORY_OK;
}

/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint32_t QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi)
{
  /* Enable write operations */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  sCommand.Instruction       = WRITE_ENABLE_CMD;
  sCommand.DataMode          = QSPI_DATA_NONE;

  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  /* Configure automatic polling mode to wait for write enabling */
  sConfig.Match           = N25Q128A_SR_WREN;
  sConfig.Mask            = N25Q128A_SR_WREN;

  sCommand.Instruction    = READ_STATUS_REG_CMD;
  sCommand.DataMode       = QSPI_DATA_4_LINES;

  if (HAL_QSPI_AutoPolling(hqspi, &sCommand, &sConfig, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return MEMORY_ERROR;
  }

  return MEMORY_OK;
}
#endif /* (CODE_AREA == USE_QSPI) */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* (CODE_AREA == USE_QSPI) || (BINARY_AREA == USE_SPI_NOR) */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
