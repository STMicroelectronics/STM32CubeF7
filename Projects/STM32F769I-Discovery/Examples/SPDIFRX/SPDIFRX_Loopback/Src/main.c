/**
  ******************************************************************************
  * @file    SPDIFRX/SPDIFRX_Loopback/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use SPDIFRX HAL APIs to establish a
  *          simple buffer transmission.
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
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup SPDIFRX_Loopback
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SAI_HandleTypeDef            hsai;
DMA_HandleTypeDef            hSaiDma;
SPDIFRX_HandleTypeDef        SpdifrxHandle;

/* 16 bit Data Buffer for Transmission */ 
uint16_t Tab_Tx_16b[64] = {
0x5152, 0x5354, 0x5556, 0x5758, 0x595A, 0x5B5C, 0x5D5E, 0x5F60,
0x6162, 0x6364, 0x6566, 0x6768, 0x696A, 0x6B6C, 0x6D6E, 0x6F70,
0x7172, 0x7374, 0x7576, 0x7778, 0x797A, 0x7B7C, 0x7D7E, 0x7F80,
0x8182, 0x8384, 0x8586, 0x8788, 0x898A, 0x8B8C, 0x8D8E, 0x8F90,
0x5152, 0x5354, 0x5556, 0x5758, 0x595A, 0x5B5C, 0x5D5E, 0x5F60,
0x6162, 0x6364, 0x6566, 0x6768, 0x696A, 0x6B6C, 0x6D6E, 0x6F70,
0x7172, 0x7374, 0x7576, 0x7778, 0x797A, 0x7B7C, 0x7D7E, 0x7F80,
0x8182, 0x8384, 0x8586, 0x8788, 0x898A, 0x8B8C, 0x8D8E, 0x8F90};
uint32_t received_data_flow[64] ;
uint32_t data_cpt_occured = 0;
uint32_t control_cpt_occured = 0;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static int SPDIFBufferCmp(uint32_t* pBuffer1, uint16_t* pBuffer2, uint8_t size);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  
  /* Configure the MPU attributes */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32F7xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock at 216 MHz */
  SystemClock_Config();

  /* Configure LED2 (Green)*/
  BSP_LED_Init(LED2);
  /* Configure LED1 (Red)*/
  BSP_LED_Init(LED1);
  
  /* Initialize reception buffer */
  for(uint32_t j = 0; j<64; j++)
  {
    received_data_flow[j] = 0; 
  }
  
  /* Configure SAI in SPDIF-Tx Mode */
  hsai.Instance = SAI2_Block_A;
  hsai.Init.AudioMode      = SAI_MODEMASTER_TX;
  hsai.Init.Synchro        = SAI_ASYNCHRONOUS;
  hsai.Init.SynchroExt     = SAI_SYNCEXT_DISABLE;
  hsai.Init.OutputDrive    = SAI_OUTPUTDRIVE_DISABLE;
  hsai.Init.NoDivider      = SAI_MASTERDIVIDER_ENABLE;
  hsai.Init.FIFOThreshold  = SAI_FIFOTHRESHOLD_EMPTY;
  hsai.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_96K;
  hsai.Init.Mckdiv         = 0;
  hsai.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai.Init.TriState       = SAI_OUTPUT_NOTRELEASED;
  hsai.Init.Protocol       = SAI_SPDIF_PROTOCOL;
  hsai.Init.DataSize       = SAI_DATASIZE_24;
  hsai.Init.FirstBit       = SAI_FIRSTBIT_MSB;
  hsai.Init.ClockStrobing  = SAI_CLOCKSTROBING_FALLINGEDGE;
  
  hsai.FrameInit.FrameLength       = 64;
  hsai.FrameInit.ActiveFrameLength = 32;
  hsai.FrameInit.FSDefinition      = SAI_FS_STARTFRAME;
  hsai.FrameInit.FSPolarity        = SAI_FS_ACTIVE_LOW;
  hsai.FrameInit.FSOffset          = SAI_FS_FIRSTBIT;
  
  hsai.SlotInit.FirstBitOffset = 0;
  hsai.SlotInit.SlotSize       = SAI_SLOTSIZE_DATASIZE;
  hsai.SlotInit.SlotNumber     = 4;
  hsai.SlotInit.SlotActive     = SAI_SLOTACTIVE_ALL;
  
  if(HAL_SAI_Init(&hsai)!= HAL_OK )
  {
    Error_Handler();
  }
  
  /* Configure SPDIFRX Peripheral */
  SpdifrxHandle.Instance = SPDIFRX;
  HAL_SPDIFRX_DeInit(&SpdifrxHandle); 
  
  SpdifrxHandle.Init.InputSelection = SPDIFRX_INPUT_IN1;
  SpdifrxHandle.Init.Retries = SPDIFRX_MAXRETRIES_15;
  SpdifrxHandle.Init.WaitForActivity = SPDIFRX_WAITFORACTIVITY_ON;
  SpdifrxHandle.Init.ChannelSelection = SPDIFRX_CHANNEL_A;
  SpdifrxHandle.Init.DataFormat = SPDIFRX_DATAFORMAT_MSB;
  SpdifrxHandle.Init.StereoMode = SPDIFRX_STEREOMODE_ENABLE;
  SpdifrxHandle.Init.PreambleTypeMask = SPDIFRX_PREAMBLETYPEMASK_ON;
  SpdifrxHandle.Init.ChannelStatusMask = SPDIFRX_CHANNELSTATUS_ON;  
  if(HAL_SPDIFRX_Init(&SpdifrxHandle) != HAL_OK)
  {
    /* Initialization error */
    Error_Handler();
  }
  
  /* Start transmission (DMA mode) */
  HAL_SAI_Transmit_DMA(&hsai, (uint8_t *)Tab_Tx_16b, 64);
  
  /* Reception in (Polling mode) */
  HAL_SPDIFRX_ReceiveDataFlow(&SpdifrxHandle, (uint32_t *)received_data_flow, 64, 0xFFF);
  
  if(SpdifrxHandle.ErrorCode != HAL_SPDIFRX_ERROR_NONE)
  {
    Error_Handler();
  }
  
  /* Compare the received data with the expected one */    
  if (SPDIFBufferCmp(received_data_flow, Tab_Tx_16b, 64) != 0)
  {
    /* Transmission error */
    Error_Handler();
  }
  else
  {
    /* Transmission succeeded */
    BSP_LED_On(LED2);
  }
  
  while(1);
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
  *            PLL_R                          = 7
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
  HAL_StatusTypeDef  ret = HAL_OK;
  
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
  RCC_OscInitStruct.PLL.PLLR = 7;
  
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Activate the OverDrive to reach the 216 MHz Frequency */  
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
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
    while(1) { ; }
  }  
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* LED1 On in error case */
  BSP_LED_On(LED1);
  while (1)
  {
  }
}

/**
* @brief  Compares two 16-bit buffers and returns the comparison result
* @param  pBuffer1: pointer to the first buffer.
* @param  pBuffer2: pointer to the second buffer.
* @retval
*    - 0: Comparison is OK (the two Buffers are identical)
*    - Value different from 0: Comparison is NOK (Buffers are different)  
*/
static int SPDIFBufferCmp(uint32_t* pBuffer1, uint16_t* pBuffer2, uint8_t size)
{
  while (size--)
  {
    if (((pBuffer1[size]&0x00ffff00) >>8) != (pBuffer2[size]))
    {
      return 1;
    }
    
    pBuffer1++;
    pBuffer2++;
  }
  
  return 0;
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


/**
  * @brief  Configure the MPU attributes
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU as Strongly ordered for not defined regions */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
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

