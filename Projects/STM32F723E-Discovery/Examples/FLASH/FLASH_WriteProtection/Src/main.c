/**
  ******************************************************************************
  * @file    FLASH/FLASH_WriteProtection/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to erase and program the
  *          STM32F7xx FLASH.
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

/** @addtogroup FLASH_WriteProtection
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_WRP_SECTORS   (OB_WRP_SECTOR_6 | OB_WRP_SECTOR_7) /* sectors 6 and 7 */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FLASH_OBProgramInitTypeDef OBInit;
__IO uint32_t SectorsWRPStatus = 0xFFF;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void MPU_ConfigPSRAM(void);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the MPU attributes for PSRAM external memory */
  MPU_ConfigPSRAM();
  
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  
  /* STM32F7xx HAL library initialization:
       - Configure the Flash ART accelerator
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 216 MHz */
  SystemClock_Config();

  /* Configure User/WakeUp push-button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /*##-1- Initialize the LCD #################################################*/
  /* Initialize the LCD */
  BSP_LCD_Init();

  BSP_LCD_SetFont(&Font20);

  /*##-2- Display messages on LCD ############################################*/
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

  /* Display test name on LCD */
  BSP_LCD_DisplayStringAt(0, 40, (uint8_t *)"Flash Write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"Protection test", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 80, (uint8_t *)"Press User", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)"Button", CENTER_MODE);

  while(1)
  {
    /* Wait for User/WakeUp push-button to be pushed */
    while (BSP_PB_GetState(BUTTON_USER) == RESET)
    {
    }

    /* Wait for User/WakeUp push-button to be released */
    while (BSP_PB_GetState(BUTTON_USER) != RESET)
    {
    }

    /* Get FLASH_WRP_SECTORS write protection status */
    HAL_FLASHEx_OBGetConfig(&OBInit);
    SectorsWRPStatus = OBInit.WRPSector & FLASH_WRP_SECTORS;

    if (SectorsWRPStatus == 0)
    {
      /* If FLASH_WRP_SECTORS are write protected, disable the write protection */

      /* Allow Access to option bytes sector */
      HAL_FLASH_OB_Unlock();

      /* Allow Access to Flash control registers and user Flash */
      HAL_FLASH_Unlock();

      /* Disable FLASH_WRP_SECTORS write protection */
      OBInit.OptionType = OPTIONBYTE_WRP;
      OBInit.WRPState   = OB_WRPSTATE_DISABLE;
      OBInit.WRPSector  = FLASH_WRP_SECTORS;
      HAL_FLASHEx_OBProgram(&OBInit);

      /* Start the Option Bytes programming process */
      if (HAL_FLASH_OB_Launch() != HAL_OK)
      {
        /* User can add here some code to deal with this error */
        while (1)
        {
        }
      }

      /* Prevent Access to option bytes sector */
      HAL_FLASH_OB_Lock();

      /* Disable the Flash option control register access (recommended to protect
      the option Bytes against possible unwanted operations) */
      HAL_FLASH_Lock();

      /* Get FLASH_WRP_SECTORS write protection status */
      HAL_FLASHEx_OBGetConfig(&OBInit);
      SectorsWRPStatus = OBInit.WRPSector & FLASH_WRP_SECTORS;

      /* Check if FLASH_WRP_SECTORS write protection is disabled */
      if (SectorsWRPStatus == FLASH_WRP_SECTORS)
      {
        /* Set the LCD Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);

        BSP_LCD_DisplayStringAt(0, 140, (uint8_t *)"Write", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"protection is", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 180, (uint8_t *)"disabled", CENTER_MODE);
      }
      else
      {
        /* Set the LCD Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_RED);

        BSP_LCD_DisplayStringAt(0, 140, (uint8_t *)"Write", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"protection is", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 180, (uint8_t *)"not disabled", CENTER_MODE);
      }
    }
    else
    {
      /* If FLASH_WRP_SECTORS are not write protected, enable the write protection */

      /* Allow Access to option bytes sector */
      HAL_FLASH_OB_Unlock();

      /* Allow Access to Flash control registers and user Flash */
      HAL_FLASH_Unlock();

      /* Enable FLASH_WRP_SECTORS write protection */
      OBInit.OptionType = OPTIONBYTE_WRP;
      OBInit.WRPState   = OB_WRPSTATE_ENABLE;
      OBInit.WRPSector  = FLASH_WRP_SECTORS;
      HAL_FLASHEx_OBProgram(&OBInit);

      /* Start the Option Bytes programming process */
      if (HAL_FLASH_OB_Launch() != HAL_OK)
      {
        /* User can add here some code to deal with this error */
        while (1)
        {
        }
      }

      /* Prevent Access to option bytes sector */
      HAL_FLASH_OB_Lock();

      /* Disable the Flash option control register access (recommended to protect
      the option Bytes against possible unwanted operations) */
      HAL_FLASH_Lock();

      /* Get FLASH_WRP_SECTORS write protection status */
      HAL_FLASHEx_OBGetConfig(&OBInit);
      SectorsWRPStatus = OBInit.WRPSector & FLASH_WRP_SECTORS;

      /* Check if FLASH_WRP_SECTORS are write protected */
      if (SectorsWRPStatus == 0)
      {
        /* Set the LCD Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);

        BSP_LCD_DisplayStringAt(0, 140, (uint8_t *)"Write", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"protection is", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 180, (uint8_t *)" enabled ", CENTER_MODE);
      }
      else
      {
        /* Set the LCD Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_RED);

        BSP_LCD_DisplayStringAt(0, 140, (uint8_t *)"Write", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"protection is", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 180, (uint8_t *)"not enabled", CENTER_MODE);
      }
    }
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;  
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1) {};
  }
  
  /* Activate the OverDrive to reach the 216 Mhz Frequency */
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    while(1) {};
  }
  
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    while(1) {};
  }
}

/**
  * @brief  Configure the MPU attributes as Write Back for PSRAM mapped on FMC
  *         BANK2.
  * @note   The Base Address is 0x64000000.
  *         The Region Size is 512KB, it is related to PSRAM memory size.
  * @param  None
  * @retval None
  */
static void MPU_ConfigPSRAM(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes for PSRAM with recomended configurations:
     Normal memory, Shareable, write-back (Display purposes) */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x64000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
