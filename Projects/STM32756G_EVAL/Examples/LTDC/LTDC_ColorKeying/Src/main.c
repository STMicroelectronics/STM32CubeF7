/**
  ******************************************************************************
  * @file    LTDC/LTDC_ColorKeying/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to enable and use 
  *          the LTDC color keying functionality.
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
#include "ARGB4444_480x272.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup LTDC_ColorKeying
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PRESSED_FIRST    	0x00
#define PRESSED_SECOND   	0x01
#define ARGB4444_COLOR_KEY	0xFFFF
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
LTDC_HandleTypeDef LtdcHandle;
uint8_t ubPressedButton = PRESSED_FIRST;

__IO uint32_t ReloadFlag = 0;

/* Private function prototypes -----------------------------------------------*/
static void LCD_Config(void); 
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  static uint8_t buttonState = 0;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  
  /* STM32F7xx HAL library initialization:
       - Configure the Flash ART accelerator on ITCM interface
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
  
  /*## LTDC Clock Configuration ###########################################*/  
  if(stmpe811_ts_drv.ReadID(TS_I2C_ADDRESS) == STMPE811_ID)
  {
    /* LCD clock configuration */
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
    
  }
  else
  {
    /* LCD clock configuration */
    /* LCD clock configuration */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 151 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 151/3 = 50.3 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 50.3/2 = 25.16Mhz */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 151;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 3;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);  
  } 
  
  /* Configure LED3 */
  BSP_LED_Init(LED3);   
  
  /*##-1- TAMPER button will be used #########################################*/
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);
  
  /*##-2- LCD Configuration ##################################################*/
  LCD_Config();
  
  /*##-3- Configure Color Keying  ############################################*/
  HAL_LTDC_ConfigColorKeying_NoReload(&LtdcHandle, ARGB4444_COLOR_KEY, LTDC_LAYER_1);  

  while (1)
  {

    if(BSP_PB_GetState(BUTTON_TAMPER) == GPIO_PIN_SET)
    {
      /* user button is pressed     */
      buttonState = 1;
    }
    else if(buttonState == 1)
    {
      /* user button is released     */
      buttonState = 0;
    
      if(ubPressedButton == PRESSED_FIRST)
      {
        /* Enable Color Keying on layer 1 */
        HAL_LTDC_EnableColorKeying_NoReload(&LtdcHandle, LTDC_LAYER_1);
        ubPressedButton = PRESSED_SECOND;  
      }
      else
      {
        /* Disable Color Keying on layer 1 */
        HAL_LTDC_DisableColorKeying_NoReload(&LtdcHandle, LTDC_LAYER_1);
        ubPressedButton = PRESSED_FIRST; 
      }
      
      ReloadFlag = 0;
      HAL_LTDC_Reload(&LtdcHandle,LTDC_RELOAD_VERTICAL_BLANKING);
      
      while(ReloadFlag == 0)
      {
        /* wait till reload takes effect (in the next vertical blanking period) */
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
  * @brief LCD configuration.
  * @note  This function Configure the LTDC peripheral :
  *        1) Configure the Pixel Clock for the LCD
  *        2) Configure the LTDC Timing and Polarity
  *        3) Configure the LTDC Layer 1 :
  *           - The frame buffer is located in FLASH memory
  *           - The Layer size configuration : 480x272 and format ARGB4444 (16 bits/pixel)
  *        4) Configure the Color Keying for Layer 1.    
  * @retval
  *  None
  */
static void LCD_Config(void)
{
  LTDC_LayerCfgTypeDef      pLayerCfg;
  
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
  
  /* Timing configuration */
  if(stmpe811_ts_drv.ReadID(TS_I2C_ADDRESS) == STMPE811_ID)
  {  
    /* The AMPIRE LCD 480x272 is selected */
    /* Timing Configuration */    
    LtdcHandle.Init.HorizontalSync = (AMPIRE480272_HSYNC - 1);
    LtdcHandle.Init.VerticalSync = (AMPIRE480272_VSYNC - 1);
    LtdcHandle.Init.AccumulatedHBP = (AMPIRE480272_HSYNC + AMPIRE480272_HBP - 1);
    LtdcHandle.Init.AccumulatedVBP = (AMPIRE480272_VSYNC + AMPIRE480272_VBP - 1);  
    LtdcHandle.Init.AccumulatedActiveH = (AMPIRE480272_HEIGHT + AMPIRE480272_VSYNC + AMPIRE480272_VBP - 1);
    LtdcHandle.Init.AccumulatedActiveW = (AMPIRE480272_WIDTH + AMPIRE480272_HSYNC + AMPIRE480272_HBP - 1);
    LtdcHandle.Init.TotalHeigh = (AMPIRE480272_HEIGHT + AMPIRE480272_VSYNC + AMPIRE480272_VBP + AMPIRE480272_VFP - 1);
    LtdcHandle.Init.TotalWidth = (AMPIRE480272_WIDTH + AMPIRE480272_HSYNC + AMPIRE480272_HBP + AMPIRE480272_HFP - 1); 
  }
  else
  {
    /* The LCD AMPIRE 640x480 is selected */
    /* Timing configuration */
    LtdcHandle.Init.HorizontalSync = (AMPIRE640480_HSYNC - 1);
    LtdcHandle.Init.VerticalSync = (AMPIRE640480_VSYNC - 1);
    LtdcHandle.Init.AccumulatedHBP = (AMPIRE640480_HSYNC + AMPIRE640480_HBP - 1);
    LtdcHandle.Init.AccumulatedVBP = (AMPIRE640480_VSYNC + AMPIRE640480_VBP - 1);  
    LtdcHandle.Init.AccumulatedActiveH = (AMPIRE640480_HEIGHT + AMPIRE640480_VSYNC + AMPIRE640480_VBP - 1);
    LtdcHandle.Init.AccumulatedActiveW = (AMPIRE640480_WIDTH + AMPIRE640480_HSYNC + AMPIRE640480_HBP - 1);
    LtdcHandle.Init.TotalHeigh = (AMPIRE640480_HEIGHT + AMPIRE640480_VSYNC + AMPIRE640480_VBP + AMPIRE640480_VFP - 1);
    LtdcHandle.Init.TotalWidth = (AMPIRE640480_WIDTH + AMPIRE640480_HSYNC + AMPIRE640480_HBP + AMPIRE640480_HFP - 1); 
  }
  
  /* Configure R,G,B component values for LCD background color : all black */
  LtdcHandle.Init.Backcolor.Blue = 0;
  LtdcHandle.Init.Backcolor.Green = 0;
  LtdcHandle.Init.Backcolor.Red = 0;

  LtdcHandle.Instance = LTDC;
  
/* Layer1 Configuration ------------------------------------------------------*/
  
  /* Windowing configuration */ 
  /* In this case all the active display area is used to display a picture then :
     Horizontal start = horizontal synchronization + Horizontal back porch = 43 
     Vertical start   = vertical synchronization + vertical back porch     = 12
     Horizontal stop = Horizontal start + window width -1 = 43 + 480 -1 
     Vertical stop   = Vertical start + window height -1  = 12 + 272 -1      */
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 480;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 272;
  
  /* Pixel Format configuration */ 
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB4444;
  
  /* Start Address configuration : frame buffer is located in FLASH memory */
  pLayerCfg.FBStartAdress = (uint32_t)&ARGB4444_480x272;
  
  /* Alpha constant (255 totally opaque) */
  pLayerCfg.Alpha = 255;
  
  /* Default Color configuration (configure A,R,G,B component values) */
  /* default color == all transparent pixel                           */
  pLayerCfg.Alpha0 = 0; /* completely transparent */
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  
  /* Configure blending factors */
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  
  /* Configure the number of lines and number of pixels per line */
  pLayerCfg.ImageWidth  = 480;
  pLayerCfg.ImageHeight = 272;

  /* Configure the LTDC */  
  if(HAL_LTDC_Init(&LtdcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /* Configure the Layer 1 */
  if(HAL_LTDC_ConfigLayer(&LtdcHandle, &pLayerCfg, LTDC_LAYER_1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }    
}  

/**
  * @brief  Reload Event callback.
  * @param  hltdc: pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
void HAL_LTDC_ReloadEventCallback(LTDC_HandleTypeDef *hltdc)
{
  ReloadFlag = 1;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
    /* Turn LED3 on */
    BSP_LED_On(LED3);
    while(1)
    {
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
