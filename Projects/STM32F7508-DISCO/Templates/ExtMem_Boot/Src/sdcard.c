/**
  ******************************************************************************
  * @file    sdcard.c
  * @author  MCD Application Team
  * @brief   This file includes the sdcard supported STM32F7508-DISCO evaluation
             board.
  @verbatim
  PartNumber supported by the file:
  -----------------------
   - ToBeUpdated :  SDCard  mounted on STM32F7508 Discovery board.

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
#include "ff_gen_drv.h"
#include "../Components/mfxstm32l152/mfxstm32l152.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* use the default SD timout as defined in the platform BSP driver*/
#if defined(SDMMC_DATATIMEOUT)
#define SD_TIMEOUT SDMMC_DATATIMEOUT
#elif defined(SD_DATATIMEOUT)
#define SD_TIMEOUT SD_DATATIMEOUT
#else
#define SD_TIMEOUT 30 * 1000
#endif

#define SD_DEFAULT_BLOCK_SIZE 512

/* Defined related to stm32756g eval board */
#define IO_I2C_ADDRESS          ((uint16_t)0x84)  /*mfx MFX_I2C_ADDR 0*/
#define IO_PIN_ALL              ((uint32_t)0xFFFFFF)
#define SD_DETECT_PIN           ((uint32_t)0x8000)

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hI2C;
#if (BINARY_AREA == USE_SDCARD)

/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;
SD_HandleTypeDef uSdHandle;


/* Private function prototypes -----------------------------------------------*/
static DSTATUS SD_CheckStatus(BYTE lun);
DSTATUS SD_initialize (BYTE);
DSTATUS SD_status (BYTE);
DRESULT SD_read (BYTE, BYTE*, DWORD, UINT);

const Diskio_drvTypeDef  SD_Driver =
{
  SD_initialize,
  SD_status,
  SD_read,
#if  _USE_WRITE == 1
  NULL,
#endif /* _USE_WRITE == 1 */

#if  _USE_IOCTL == 1
  NULL,
#endif /* _USE_IOCTL == 1 */
};


/**
  * @brief  Initialize the SDCARD through the FATFS, copy binary to its execution
  *         area and finaly free no more needed ressources.
  * @param  None
  * @retval None
  */
uint32_t SDCARD_Startup(void)
{
  uint32_t bytesread;   /* File read counts */
  FATFS SDFatFs;        /* File system object for SD card logical drive */
  FIL BinaryFile;       /* File object */
  char SDPath[4];       /* SD card logical drive path */

  if(FATFS_LinkDriver(&SD_Driver, SDPath) != 0)
  {
    return MEMORY_ERROR;
  }

  if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
  {
    return MEMORY_ERROR;
  }

  if(f_open(&BinaryFile, BINARY_FILENAME, FA_OPEN_EXISTING | FA_READ) != FR_OK)
  {
    return MEMORY_ERROR;
  }

  if(f_read(&BinaryFile, (void*)APPLICATION_ADDRESS, f_size(&BinaryFile), (UINT*)&bytesread) != FR_OK)
  {
    return MEMORY_ERROR;
  }

  if(f_size(&BinaryFile) != bytesread)
  {
    return MEMORY_ERROR;
  }

  f_close(&BinaryFile);

  FATFS_UnLinkDriver(SDPath);

  /* De-Initialize all no more needed ressources */
  HAL_SD_DeInit(&uSdHandle);
  SDCARD_MspDeInit();

  return MEMORY_OK;
}

/* Private functions ---------------------------------------------------------*/
static DSTATUS SD_CheckStatus(BYTE lun)
{
  Stat = STA_NOINIT;

  if(HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER)
  {
    Stat &= ~STA_NOINIT;
  }

  return Stat;
}

/**
  * @brief  Initializes a Drive
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS SD_initialize(BYTE lun)
{
  Stat = STA_NOINIT;

  /* uSD device interface configuration */
  uSdHandle.Instance = SDMMC1;

  uSdHandle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
  uSdHandle.Init.ClockBypass         = SDMMC_CLOCK_BYPASS_DISABLE;
  uSdHandle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  uSdHandle.Init.BusWide             = SDMMC_BUS_WIDE_1B;
  uSdHandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  uSdHandle.Init.ClockDiv            = SDMMC_TRANSFER_CLK_DIV;

  /* Initialize all required GPIO PINs */
  SDCARD_MspInit();

  /* HAL SD initialization */
  if(HAL_SD_Init(&uSdHandle) != HAL_OK)
  {
    return RES_ERROR;
  }

  /* Enable wide operation */
  if(HAL_SD_ConfigWideBusOperation(&uSdHandle, SDMMC_BUS_WIDE_4B) != HAL_OK)
  {
    return RES_ERROR;
  }
  else
  {
    return RES_OK;
  }
}

/**
  * @brief  Gets Disk Status
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS SD_status(BYTE lun)
{
  return SD_CheckStatus(lun);
}

/**
  * @brief  Reads Sector(s)
  * @param  lun : not used
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR;

  if(HAL_SD_ReadBlocks(&uSdHandle,
                       (uint8_t *)buff,
                       sector,
                       count,
                       SD_TIMEOUT) == HAL_OK)
  {
    /* wait until the read operation is finished */
    while(HAL_SD_GetCardState(&uSdHandle) != HAL_SD_CARD_TRANSFER)
    {
    }
    res = RES_OK;
  }

  return res;
}

#endif /* (BINARY_AREA == USE_SDCARD) */

/********************************* LINK MFX ***********************************/
///**
//  * @brief  Initializes MFX low level.
//  * @retval None
//  */
//void MFX_IO_Init(void)
//{
//  hI2C.Instance              = I2C1;
//  hI2C.Init.Timing           = ((uint32_t)0x40912732);
//  hI2C.Init.OwnAddress1      = 0;
//  hI2C.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
//  hI2C.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
//  hI2C.Init.OwnAddress2      = 0;
//  hI2C.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
//  hI2C.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
//
//  /* Init the I2C */
//  HAL_I2C_Init(&hI2C);
//}

///**
//  * @brief  DeInitializes MFX low level.
//  * @retval None
//  */
//void MFX_IO_DeInit(void)
//{
//}
//
///**
//  * @brief  Configures MFX low level interrupt.
//  * @retval None
//  */
//void MFX_IO_ITConfig(void)
//{
//}

///**
//  * @brief  MFX writes single data.
//  * @param  Addr: I2C address
//  * @param  Reg: Register address
//  * @param  Value: Data to be written
//  * @retval None
//  */
//void MFX_IO_Write(uint16_t Addr, uint8_t Reg, uint8_t Value)
//{
//  HAL_StatusTypeDef status = HAL_OK;
//
//  status = HAL_I2C_Mem_Write(&hI2C, (uint8_t)Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 100);
//
//  /* Check the communication status */
//  if(status != HAL_OK)
//  {
//    return;
//  }
//}

///**
//  * @brief  MFX reads single data.
//  * @param  Addr: I2C address
//  * @param  Reg: Register address
//  * @retval Read data
//  */
//uint8_t MFX_IO_Read(uint16_t Addr, uint8_t Reg)
//{
//  HAL_StatusTypeDef status = HAL_OK;
//  uint8_t Value = 0;
//
//  status = HAL_I2C_Mem_Read(&hI2C, (uint8_t)Addr, Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 1000);
//
//  /* Check the communication status */
//  if(status != HAL_OK)
//  {
//    return 0;
//  }
//  return Value;
//}

///**
//  * @brief  MFX reads multiple data.
//  * @param  Addr: I2C address
//  * @param  Reg: Register address
//  * @param  Buffer: Pointer to data buffer
//  * @param  Length: Length of the data
//  * @retval Number of read data
//  */
//uint16_t MFX_IO_ReadMultiple(uint16_t Addr, uint8_t Reg, uint8_t *Buffer, uint16_t Length)
//{
//  return 0;
//}
//
///**
//  * @brief  MFX delay
//  * @param  Delay: Delay in ms
//  * @retval None
//  */
//void MFX_IO_Delay(uint32_t Delay)
//{
//  HAL_Delay(Delay);
//}
//
///**
//  * @brief  Used by Lx family but requested for MFX component compatibility.
//  * @retval None
//  */
//void MFX_IO_Wakeup(void)
//{
//}
//
///**
//  * @brief  Used by Lx family but requested for MXF component compatibility.
//  * @retval None
//  */
//void MFX_IO_EnableWakeupPin(void)
//{
//}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

