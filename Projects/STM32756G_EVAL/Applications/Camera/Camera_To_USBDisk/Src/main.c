/**
  ******************************************************************************
  * @file    Camera/Camera_To_USBDisk/Src/main.c 
  * @author  MCD Application Team
  * @brief   This application describes how to configure the camera in continuous mode
             and save picture under USBDisk.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F7xx_HAL_Applications
  * @{
  */

/** @addtogroup Camera_To_USBDisk
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PRESSED_FIRST    0x00

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t ubPressedButton = PRESSED_FIRST;

FATFS MSC_FatFs;  /* File system object for USB disk logical drive */
FIL MyFile;       /* File object */
char MSC_Path[4]; /* USB Host logical drive path */
USBH_HandleTypeDef  hUSB_Host;

/* Image header */  
const uint32_t aBMPHeader[14]=
{0xFA364D42, 0x00000005, 0x00360000, 0x00280000, 0x01E00000, 0x01100000, 0x00010000, 
 0x00000018, 0xF5400000, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0000};

typedef enum {
  STORAGE_IDLE = 0,  
  STORAGE_READY,    
}MSC_ApplicationTypeDef;

MSC_ApplicationTypeDef Appli_state = STORAGE_IDLE;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void PicturePrepare(void);
static void Error_Handler(void);
static void SavePicture(void);
static void LCD_LL_ConvertLineToARGB8888(void *pSrc, void *pDst);
static void CAMERA_Capture(void);
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32F7xx HAL library initialization:
       - Configure the Flash ART accelerator on ITCM interface
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 200 MHz */
  SystemClock_Config(); 
  
  /* Configure LED1 and LED2 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  
  /*##-1- Initialize the LCD #################################################*/
  /* Two layers are used in this application simultaneously 
     so "LCD_MIN_PCLK" is recommended to programme the PCLK at 20 MHz */   
  BSP_LCD_InitEx(LCD_MIN_PCLK);
  
  /* Foreground Layer Initialization */
  BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER_LAYER1);
  /* Set Foreground Layer */
  BSP_LCD_SelectLayer(1);
  /* Clear the LCD Foreground layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetLayerVisible(1, DISABLE);

  /* Background Layer Initialization */
  BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
  
  /* Enable the LCD */
  BSP_LCD_DisplayOn();   
  
  /* Select the LCD Foreground layer */
  BSP_LCD_SelectLayer(0);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  BSP_LCD_SetLayerWindow(0, 0, 0, 480, 272);
  /*##-2- Init Host Library ##################################################*/

  /* Display USB initialization message */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"USB init ..", RIGHT_MODE);

  USBH_Init(&hUSB_Host, USBH_UserProcess, 0);
  
  /* Add Supported Class */
  USBH_RegisterClass(&hUSB_Host, USBH_MSC_CLASS);

  /* Start Host Process */
  USBH_Start(&hUSB_Host);

  /*##-3- Configure TAMPER Button ############################################*/
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);

  /*##-4- Link the USB Host disk I/O driver ##################################*/
  FATFS_LinkDriver(&USBH_Driver, MSC_Path);

  /*##-5- Camera Initialization and start capture ############################*/

  /* Display camera initialization message */
  BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"Camera init ..", RIGHT_MODE);

  /* Initialize the Camera */
  BSP_CAMERA_Init(RESOLUTION_R480x272);
  
  /* Start the Camera Capture */
  BSP_CAMERA_ContinuousStart((uint8_t *)CAMERA_FRAME_BUFFER);
  
  /*##-6- Run Application ####################################################*/
  while (1)
  {
    /* USB Host Background task */
    USBH_Process(&hUSB_Host);
    
    switch(Appli_state)
    {
    case STORAGE_READY:
      CAMERA_Capture();
      break;
      
    case STORAGE_IDLE:
      break;      
    } 
  }
}

/**
  * @brief  Saves Picture into USB disk.
  * @param  None
  * @retval None
  */
static void SavePicture(void)
{
  FRESULT res1, res2;     /* FatFs function common result code */
  uint32_t byteswritten;  /* File write count */
  static uint32_t counter = 0;
  uint8_t str[30];
  
  /* Suspend the camera capture */
  BSP_CAMERA_Suspend();
  
  /* Prepare the image to be saved */
  PicturePrepare();
  
  /* Set foreground Layer */
  BSP_LCD_SetLayerVisible(1, ENABLE);
  BSP_LCD_SetColorKeying(1, LCD_COLOR_WHITE);
  BSP_LCD_SelectLayer(1);
  BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
  BSP_LCD_SetFont(&Font24);
  
  /* Format the string */
  sprintf((char *)str,"image_%d.bmp", (int)counter);
  
  BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize() - 24), (uint8_t *)"Saving ..", RIGHT_MODE);
  
  /* Create and Open a new text file object with write access */
  if(f_open(&MyFile, (const char*)str, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) 
  {
    Error_Handler();
  }
  else
  {
    /* Write data to the BMP file */
    res1 = f_write(&MyFile, (uint32_t *)aBMPHeader, 54, (void *)&byteswritten);
    res2 = f_write(&MyFile, (uint32_t *)CONVERTED_FRAME_BUFFER, (BSP_LCD_GetXSize()*BSP_LCD_GetYSize()*3), (void *)&byteswritten);
    
    if((res1 != FR_OK) || (res2 != FR_OK) || (byteswritten == 0))
    {
      Error_Handler();
    }
    else
    {
      /* Close the open BMP file */
      f_close(&MyFile);
      
      /* Success of the demo: no error occurrence */
      BSP_LED_On(LED1);
      BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
      BSP_LCD_DisplayStringAt(20, (BSP_LCD_GetYSize()-24), (uint8_t *)"   Saved  ", RIGHT_MODE);
      /* Wait for 2s */
      HAL_Delay(2000);
      /* Disable the Layer 2 */
      BSP_LCD_SetLayerVisible(1, DISABLE);
      /* Select Layer 1 */
      BSP_LCD_SelectLayer(0); 
      counter++;
      BSP_LED_Off(LED1);
      
      /* Resume the camera capture */
      BSP_CAMERA_Resume();      
    }
  }
}

/**
  * @brief  Main routine for Camera capture
  * @param  None
  * @retval None
  */
static void CAMERA_Capture(void)
{
  while(1)
  {
    if(BSP_PB_GetState(BUTTON_TAMPER) != GPIO_PIN_RESET)
    {
      SavePicture();
    }
  }
}

/**
  * @brief  Line event callback.
  * @param  None
  * @retval None
  */
void BSP_CAMERA_LineEventCallback(void)
{
  static uint32_t camera_datapointer, lcd_datapointer, line_number;
  
  if(BSP_LCD_GetYSize() > line_number)
  {
    LCD_LL_ConvertLineToARGB8888((uint32_t *)(CAMERA_FRAME_BUFFER + camera_datapointer), (uint32_t *)(LCD_FRAME_BUFFER + lcd_datapointer));
    camera_datapointer = camera_datapointer + BSP_LCD_GetXSize() * (sizeof(uint16_t)); 
    lcd_datapointer = lcd_datapointer + BSP_LCD_GetXSize() * (sizeof(uint32_t));
    line_number++;
  }
  else
  {
    camera_datapointer = 0;
    lcd_datapointer = 0;
    line_number = 0;
  }
}

/**
  * @brief  Prepares the picture to be Saved in USB.
  * @param  None
  * @retval None
  */
static void PicturePrepare(void) 
{ 
  static DMA2D_HandleTypeDef hdma2d_eval;
  uint32_t address1 = CONVERTED_FRAME_BUFFER;
  uint32_t address2 = LCD_FRAME_BUFFER;
  uint32_t index = 0;
  
  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_OUTPUT_RGB888;
  hdma2d_eval.Init.OutputOffset = 0;     
  
  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d_eval.LayerCfg[1].InputOffset = 0;
  
  hdma2d_eval.Instance = DMA2D; 

  /* Go to the end of the bitmap file address */
  address2 += ((BSP_LCD_GetXSize() * (BSP_LCD_GetYSize() - 1)) * 4);  
  
  /* Convert picture to RGB888 pixel format */
  for(index = 0; index < (BSP_LCD_GetYSize()); index++)
  { 
    /* DMA2D Initialization */
    if(HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK) 
    {
      if(HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK) 
      {
        if (HAL_DMA2D_Start(&hdma2d_eval, address2, address1, (BSP_LCD_GetXSize()), 1) == HAL_OK)
        {
          /* Polling For DMA transfer */  
          HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
        }
      }
    }    
    /* Increment the source and destination buffers */
    address1 += ((BSP_LCD_GetXSize())*3);
    address2 -= BSP_LCD_GetXSize()*4;
  }
}

/**
  * @brief  Converts a line to an ARGB8888 pixel format.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Output color  
  * @retval None
  */
static void LCD_LL_ConvertLineToARGB8888(void *pSrc, void *pDst)
{ 
  static DMA2D_HandleTypeDef hdma2d_eval;  
  
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
      if (HAL_DMA2D_Start(&hdma2d_eval, (uint32_t)pSrc, (uint32_t)pDst, BSP_LCD_GetXSize(), 1) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
      }
    }
  } 
}

/**
  * @brief  User Process
  * @param  phost: Host handle
  * @param  id: Host Library user message ID
  * @retval None
  */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{  
  switch (id)
  { 
  case HOST_USER_SELECT_CONFIGURATION:
    break;
    
  case HOST_USER_DISCONNECTION:
    Appli_state = STORAGE_IDLE;
    f_mount(NULL, (TCHAR const*)"", 0);      
    break;
    
  case HOST_USER_CLASS_ACTIVE:
    /* Register the file system object to the FatFs module */
    if(f_mount(&MSC_FatFs, (TCHAR const*)MSC_Path, 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      Error_Handler();
    }
    else
    {
      Appli_state = STORAGE_READY;
    }
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
  /* Turn LED2 on */
  BSP_LED_On(LED2);
  BSP_LCD_SelectLayer(0); 
  while(1)
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
  HAL_StatusTypeDef ret = HAL_OK;

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
