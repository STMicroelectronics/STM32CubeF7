/**
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemToMemWithPFC/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example provides a description of how to configure DMA2D peripheral in 
  *          Memory to Memory with pixel format conversion transfer mode and display the 
  *          result on LCD.
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

#include "ARGB8888_300x120.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup DMA2D_MemToMemWithPFC
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMA2D_HandleTypeDef Dma2dHandle;

/* DMA2D output buffer which is input layer 2 for LTDC display on LCD */
/* Format is ARGB4444 : 16 bpp and size 300x120 */
uint32_t aBufferResult[(LAYER_SIZE_X * LAYER_SIZE_Y * FINAL_LAYER_BYTE_PER_PIXEL) / 4];


/* Private function prototypes -----------------------------------------------*/
static void LCD_Config(void);

static void DMA2D_Config(void);
static void TransferError(DMA2D_HandleTypeDef* Dma2dHandle);
static void TransferComplete(DMA2D_HandleTypeDef* Dma2dHandle);
static void SystemClock_Config(void);
static void OnError_Handler(uint32_t condition);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{       
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  HAL_StatusTypeDef hal_status = HAL_OK;

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
  
  /* Configure the system clock */
  SystemClock_Config();


  /*## LTDC Clock Configuration ###########################################*/  
  /* LCD clock configuration */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/5 = 38.4 Mhz */
  /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_4 = 38.4/4 = 9.6Mhz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);  
    
  /* Configure LED1 */
  BSP_LED_Init(LED1);
  
  /*##-1- LCD Configuration ##################################################*/
  LCD_Config();    
  
  /*##-2- DMA2D configuration ################################################*/
  DMA2D_Config(); 
  
  /*##-3- Start DMA2D transfer ###############################################*/  
  hal_status = HAL_DMA2D_Start_IT(&Dma2dHandle, 
                        (uint32_t)&ARGB8888_300x120, /* Input image 300x120 of format ARGB8888 (32 bpp) */
                        (uint32_t)&aBufferResult,    /* Output image of same size 300x120 after conversion by PFC in ARGB4444 (16 bpp) */
                        LAYER_SIZE_X,
                        LAYER_SIZE_Y);
  OnError_Handler(hal_status != HAL_OK);

  
  while (1) 
  { 
    ; 
  }
}


/**
  * @brief DMA2D configuration.
  * @note  This function Configure tha DMA2D peripheral :
  *        1) Configure the transfer mode : memory to memory with pixel format conversion (PFC)
  *        2) Configure the output color mode to ARGB4444
  *        3) Configure the output memory address at SRAM memory  
  *        4) Configure the data size : 320x120 (pixels)  
  *        5) Configure the input color mode as ARGB8888
  *        6) Configure the input memory address at FLASH memory 
  * @retval
  *  None
  */

static void DMA2D_Config(void)
{    
  HAL_StatusTypeDef hal_status = HAL_OK;
  
  /* Configure the DMA2D Mode, Color Mode and output offset */
  Dma2dHandle.Init.Mode         = DMA2D_M2M_PFC; /* DMA2D mode Memory to Memory with Pixel Format Conversion */
  Dma2dHandle.Init.ColorMode    = DMA2D_OUTPUT_ARGB4444; /* DMA2D Output color mode is ARGB4444 (16 bpp) */
  Dma2dHandle.Init.OutputOffset = 0x0; /* No offset in output */    

  /* DMA2D Callbacks Configuration */
  Dma2dHandle.XferCpltCallback  = TransferComplete;
  Dma2dHandle.XferErrorCallback = TransferError;
  
  /* Foreground layer Configuration : layer 1 */
  Dma2dHandle.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  Dma2dHandle.LayerCfg[1].InputAlpha = 0xFF; /* Alpha fully opaque */
  Dma2dHandle.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888; /* Layer 1 input format is ARGB8888 (32 bpp) */
  Dma2dHandle.LayerCfg[1].InputOffset = 0x0; /* No offset in input */

  Dma2dHandle.Instance = DMA2D; 
  
  /* DMA2D Initialization */
  hal_status = HAL_DMA2D_Init(&Dma2dHandle);
  OnError_Handler(hal_status != HAL_OK);
  
  hal_status = HAL_DMA2D_ConfigLayer(&Dma2dHandle, 1);
  OnError_Handler(hal_status != HAL_OK);
}

/**
  * @brief  On Error Handler on condition TRUE.
  * @param  condition : Can be TRUE or FALSE
  * @retval None
  */
static void OnError_Handler(uint32_t condition)
{
  if(condition)
  {
    while(1)
    {
      /* Toggle LED1 with a period of 200 ms */
      BSP_LED_Toggle(LED1);
      HAL_Delay(200);
    }
  }
}

/**
  * @brief  DMA2D Transfer completed callback
  * @param  hdma2d: DMA2D handle. 
  * @note   This example shows a simple way to report end of DMA2D transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
static void TransferComplete(DMA2D_HandleTypeDef *hdma2d)
{
  /* Turn LED1 On */
  BSP_LED_On(LED1);
}

/**
  * @brief  DMA2D error callbacks
  * @param  hdma2d: DMA2D handle
  * @note   This example shows a simple way to report DMA2D transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
static void TransferError(DMA2D_HandleTypeDef *hdma2d)
{
  while (1)
  {
    /* Toggle LED1 with a period of 1 s */
    BSP_LED_Toggle(LED1);
    HAL_Delay(1000);
  }
}

/**
  * @brief LCD Configuration.
  * @note  This function Configure the LTDC peripheral :
  *        1) Configure the Pixel Clock for the LCD
  *        2) Configure the LTDC Timing and Polarity
  *        3) Configure the LTDC Layer 1 :
  *           - ARGB8888 as pixel format  
  *           - The frame buffer is located in internal Flash : original image 300x120 in ARGB8888 format
  *           - The Layer size configuration : 300x120
  *        4) Configure the LTDC Layer 2 :
  *           - ARGB4444 as pixel format  
  *           - The frame buffer is located at internal SRAM : The output of DMA2D transfer
  *           - The Layer size configuration : 300x120                        
  * @retval
  *  None
  */
static void LCD_Config(void)
{ 
  HAL_StatusTypeDef                hal_status = HAL_OK;
  static LTDC_HandleTypeDef        LtdcHandle;
  static LTDC_LayerCfgTypeDef      pLayerCfg;
  static LTDC_LayerCfgTypeDef      pLayerCfg1;
  
/* LTDC Initialization -------------------------------------------------------*/
  
  /* Polarity configuration */
  /* Initialize the horizontal synchronization polarity as active low */
  LtdcHandle.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  /* Initialize the vertical synchronization polarity as active low */ 
  LtdcHandle.Init.VSPolarity = LTDC_VSPOLARITY_AL; 
  /* Initialize the data enable polarity as active low */ 
  LtdcHandle.Init.DEPolarity = LTDC_DEPOLARITY_AL; 
  /* Initialize the pixel clock polarity as input pixel clock */  
  LtdcHandle.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  
  /* Timing configuration for RK043FN48H 480x272 LCD */
  /* Horizontal synchronization width = Hsync - 1 */
  LtdcHandle.Init.HorizontalSync = 40;
  /* Vertical synchronization height = Vsync - 1 */
  LtdcHandle.Init.VerticalSync = 9;
  /* Accumulated horizontal back porch = Hsync + HBP - 1 */
  LtdcHandle.Init.AccumulatedHBP = 53;
  /* Accumulated vertical back porch = Vsync + VBP - 1 */
  LtdcHandle.Init.AccumulatedVBP = 11;
  /* Accumulated active width = Hsync + HBP + Active Width - 1 */
  LtdcHandle.Init.AccumulatedActiveH = 283;
  /* Accumulated active height = Vsync + VBP + Active Height - 1 */
  LtdcHandle.Init.AccumulatedActiveW = 533;
  /* Total height = Vsync + VBP + Active Height + VFP - 1 */
  LtdcHandle.Init.TotalHeigh = 285;
  /* Total width = Hsync + HBP + Active Width + HFP - 1 */
  LtdcHandle.Init.TotalWidth = 565;
  
  /* Configure R,G,B component values for LCD background color */
  LtdcHandle.Init.Backcolor.Blue = 0; /* All black background color */
  LtdcHandle.Init.Backcolor.Green = 0;
  LtdcHandle.Init.Backcolor.Red = 0;

  LtdcHandle.Instance = LTDC;
  
/* Layer1 Configuration ------------------------------------------------------*/
  
  /* Windowing configuration */ 
  /*
     WindowX0 = Horizontal start
     WindowX1 = Horizontal stop      
     WindowY0 = Vertical start 
     WindowY1 = Vertical stop      
  */
  pLayerCfg.WindowX0 = 90; /* Translation by vector (90, 15) for display on LCD of layer 1 */
  pLayerCfg.WindowX1 = 390;
  pLayerCfg.WindowY0 = 15;
  pLayerCfg.WindowY1 = 135;
  
  /* Pixel Format configuration*/ 
  /* Layer 1 is the original image ARGB8888 of size 300x120 from Flash */
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  
  /* Start Address configuration : frame buffer is located at FLASH memory */
  /* Display original buffer from Flash in ARGB8888 on LCD */
  pLayerCfg.FBStartAdress = (uint32_t)&ARGB8888_300x120;
  
  /* Alpha constant (255 totally opaque) */
  pLayerCfg.Alpha = 255;
  
  /* Default Color configuration (configure A,R,G,B component values) */
  pLayerCfg.Alpha0 = 0; /* default color is fully transparent */
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  
  /* Configure blending factors */
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  
  /* Configure the number of lines and number of pixels per line */
  pLayerCfg.ImageWidth  = LAYER_SIZE_X;
  pLayerCfg.ImageHeight = LAYER_SIZE_Y;

  /* Layer2 Configuration ------------------------------------------------------*/
  
  /* Windowing configuration */ 
  /* In this case all the active display area is used to display a picture */
  pLayerCfg1.WindowX0 = 90; /* Translation by vector (90, 137) for display on LCD of layer 2 */
  pLayerCfg1.WindowX1 = 390;
  pLayerCfg1.WindowY0 = 137;
  pLayerCfg1.WindowY1 = 257;
  
  /* Pixel Format configuration*/ 
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_ARGB4444;
  
  /* Start Address configuration : frame buffer is located at FLASH memory */
  /* This is the output buffer in ARGB4444 of the DMA2D with PFC of original buffer in ARGB8888 from Flash */
  pLayerCfg1.FBStartAdress = (uint32_t)&aBufferResult;
  
  /* Alpha constant (255 totally opaque) */
  pLayerCfg1.Alpha = 255;
  
  /* Default Color configuration (configure A,R,G,B component values) */
  pLayerCfg1.Alpha0 = 0; /* fully transparent */
  pLayerCfg1.Backcolor.Blue = 0;
  pLayerCfg1.Backcolor.Green = 0;
  pLayerCfg1.Backcolor.Red = 0;
  
  /* Configure blending factors */
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  
  /* Configure the number of lines and number of pixels per line */
  pLayerCfg1.ImageWidth  = LAYER_SIZE_X;
  pLayerCfg1.ImageHeight = LAYER_SIZE_Y;  
   
  /* Configure the LTDC */  
  hal_status  = HAL_LTDC_Init(&LtdcHandle);
  OnError_Handler(hal_status != HAL_OK);

  /* Assert display enable LCD_DISP pin */
  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_12, GPIO_PIN_SET);

  /* Assert backlight LCD_BL_CTRL pin */
  HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3, GPIO_PIN_SET);

  /* Configure the Background Layer : layer 0 of LTDC : display original image ARGB8888 300x120 */
  hal_status  = HAL_LTDC_ConfigLayer(&LtdcHandle, &pLayerCfg, 0);
  OnError_Handler(hal_status != HAL_OK);
  
  /* Configure the Foreground Layer : layer 1 of LTDC */
  /* Which is DMA2D output image in ARGB4444 of size 300x120 */
  hal_status  = HAL_LTDC_ConfigLayer(&LtdcHandle, &pLayerCfg1, 1);
  OnError_Handler(hal_status != HAL_OK);
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
  HAL_StatusTypeDef ret = HAL_OK;

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

#ifdef USE_FULL_ASSERT
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
