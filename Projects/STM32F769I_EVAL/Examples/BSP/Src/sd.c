/**
  ******************************************************************************
  * @file    sd.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the SD Driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
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
#include "main.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            5     /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t aTxBuffer[BUFFER_WORDS_SIZE];
uint32_t aRxBuffer[BUFFER_WORDS_SIZE];
uint32_t aRxBuffer2[BUFFER_WORDS_SIZE];
HAL_SD_CardInfoTypeDef CardInfo;
HAL_SD_CardInfoTypeDef CardInfo2;

extern SD_HandleTypeDef uSdHandle;
extern SD_HandleTypeDef uSdHandle2;

__IO uint32_t SD1WriteStatus = 0, SD1ReadStatus = 0, SD2WriteStatus = 0, SD2ReadStatus = 0;

/* Private function prototypes -----------------------------------------------*/
static void SD_SetHint(void);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SD Demo
  * @param  None
  * @retval None
  */
void SD_demo (void)
{ 
  uint8_t SD1_state = MSD_OK;
  uint8_t SD2_state = MSD_OK;
  __IO uint8_t prev_status = 0; 
  __IO uint8_t prev_status2 = 0; 
  
  SdmmcTest = 1;
  SD_SetHint();
  
  SD1_state = BSP_SD_Init();
  SD2_state = BSP_SD_InitEx(SD_CARD2);
  /* Check if the SD card is plugged in the slot */
  if((BSP_SD_IsDetected() == SD_PRESENT) && (BSP_SD_IsDetectedEx(SD_CARD2) == SD_PRESENT))
  {
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize() - 30, (uint8_t *)" SD1 Connected    ", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize() - 30, (uint8_t *)" SD2 Connected    ", RIGHT_MODE);    
  }
  else if((BSP_SD_IsDetected() == SD_PRESENT) && (BSP_SD_IsDetectedEx(SD_CARD2) != SD_PRESENT))
  {
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize() - 30, (uint8_t *)" SD1 Connected    ", LEFT_MODE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(10, BSP_LCD_GetYSize() - 30, (uint8_t *)"    SD2 Not Connected ", RIGHT_MODE);
  }
  else if((BSP_SD_IsDetected() != SD_PRESENT) && (BSP_SD_IsDetectedEx(SD_CARD2) == SD_PRESENT))
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize() - 30, (uint8_t *)" SD1 Not Connected", LEFT_MODE);    
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize() - 30, (uint8_t *)" SD2 Connected    ", RIGHT_MODE);
  }  
  else 
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize() - 30, (uint8_t *)"SD1 Not Connected", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize() - 30, (uint8_t *)"    SD2 Not Connected ", RIGHT_MODE);    
  }
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  
  if((SD1_state != MSD_OK) && (SD2_state != MSD_OK))
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SD1/SD2 INITIALIZATION : FAIL.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD1/SD2 Test Aborted.", LEFT_MODE);
  }
  else if((SD1_state != MSD_OK) && (SD2_state == MSD_OK))
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SD1 INITIALIZATION : FAIL.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD1 Test Aborted.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SD2 INITIALIZATION : OK.", RIGHT_MODE);
    
    SD2_state = BSP_SD_EraseEx(SD_CARD2, BLOCK_START_ADDR, BLOCK_START_ADDR + NUM_OF_BLOCKS);
    /* Wait until SD card is ready to use for new operation */
    while(BSP_SD_GetCardStateEx(SD_CARD2) != SD_TRANSFER_OK)
    {
    }
    
    if(SD2_state != MSD_OK)
    {
      BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD2 ERASE : FAILED.", RIGHT_MODE);
      BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD2 Test Aborted.", RIGHT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD2 ERASE : OK.", RIGHT_MODE);
      
      /* Fill the buffer to write */
      Fill_Buffer(aTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);
      SD2_state = BSP_SD_WriteBlocks_DMAEx(SD_CARD2, (uint32_t *)aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
      /* Wait for the write process is completed */
      while((SD2WriteStatus == 0))
      {
      }
      SD2WriteStatus = 0;
      
      /* Wait until SD cards are ready to use for new operation */
      while((BSP_SD_GetCardStateEx(SD_CARD2) != SD_TRANSFER_OK))
      {
      }        
      
      if(SD2_state != MSD_OK)
      {
        BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD2 WRITE : FAILED.", RIGHT_MODE);
        BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD2 Test Aborted.", RIGHT_MODE);
      }
      else
      {
        BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD2 WRITE : OK.", RIGHT_MODE);
        SD2_state = BSP_SD_ReadBlocks_DMAEx(SD_CARD2, (uint32_t *)aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
        
        /* Wait for the read process is completed */
        while(SD2ReadStatus == 0)
        {
        }
        SD2ReadStatus = 0;
        
        /* Wait until SD card is ready to use for new operation */
        while(BSP_SD_GetCardStateEx(SD_CARD2) != SD_TRANSFER_OK)
        {
        } 
        if(SD2_state != MSD_OK)
        {
          BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD2 READ : FAILED.", RIGHT_MODE);
          BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD2 Test Aborted.", RIGHT_MODE);            
        }
        else
        {
          BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD2 READ : OK.", RIGHT_MODE);
          if(Buffercmp(aTxBuffer, aRxBuffer, BUFFER_WORDS_SIZE) > 0)
          {
            BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD2 COMPARE : FAILED.", RIGHT_MODE);
            BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SD2 Test Aborted.", RIGHT_MODE);
          }
          else
          {
            BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD2 COMPARE : OK.", RIGHT_MODE);
          }  
        }
      }
    }
  }
  else if((SD1_state == MSD_OK) && (SD2_state != MSD_OK))
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SD2 INITIALIZATION : FAIL.", RIGHT_MODE);
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD2 Test Aborted.", RIGHT_MODE);
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SD1 INITIALIZATION : OK.", LEFT_MODE);
    
    SD1_state = BSP_SD_Erase(BLOCK_START_ADDR, BLOCK_START_ADDR + NUM_OF_BLOCKS);
    /* Wait until SD card is ready to use for new operation */
    while(BSP_SD_GetCardState() != SD_TRANSFER_OK)
    {
    }
    
    if(SD1_state != MSD_OK)
    {
      BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD1 ERASE : FAILED.", LEFT_MODE);
      BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD1 Test Aborted.", LEFT_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD1 ERASE : OK.", LEFT_MODE);
      
      /* Fill the buffer to write */
      Fill_Buffer(aTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);
      SD1_state = BSP_SD_WriteBlocks_DMA((uint32_t *)aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
      /* Wait for the write process is completed */
      while((SD1WriteStatus == 0))
      {
      }
      SD1WriteStatus = 0;
      
      /* Wait until SD cards are ready to use for new operation */
      while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
      {
      }        
      
      if(SD1_state != MSD_OK)
      {
        BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD1 WRITE : FAILED.", LEFT_MODE);
        BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD1 Test Aborted.", LEFT_MODE);
      }
      else
      {
        BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD1 WRITE : OK.", LEFT_MODE);
        SD1_state = BSP_SD_ReadBlocks_DMA((uint32_t *)aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
        
        /* Wait for the read process is completed */
        while(SD1ReadStatus == 0)
        {
        }
        SD1ReadStatus = 0;
        
        /* Wait until SD card is ready to use for new operation */
        while(BSP_SD_GetCardState() != SD_TRANSFER_OK)
        {
        } 
        if(SD1_state != MSD_OK)
        {
          BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD1 READ : FAILED.", LEFT_MODE);
          BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD1 Test Aborted.", LEFT_MODE);            
        }
        else
        {
          BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD1 READ : OK.", LEFT_MODE);
          if(Buffercmp(aTxBuffer, aRxBuffer, BUFFER_WORDS_SIZE) > 0)
          {
            BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD1 COMPARE : FAILED.", LEFT_MODE);
            BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SD1 Test Aborted.", LEFT_MODE);
          }
          else
          {
            BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD1 COMPARE : OK.", LEFT_MODE);
          }  
        }
      }
    }
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SD1/SD2 INITIALIZATION : OK.", LEFT_MODE); 
    
    BSP_SD_GetCardInfo(&CardInfo);
    BSP_SD_GetCardInfoEx(SD_CARD2, &CardInfo2);
    if((SD1_state != MSD_OK) && (SD2_state != MSD_OK))
    {
      BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD1/SD2 GET CARD INFO : FAIL.", LEFT_MODE);
      BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD1/SD2 Test Aborted.", LEFT_MODE);
    }
    else if((SD1_state != MSD_OK) && (SD2_state == MSD_OK))
    {
      BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD1 GET CARD INFO : FAIL.", LEFT_MODE);
      BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD1 Test Aborted.", LEFT_MODE);
    }
    else if((SD1_state == MSD_OK) && (SD2_state != MSD_OK))
    {
      BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD2 GET CARD INFO : FAIL.", LEFT_MODE);
      BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD2 Test Aborted.", LEFT_MODE);
    }  
    else
    {
      BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SD1/SD2 GET CARD INFO : OK.", LEFT_MODE);
      
      SD1_state = BSP_SD_Erase(BLOCK_START_ADDR, BLOCK_START_ADDR + NUM_OF_BLOCKS);
      /* Wait until SD cards are ready to use for new operation */
      while(BSP_SD_GetCardState() != SD_TRANSFER_OK)
      {
      }
      
      SD2_state = BSP_SD_EraseEx(SD_CARD2, BLOCK_START_ADDR, BLOCK_START_ADDR + NUM_OF_BLOCKS);
      /* Wait until SD cards are ready to use for new operation */
      while(BSP_SD_GetCardStateEx(SD_CARD2) != SD_TRANSFER_OK)
      {
      }
      
      if((SD1_state != MSD_OK) && (SD2_state != MSD_OK))
      {
        BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD1/SD2 ERASE : FAILED.", LEFT_MODE);
        BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD1/SD2 Test Aborted.", LEFT_MODE);
      }
      else if((SD1_state != MSD_OK) && (SD2_state == MSD_OK))
      {
        BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD1 ERASE : FAILED.", LEFT_MODE);
        BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD1 Test Aborted.", LEFT_MODE);
      }  
      else if((SD1_state == MSD_OK) && (SD2_state != MSD_OK))
      {
        BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD2 ERASE : FAILED.", LEFT_MODE);
        BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD2 Test Aborted.", LEFT_MODE);
      }       
      else
      {
        BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SD1/SD2 ERASE : OK.", LEFT_MODE);
        
        /* Fill the buffer to write */
        Fill_Buffer(aTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);
        
        SD2_state = BSP_SD_WriteBlocks_DMAEx(SD_CARD2, (uint32_t *)aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
        
        /* Wait for the write process is completed */
        while((SD2WriteStatus == 0))
        {
        }
        SD2WriteStatus = 0;
        
        /* Wait until SD cards are ready to use for new operation */
        while(BSP_SD_GetCardStateEx(SD_CARD2) != SD_TRANSFER_OK)
        {
        }
        
        SD1_state = BSP_SD_WriteBlocks_DMA((uint32_t *)aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
        /* Wait for the write process is completed */
        while((SD1WriteStatus == 0))
        {
        }
        SD1WriteStatus = 0;
        
        /* Wait until SD cards are ready to use for new operation */
        while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
        {
        }        
        
        if((SD1_state != MSD_OK) && (SD2_state != MSD_OK))
        {
          BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD1/SD2 WRITE : FAILED.", LEFT_MODE);
          BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD1/SD2 Test Aborted.", LEFT_MODE);
        }
        else if((SD1_state != MSD_OK) && (SD2_state == MSD_OK))
        {
          BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD1 WRITE : FAILED.", LEFT_MODE);
          BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD1 Test Aborted.", LEFT_MODE);
        }
        else if((SD1_state == MSD_OK) && (SD2_state != MSD_OK))
        {
          BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD2 WRITE : FAILED.", LEFT_MODE);
          BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD2 Test Aborted.", LEFT_MODE);
        }        
        else
        {
          BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SD1/SD2 WRITE : OK.", LEFT_MODE);
          SD1_state = BSP_SD_ReadBlocks_DMA((uint32_t *)aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
          /* Wait for the read process is completed */
          while(SD1ReadStatus == 0)
          {
          }
          SD1ReadStatus = 0;
          
          /* Wait until SD cards are ready to use for new operation */
          while(BSP_SD_GetCardState() != SD_TRANSFER_OK)
          {          
          }
          SD2_state = BSP_SD_ReadBlocks_DMAEx(SD_CARD2, (uint32_t *)aRxBuffer2, BLOCK_START_ADDR, NUM_OF_BLOCKS);
          /* Wait for the read process is completed */
          while(SD2ReadStatus == 0)
          {
          }
          SD2ReadStatus = 0; 
          
          /* Wait until SD cards are ready to use for new operation */
          while(BSP_SD_GetCardStateEx(SD_CARD2) != SD_TRANSFER_OK)
          {
          }
          
          if((SD1_state != MSD_OK)&&(SD2_state != MSD_OK))
          {
            BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD1/SD2 READ : FAILED.", LEFT_MODE);
            BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SD1/SD2 Test Aborted.", LEFT_MODE);
          }
          else if((SD1_state != MSD_OK)&&(SD2_state == MSD_OK))
          {
            BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD1 READ : FAILED.", LEFT_MODE);
            BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SD1 Test Aborted.", LEFT_MODE);
          }
          if((SD1_state == MSD_OK)&&(SD2_state != MSD_OK))
          {
            BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD2 READ : FAILED.", LEFT_MODE);
            BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SD2 Test Aborted.", LEFT_MODE);
          }          
          else
          {
            BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SD1/SD2 READ : OK.", LEFT_MODE);
            if(Buffercmp(aTxBuffer, aRxBuffer, BUFFER_WORDS_SIZE) > 0)
            {
              BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SD1 COMPARE : FAILED.", LEFT_MODE);
              BSP_LCD_DisplayStringAt(20, 190, (uint8_t *)"SD1 Test Aborted.", LEFT_MODE);
            }
            else
            {
              BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SD1 TEST : OK.", LEFT_MODE);
            }
            if(Buffercmp(aTxBuffer, aRxBuffer2, BUFFER_WORDS_SIZE) > 0)
            {
              BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SD2 COMPARE : FAILED.", RIGHT_MODE);
              BSP_LCD_DisplayStringAt(20, 190, (uint8_t *)"SD2 Test Aborted.", RIGHT_MODE);
            }
            else
            {
              BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"SD2 TEST : OK.", RIGHT_MODE);
            }            
          }
        }
      }
    }
  }
  
  while (1)
  {
    /* Check if the SD card is plugged in the slot */
    if(BSP_SD_IsDetected() != SD_PRESENT)
    {
      if(prev_status == 0)
      {
        prev_status = 1; 
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize() - 30, (uint8_t *)"SD1 Not Connected", LEFT_MODE);
      }
    }
    else if (prev_status == 1)
    {
      BSP_SD_Init();
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize() - 30, (uint8_t *)" SD1 Connected    ", LEFT_MODE);
      prev_status = 0;
    }
    
    /* Check if the SD card is plugged in the slot */
    if(BSP_SD_IsDetectedEx(SD_CARD2) != SD_PRESENT)
    {
      if(prev_status2 == 0)
      {
        prev_status2 = 1; 
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_DisplayStringAt(10, BSP_LCD_GetYSize() - 30, (uint8_t *)"    SD2 Not Connected ", RIGHT_MODE);
      }
    }
    else if (prev_status2 == 1)
    {
      BSP_SD_InitEx(SD_CARD2);
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_DisplayStringAt(10, BSP_LCD_GetYSize() - 30, (uint8_t *)" SD2 Connected    ", RIGHT_MODE);
      prev_status2 = 0;
    }
    
    if(CheckForUserInput() > 0)
    {
      SdmmcTest = 0;
      BSP_SD_DeInitEx(SD_CARD1);
      BSP_SD_DeInitEx(SD_CARD2);
      return;
    }
  }
}

/**
  * @brief  Display SD Demo Hint
  * @param  None
  * @retval None
  */
static void SD_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 87);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"SD1/SD2", CENTER_MODE);

  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"and read data on the microSD and also", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"how to detect the presence of the card", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 75, (uint8_t *)"Make sure JP7 is in position 1-2", CENTER_MODE);

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 92, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 93, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
 }

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}
uint32_t i = 0;
/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return 0;
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void BSP_SD_WriteCpltCallback(uint32_t SdCard)
{
  if(SdCard == SD_CARD1)
  {
  SD1WriteStatus = 1;
  }
  else
  {
   SD2WriteStatus = 1; 
  }
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void BSP_SD_ReadCpltCallback(uint32_t SdCard)
{
  if(SdCard == SD_CARD1)
  {
  SD1ReadStatus = 1;
  }
  else
  {
   SD2ReadStatus = 1; 
  }  
} 

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
