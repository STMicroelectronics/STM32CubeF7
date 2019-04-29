/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Decoding/Src/main.c
  * @author  MCD Application Team
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F7xx_HAL_Applications
  * @{
  */

/** @addtogroup LCD_PicturesFromUSBCard
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS USBH_FatFs;  /* File system object for USB card logical drive */
char USB_Path[4]; /* USB card logical drive path */
FIL MyFile;     /* File object */
USBH_HandleTypeDef hUSBHost;
JPG_ApplicationTypeDef Appli_state = APPLICATION_IDLE;
RGB_typedef *RGB_matrix;
uint8_t   _aucLine[2048];
uint16_t LineCounter = 0;
__IO uint32_t DecodeDone = 0;

/* Private function prototypes -----------------------------------------------*/
static void Decode_DisplayProcess(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
static uint8_t Jpeg_CallbackFunction(uint8_t* Row, uint32_t DataLength);
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
       - Configure the Flash ART accelerator on ITCM interface
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  /* Configure the system clock to 216 MHz */
  SystemClock_Config();

  /* Configure LED5 */
  BSP_LED_Init(LED5);

  /* Configure LCD */
  BSP_LCD_Init();
  /* Clear the Background Layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Init Host Library */
  USBH_Init(&hUSBHost, USBH_UserProcess, 0);

  /* Add Supported Class */
  USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);

  /* Start Host Process */
  USBH_Start(&hUSBHost);

  /* Main infinite loop */
  while(1)
  {
    if(DecodeDone == 0)
    {
      /* USB Host Background task */
      USBH_Process(&hUSBHost);

      /* Decode and Display Process */
      Decode_DisplayProcess();
    }
  }
}

/**
  * @brief  Copy decompressed data to display buffer.
  * @param  Row: Output row buffer
  * @param  DataLength: Row width in output buffer
  * @retval None
  */
static uint8_t Jpeg_CallbackFunction(uint8_t* Row, uint32_t DataLength)
{
  RGB_matrix =  (RGB_typedef*)Row;
  uint16_t  RGB565Buffer[IMAGE_WIDTH];
  uint16_t counter = 0;

#ifdef SWAP_RB
  for(counter = 0; counter < IMAGE_WIDTH; counter++)
  {
    RGB565Buffer[counter]  = (uint16_t)
    (
     (((RGB_matrix[counter].R) >> 3)|
      (((RGB_matrix[counter].G) >> 2) << 5)|
      (((RGB_matrix[counter].B) >> 3) << 11))
    );

    BSP_LCD_DrawPixel(counter, LineCounter, RGB565Buffer[counter]);
  }
#else
  for(counter = 0; counter < IMAGE_WIDTH; counter++)
  {
    RGB565Buffer[counter]  = (uint16_t)
    (
     (((RGB_matrix[counter].B) >> 3)|
      (((RGB_matrix[counter].G) >> 2) << 5)|
      (((RGB_matrix[counter].R) >> 3) << 11))
    );

    BSP_LCD_DrawPixel(counter, LineCounter, RGB565Buffer[counter]);
  }
#endif

  LineCounter++;
  return 0;
}

/**
  * @brief  Decode a JPEG image and Display it on LCD
  * @param  None
  * @retval None
  */
static void Decode_DisplayProcess(void)
{
  if(Appli_state == APPLICATION_READY)
  {
    if(f_mount(&USBH_FatFs, (TCHAR const*)USB_Path, 0) == FR_OK)
    {
      if(f_open(&MyFile, "image.jpg", FA_READ) == FR_OK)
      {
        jpeg_decode(&MyFile, IMAGE_WIDTH, _aucLine, Jpeg_CallbackFunction);
      }
      else
      {
        BSP_LCD_Clear(LCD_COLOR_BLACK);
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"    !!! image.jpg not found !!!     ");
        Error_Handler();
      }
    }

    f_close(&MyFile);
    DecodeDone = 1;
  }
}

/**
  * @brief  User Process
  * @param  phost: Host Handle
  * @param  id: Host Library user message ID
  * @retval None
  */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
    break;

  case HOST_USER_DISCONNECTION:
    Appli_state = APPLICATION_DISCONNECT;
    if(FATFS_UnLinkDriver(USB_Path) != 0)
    {
      LCD_ErrLog("ERROR : Cannot unlink FatFS driver! \n");
    }
    if(f_mount(NULL, "", 0) != FR_OK)
    {
      LCD_ErrLog("ERROR : Cannot DeInitialize FatFs! \n");
    }
    break;

  case HOST_USER_CLASS_ACTIVE:
    Appli_state = APPLICATION_READY;
    break;

  case HOST_USER_CONNECTION:
    /* Link the USB Mass Storage disk I/O driver */
    if(FATFS_LinkDriver(&USBH_Driver, USB_Path) != 0)
    {
      LCD_ErrLog("ERROR : Cannot link FatFS driver! \n");
      break;
    }
    if(f_mount(&USBH_FatFs, "", 0) != FR_OK)
    {
      LCD_ErrLog("ERROR : Cannot Initialize FatFs! \n");

      Appli_state = APPLICATION_CONNECTING;
      break;
    }


    break;

  default:
    break;
  }
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED5 on */
  BSP_LED_On(LED5);
  while(1)
  {
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
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  /* Activate the OverDrive to reach the 216 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  Configure the MPU attributes as Write Back for PSRAM mapped on FMC
  *         BANK2
  * @note   The Base Address is 0x64000000 (for Display purposes).
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
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
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
void assert_failed(uint8_t* file, uint32_t line)
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
