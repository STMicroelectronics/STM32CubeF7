/**
  ******************************************************************************
  * @file    Camera/Src/main.c
  * @author  MCD Application Team
  * @brief   This application discribes how to configure the camera in continuous 
  *          or snapshots modes and QVGA resolution.
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SNAPSHOT_MODE  1
#define CONTINOUS_MODE 2
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMA2D_HandleTypeDef hdma2d_eval;
static uint32_t CaptureMode  = 0;
static TS_StateTypeDef  TS_State;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void LCD_LL_ConvertLineToARGB8888(void *pSrc, void *pDst);
static void CPU_CACHE_Enable(void);
static void DCMI_GetCaptureMode(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t  i, previous_mode = CONTINOUS_MODE;
  uint32_t  *ptrLcd;
  uint8_t status = CAMERA_OK;
  
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
  
  /*##-1- Initialise the LCD #################################################*/
  BSP_LCD_Init();

  /* Init LCD screen buffer */
  ptrLcd = (uint32_t*)(LCD_FRAME_BUFFER);
  for (i=0; i<(BSP_LCD_GetXSize()*BSP_LCD_GetYSize()); i++)
  {
    ptrLcd[i]=0;
  }
  
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FRAME_BUFFER);
  
  /* Set LCD Foreground Layer  */
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
  
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_BLACK);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(340, 80, 80, 30);
  BSP_LCD_FillRect(340, 150, 80, 30);
  BSP_LCD_DrawRect(8, 8, 322, 242);
  BSP_LCD_DrawRect(9, 9, 321, 241);    
  BSP_LCD_SetFont(&Font12); 
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);  
  BSP_LCD_DisplayStringAt(345, 90, (uint8_t *)"CONTINUOUS", LEFT_MODE);
  BSP_LCD_DisplayStringAt(345, 160, (uint8_t *)" SNAPSHOT", LEFT_MODE);
  
  /*##-2- Camera Initialisation and start capture ############################*/
  /* Initialize the Camera */
  
  status = BSP_CAMERA_Init(RESOLUTION_R320x240);
  if(status != CAMERA_OK)
  {
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect(10, 10, 320, 240);    
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);      
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"CAMERA sensor is unpluged", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 120, (uint8_t *)"Plug the OV9655 sensor   ", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 140, (uint8_t *)"on P1 camera connector   ", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"And restart the program  ", LEFT_MODE);
  }
  else
  {
    BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
    
    /* Start the Camera Capture */
    BSP_CAMERA_ContinuousStart((uint8_t *)CAMERA_FRAME_BUFFER);
    CaptureMode = CONTINOUS_MODE;
  }
  
  while (1)
  {
    if(status == CAMERA_OK)
    {
      DCMI_GetCaptureMode();
      if(previous_mode != CaptureMode)
      {
        if(CaptureMode == CONTINOUS_MODE)
        {
          /* Initialize the Camera */
          BSP_CAMERA_Init(RESOLUTION_R320x240); 
          /* Start the Camera Capture */
          BSP_CAMERA_ContinuousStart((uint8_t *)CAMERA_FRAME_BUFFER);       
        }
        else
        {
          BSP_CAMERA_SnapshotStart((uint8_t *)CAMERA_FRAME_BUFFER); 
        }
        previous_mode = CaptureMode;
      }
    }
  }
}

/**
  * @brief  Update the CaptureMode global variable to handle
  *         the switch between snapshot and continuous modes
  * @retval None
  */
static void DCMI_GetCaptureMode(void)
{
  uint16_t x, y;  
  /* Check in polling mode in touch screen the touch status and coordinates */
  /* if touch occurred                                                      */
  BSP_TS_GetState(&TS_State);
  if(TS_State.touchDetected)
  {
    /* Get X and Y position of the touch post calibrated */
    x = TS_State.touchX[0];
    y = TS_State.touchY[0];
    
    if((x > 340) && (x < 420))
    {
      if((y > 80) && (y < 110))
      {
        CaptureMode = CONTINOUS_MODE;
      }
      if((y > 150) && (y < 180))
      {
        CaptureMode = SNAPSHOT_MODE;
      }
    }
  }
}

/**
  * @brief  Camera line event callback
  * @param  None
  * @retval None
  */
void BSP_CAMERA_LineEventCallback(void)
{
  static uint32_t tmp, tmp2, counter;
  
  if(240 > counter)
  {
    LCD_LL_ConvertLineToARGB8888((uint32_t *)(CAMERA_FRAME_BUFFER + tmp), (uint32_t *)(LCD_FRAME_BUFFER + 0x4B28 + tmp2));
    tmp  = tmp + 320*sizeof(uint16_t); 
    tmp2 = tmp2 + (480) * sizeof(uint32_t);
    counter++;
  }
  else
  {
    tmp = 0;
    tmp2 = 0;
    counter = 0;
  }  
}

/**
  * @brief  Converts a line to an ARGB8888 pixel format.
  * @param  pSrc Pointer to source buffer
  * @param  pDst Output color
  * @param  xSize Buffer width
  * @param  ColorMode Input color mode
  * @retval None
  */
static void LCD_LL_ConvertLineToARGB8888(void *pSrc, void *pDst)
{  
  /* Enable DMA2D clock */
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  hdma2d_eval.Init.OutputOffset = 0;

  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d_eval.LayerCfg[1].InputOffset = 0;

  hdma2d_eval.Instance = DMA2D;
  
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hdma2d_eval, (uint32_t)pSrc, (uint32_t)pDst, 320, 1) == HAL_OK)
      {
        /* Polling For DMA transfer */
        HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
      }
    }
  }
  else
  {
    /* FatFs Initialization Error */
    while(1);
  }
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
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 6
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  HAL_StatusTypeDef ret = HAL_OK;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
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
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Activate the OverDrive to reach the 200 MHz Frequency */  
  ret = HAL_PWREx_ActivateOverDrive();
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
  
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
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
  * @param  file pointer to the source file name
  * @param  line assert_param error line source number
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
