/**
  ******************************************************************************
  * @file    JPEG/JPEG_DecodingUsingFs_DMA/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the HW JPEG to Decode a JPEG file with DMA method.
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

/** @addtogroup JPEG_DecodingUsingFs_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if (JPEG_RGB_FORMAT == JPEG_ARGB8888)
  #define BYTES_PER_PIXEL 4  /* Number of bytes in a pixel */
#elif (JPEG_RGB_FORMAT == JPEG_RGB888)
  #define BYTES_PER_PIXEL 3  /* Number of bytes in a pixel */
#elif (JPEG_RGB_FORMAT == JPEG_RGB565)
  #define BYTES_PER_PIXEL 2  /* Number of bytes in a pixel */
#else
  #error "unknown JPEG_RGB_FORMAT "
#endif /* JPEG_RGB_FORMAT */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;  /* File system object for SD card logical drive */
char SDPath[4]; /* SD card logical drive path */
FIL JPEG_File;  /* File object */

uint32_t JpegProcessing_End = 0;
static uint32_t LCD_X_Size = 0;

JPEG_HandleTypeDef    JPEG_Handle;

static DMA2D_HandleTypeDef    DMA2D_Handle;
static JPEG_ConfTypeDef       JPEG_Info;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void LCD_BriefDisplay(void);
static void LCD_FileErrorDisplay(void);
static void DMA2D_CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t xPos = 0, yPos = 0;
  uint8_t  lcd_status = LCD_OK;

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

  /* Initialize the LED3 (Red LED , set to On when error) */
  BSP_LED_Init(LED3);

  /*##-1- JPEG Initialization ################################################*/
  /* Init The JPEG Color Look Up Tables used for YCbCr to RGB conversion   */
  JPEG_InitColorTables();

   /* Init the HAL JPEG driver */
  JPEG_Handle.Instance = JPEG;
  HAL_JPEG_Init(&JPEG_Handle);

  /*##-2- LCD Configuration ##################################################*/
  /* Initialize the LCD   */

  lcd_status = BSP_LCD_Init();
  if(lcd_status != LCD_OK)
  {
    OnError_Handler();
  }

  BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
  BSP_LCD_SelectLayer(0);


  /* Get the LCD Width */
  LCD_X_Size = BSP_LCD_GetXSize();

  /* Cear LCD */
  BSP_LCD_Clear(LCD_COLOR_BLACK);

  /*##-3- Link the micro SD disk I/O driver ##################################*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {
    /*##-4- Register the file system object to the FatFs module ##############*/
    if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) == FR_OK)
    {
      /*##-5- Open the JPG file with read access #############################*/
      if(f_open(&JPEG_File, "image.jpg", FA_READ) == FR_OK)
      {
        /*##-6- JPEG decoding with DMA (Not Blocking ) Method ################*/
        JPEG_Decode_DMA(&JPEG_Handle, &JPEG_File, JPEG_OUTPUT_DATA_BUFFER);

        /*##-7- Wait till end of JPEG decoding and perfom Input/Output Processing in BackGround  #*/
        do
        {
          JPEG_InputHandler(&JPEG_Handle);
          JpegProcessing_End = JPEG_OutputHandler(&JPEG_Handle);

        }while(JpegProcessing_End == 0);

        /*##-8- Get JPEG Info  ###############################################*/
        HAL_JPEG_GetInfo(&JPEG_Handle, &JPEG_Info);

        /*##-9- Copy RGB decoded Data to the display FrameBuffer  ############*/
        xPos = (BSP_LCD_GetXSize() - JPEG_Info.ImageWidth)/2;
        yPos = (BSP_LCD_GetYSize() - JPEG_Info.ImageHeight)/2;

        DMA2D_CopyBuffer((uint32_t *)JPEG_OUTPUT_DATA_BUFFER, (uint32_t *)LCD_FRAME_BUFFER, xPos , yPos, JPEG_Info.ImageWidth, JPEG_Info.ImageHeight);

        /*##-10- Close the JPG file ##########################################*/
        f_close(&JPEG_File);
      }
      else /* Can't Open JPG file*/
      {
        /* Display example brief   */
        LCD_BriefDisplay();
        /* Display error brief   */
        LCD_FileErrorDisplay();
        OnError_Handler();
      }
    }
  }

  /* Infinite loop */
  while (1)
  {
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
  * @brief  Display Example description.
  * @param  None
  * @retval None
  */
static void LCD_BriefDisplay(void)
{
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, LCD_X_Size, 112);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, LINE(2), (uint8_t *)"JPEG Decoding from uSD Fatfs : DMA Method", CENTER_MODE);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"This example shows how to Decode (with DMA)", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, LINE(6), (uint8_t *)"and  display a JPEG file", CENTER_MODE);
}

/**
  * @brief  Display File access error message.
  * @param  None
  * @retval None
  */
static void LCD_FileErrorDisplay(void)
{
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAtLine(27, (uint8_t *)"Unable to open JPEG file image.jpg");
  BSP_LCD_DisplayStringAtLine(28, (uint8_t *)"Please check that a jpeg file named image.jpg is stored on the uSD1");

}

/**
  * @brief  Copy the Decoded image to the display Frame buffer.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Pointer to destination buffer
  * @param  x: destination horizenatl offset.
  * @param  y: destination Vertical offset.
  * @param  xSize: image width
  * @param  ysize: image Height
  * @retval None
  */
static void DMA2D_CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
{

  uint32_t destination = (uint32_t)pDst + (y * BSP_LCD_GetXSize() + x) * 4;
  uint32_t source      = (uint32_t)pSrc;

  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
  DMA2D_Handle.Init.Mode          = DMA2D_M2M_PFC;
  DMA2D_Handle.Init.ColorMode     = DMA2D_OUTPUT_ARGB8888;
  DMA2D_Handle.Init.OutputOffset  = LCD_X_Size - xsize;
  DMA2D_Handle.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion*/
  DMA2D_Handle.Init.RedBlueSwap   = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */

  /*##-2- DMA2D Callbacks Configuration ######################################*/
  DMA2D_Handle.XferCpltCallback  = NULL;

  /*##-3- Foreground Configuration ###########################################*/
  DMA2D_Handle.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  DMA2D_Handle.LayerCfg[1].InputAlpha = 0xFF;

#if (JPEG_RGB_FORMAT == JPEG_ARGB8888)
  DMA2D_Handle.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;

#elif (JPEG_RGB_FORMAT == JPEG_RGB888)
  DMA2D_Handle.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB888;

#elif (JPEG_RGB_FORMAT == JPEG_RGB565)
  DMA2D_Handle.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;

#endif /* JPEG_RGB_FORMAT * */


  DMA2D_Handle.LayerCfg[1].InputOffset = 0;
  DMA2D_Handle.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; /* No ForeGround Red/Blue swap */
  DMA2D_Handle.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */

  DMA2D_Handle.Instance          = DMA2D;

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&DMA2D_Handle) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&DMA2D_Handle, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&DMA2D_Handle, source, destination, xsize, ysize) == HAL_OK)
      {
        /* Polling For DMA transfer */
        HAL_DMA2D_PollForTransfer(&DMA2D_Handle, 100);
      }
    }
  }
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
#endif /* USE_FULL_ASSERT */

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
  * @brief  On Error Handler.
  * @param  None
  * @retval None
  */
void OnError_Handler(void)
{
  BSP_LED_On(LED3);
  while(1) { ; } /* Blocking on error */
}

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
