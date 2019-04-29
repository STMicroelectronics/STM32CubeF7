/**
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemToMemWithBlending/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to configure
  *          DMA2D peripheral in Memory to Memory with Blending transfer mode
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

#include "RGB565_240x130_1.h"
#include "RGB565_240x130_2.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup DMA2D_MemToMemWithBlending
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMA2D_HandleTypeDef Dma2dHandle;

/* DMA2D output address in SRAM : this is the buffer displayed on LCD screen */
/* The buffer in SRAM is 240x130x2 = 60 KBytes                               */
uint32_t aBlendedImage[(LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_BYTE_PER_PIXEL) / 4];

/* Blended image Ready flag */
static __IO uint32_t   blended_image_ready = 0;
static uint32_t   offset_address_area_blended_image_in_lcd_buffer =  0;

/* Private function prototypes -----------------------------------------------*/
static uint8_t CopyImageToLcdFrameBuffer(void *pSrc, void *pDst, uint32_t xSize, uint32_t ySize);

static void DMA2D_Config(void);
static void TransferError(DMA2D_HandleTypeDef* Dma2dHandle);
static void TransferComplete(DMA2D_HandleTypeDef* Dma2dHandle);
static void SystemClock_Config(void);
static void OnError_Handler(uint32_t condition);

static void CPU_CACHE_Enable(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t  lcd_status = LCD_OK;
  HAL_StatusTypeDef hal_status = HAL_OK;

  blended_image_ready = 0;
  offset_address_area_blended_image_in_lcd_buffer =  ((((WVGA_RES_Y - LAYER_SIZE_Y) / 2) * WVGA_RES_X)
                                                    +   ((WVGA_RES_X - LAYER_SIZE_X) / 2))
                                                    * ARGB8888_BYTE_PER_PIXEL;
  
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

  /* Configure the system clock to 200 MHz */
  SystemClock_Config();

  /* Configure LED1, LED2, and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

 /*##-1- Initialize the LCD #################################################*/

  /* Proceed to LTDC, DSI initialization and LCD screen initialization */
  lcd_status = BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);   
  OnError_Handler(lcd_status != LCD_OK);

  /* Prepare using DMA2D the 800x480 LCD frame buffer to display background color black */
  /* and title of the example                                                           */
  BSP_LCD_Clear(LCD_COLOR_BLACK);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font16);

  /* Print example description */
  BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)"DMA2D_MemToMemWithBlending example", CENTER_MODE);

  HAL_Delay(100);

  /*##-2- Configure DMA2D : Configure foreground and background layers ##############*/
  DMA2D_Config();

  /*##-3- Start DMA2D transfer in interrupt mode ################################################*/
  /*## RGB565_240x130_1[] is the foreground layer and RGB565_240x130_2[] is the background layer */
  hal_status = HAL_DMA2D_BlendingStart_IT(&Dma2dHandle,
                                          (uint32_t)&RGB565_240x130_1,
                                          (uint32_t)&RGB565_240x130_2,
                                          (uint32_t)&aBlendedImage,
                                          LAYER_SIZE_X,
                                          LAYER_SIZE_Y);
  OnError_Handler(hal_status != HAL_OK);

  /* Wait until blended image is ready to be displayed */
  while(blended_image_ready == 0) {;}

 

  while (1)
  {

    /*##-4- Copy Foreground image in center of LCD frame buffer ################################################*/
    
    BSP_LCD_DisplayStringAt(0, 420, (uint8_t *)"Display Foreground Image :           life.augmented", CENTER_MODE); 
    /* Blocking copy Foreground image buffer to LCD frame buffer center area */
    /* Using the DMA2D in polling mode                            */
    lcd_status = CopyImageToLcdFrameBuffer((void*)&(RGB565_240x130_1[0]),
                                                (void*)(LCD_FRAME_BUFFER + offset_address_area_blended_image_in_lcd_buffer),
                                                LAYER_SIZE_X,
                                                LAYER_SIZE_Y);
    OnError_Handler(lcd_status != LCD_OK);  
    HAL_Delay(5000);  

    
    /*##-5- Copy Background image in center of LCD frame buffer ################################################*/

    BSP_LCD_DisplayStringAt(0, 420, (uint8_t *)"Display Background Image :                  ST Logo", CENTER_MODE);
    /* Blocking copy Background image buffer to LCD frame buffer center area */
    /* Using the DMA2D in polling mode  */    
    lcd_status = CopyImageToLcdFrameBuffer((void*)&(RGB565_240x130_2[0]),
                                                (void*)(LCD_FRAME_BUFFER + offset_address_area_blended_image_in_lcd_buffer),
                                                LAYER_SIZE_X,
                                                LAYER_SIZE_Y);
    OnError_Handler(lcd_status != LCD_OK);    
    HAL_Delay(5000); 
  

    /*##-6- Copy Blended image in center of LCD frame buffer ################################################*/
    
    BSP_LCD_DisplayStringAt(0, 420, (uint8_t *)"Display Blended Image    : ST Logo + life.augmented", CENTER_MODE);
    /* Blocking copy Blended image buffer to LCD frame buffer center area */
    /* Using the DMA2D in polling mode  */     
    lcd_status = CopyImageToLcdFrameBuffer((void*)&(aBlendedImage[0]),
                                                (void*)(LCD_FRAME_BUFFER + offset_address_area_blended_image_in_lcd_buffer),
                                                LAYER_SIZE_X,
                                                LAYER_SIZE_Y);
    OnError_Handler(lcd_status != LCD_OK); 
    HAL_Delay(5000); 
  }
}

/**
  * @brief  Copy and convert image (LAYER_SIZE_X, LAYER_SIZE_Y) of format RGB565
  * to LCD frame buffer area centered in WVGA resolution.
  * The area of copy is of size (LAYER_SIZE_X, LAYER_SIZE_Y) in ARGB8888.
  * @param  pSrc: Pointer to source buffer : source image buffer start here
  * @param  pDst: Pointer to destination buffer LCD frame buffer center area start here
  * @param  xSize: Buffer width (LAYER_SIZE_X here)
  * @param  ySize: Buffer height (LAYER_SIZE_Y here)
  * @retval LCD Status : LCD_OK or LCD_ERROR
  */
static uint8_t CopyImageToLcdFrameBuffer(void *pSrc, void *pDst, uint32_t xSize, uint32_t ySize)
{
  DMA2D_HandleTypeDef hdma2d_eval;
  HAL_StatusTypeDef hal_status = HAL_OK;
  uint8_t lcd_status = LCD_ERROR;

  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888; /* Output color out of PFC */
  hdma2d_eval.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion*/  
  hdma2d_eval.Init.RedBlueSwap   = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */   

  /* Output offset in pixels == nb of pixels to be added at end of line to come to the  */
  /* first pixel of the next line : on the output side of the DMA2D computation         */
  hdma2d_eval.Init.OutputOffset = (WVGA_RES_X - LAYER_SIZE_X);

  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF; /* fully opaque */
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d_eval.LayerCfg[1].InputOffset = 0;
  hdma2d_eval.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; /* No ForeGround Red/Blue swap */
  hdma2d_eval.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */
  
  hdma2d_eval.Instance = DMA2D;

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hdma2d_eval, (uint32_t)pSrc, (uint32_t)pDst, xSize, ySize) == HAL_OK)
      {
        /* Polling For DMA transfer */
        hal_status = HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
        if(hal_status == HAL_OK)
        {
          /* return good status on exit */
          lcd_status = LCD_OK;
        }
      }
    }
  }

  return(lcd_status);
}

/**
  * @brief DMA2D configuration.
  * @note  This function Configure the DMA2D peripheral :
  *        1) Configure the Transfer mode as memory to memory with blending.
  *        2) Configure the output color mode as RGB565 pixel format.
  *        3) Configure the Foreground
  *          - Foreground image is loaded from FLASH memory (RGB565_240x130_2[])
  *          - constant alpha value (decreased to see the background)
  *          - color mode as RGB565 pixel format
  *        4) Configure the Background
  *          - Background image loaded from FLASH memory (RGB565_240x130_1[])
  *          - color mode as RGB565 pixel format
  * @retval
  *  None
  */
static void DMA2D_Config(void)
{
  HAL_StatusTypeDef hal_status = HAL_OK;

  /* Configure the DMA2D Mode, Color Mode and output offset */
  Dma2dHandle.Init.Mode         = DMA2D_M2M_BLEND; /* DMA2D mode Memory to Memory with Blending */
  Dma2dHandle.Init.ColorMode    = DMA2D_OUTPUT_RGB565; /* output format of DMA2D */
  Dma2dHandle.Init.OutputOffset = 0x0;  /* No offset in output */
  Dma2dHandle.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion*/  
  Dma2dHandle.Init.RedBlueSwap   = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */   

  /* DMA2D Callbacks Configuration */
  Dma2dHandle.XferCpltCallback  = TransferComplete;
  Dma2dHandle.XferErrorCallback = TransferError;

  /* Foreground layer Configuration */
  Dma2dHandle.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  Dma2dHandle.LayerCfg[1].InputAlpha = 0x7F; /* 127 : semi-transparent */
  Dma2dHandle.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  Dma2dHandle.LayerCfg[1].InputOffset = 0x0; /* No offset in input */
  Dma2dHandle.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; /* No ForeGround Red/Blue swap */
  Dma2dHandle.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */
  
  /* Background layer Configuration */
  Dma2dHandle.LayerCfg[0].AlphaMode = DMA2D_REPLACE_ALPHA;
  Dma2dHandle.LayerCfg[0].InputAlpha = 0x7F; /* 127 : semi-transparent */
  Dma2dHandle.LayerCfg[0].InputColorMode = DMA2D_INPUT_RGB565;
  Dma2dHandle.LayerCfg[0].InputOffset = 0x0; /* No offset in input */
  Dma2dHandle.LayerCfg[0].RedBlueSwap = DMA2D_RB_REGULAR; /* No BackGround Red/Blue swap */
  Dma2dHandle.LayerCfg[0].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No BackGround Alpha inversion */  

  Dma2dHandle.Instance = DMA2D;

  /* DMA2D Initialization */
  hal_status = HAL_DMA2D_Init(&Dma2dHandle);
  OnError_Handler(hal_status != HAL_OK);

  /* Apply DMA2D Foreground configuration */
  HAL_DMA2D_ConfigLayer(&Dma2dHandle, 1);

  /* Apply DMA2D Background configuration */
  HAL_DMA2D_ConfigLayer(&Dma2dHandle, 0);
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
    BSP_LED_On(LED3);
    while(1) { ; } /* Blocking on error */
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

  /* The Blended image is now ready for display */
  blended_image_ready = 1;
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
  /* Turn LED2 On */
  BSP_LED_On(LED2);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 200000000
  *            HCLK(Hz)                       = 200000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 400
  *            PLL_P                          = 2
  *            PLL_Q                          = 8
  *            PLL_R                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 6
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
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 7;
  
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Activate the OverDrive to reach the 200 MHz Frequency */  
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
  
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
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
