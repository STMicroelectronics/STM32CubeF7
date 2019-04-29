/**
  ******************************************************************************
  * @file    Display/LCD_PicturesFromUSB/Src/main.c
  * @author  MCD Application Team
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V. 
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

/** @addtogroup LCD_PicturesFromUSBCard
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS USB_FatFs;  /* File system object for USB card logical drive */
char USB_Path[4]; /* USB card logical drive path */
USBH_HandleTypeDef hUSBHost;
IMG_ApplicationTypeDef Appli_state = APPLICATION_IDLE;
IMG_DEMO_StateMachine img_demo;
char* pDirectoryFiles[MAX_BMP_FILES];
uint8_t  ubNumberOfFiles = 0;
uint32_t uwBmplen = 0;


/* Private function prototypes -----------------------------------------------*/
static void Images_DisplayProcess(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
static void CPU_CACHE_Enable(void);
static void MPU_ConfigPSRAM(void);
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

  img_demo.state = IMG_DEMO_START;
  
  /* Main infinite loop */
  while(1)
  {
    /* USB Host Background task */
    USBH_Process(&hUSBHost);
    
    /* Images Display Process */
    Images_DisplayProcess();  
  }
}

/**
  * @brief  Images Display Process
  * @param  None
  * @retval None
  */
static void Images_DisplayProcess(void)
{
  uint32_t counter = 0;
  uint8_t str[30];  
  /* Internal Buffer defined in PSRAM memory */
  uint8_t *uwInternalBuffer;
  
  uwInternalBuffer = (uint8_t *)INTERNAL_BUFFER_START_ADDRESS;
  if(Appli_state == APPLICATION_READY)
  { 
    switch(img_demo.state)
    {
    case IMG_DEMO_START:
      if(Appli_state == APPLICATION_READY)
      {
        /*##-3- Initialize the Directory Files pointers (heap) ###################*/
        for (counter = 0; counter < MAX_BMP_FILES; counter++)
        {
          pDirectoryFiles[counter] = malloc(MAX_BMP_FILE_NAME);
          if(pDirectoryFiles[counter] == NULL)
          {
            /* Set the Text Color */
            BSP_LCD_SetTextColor(LCD_COLOR_RED);
            
            BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"  Cannot allocate memory ");
            while(1) { ; }
          }
        }
        img_demo.state = IMG_DEMO_FILE_OPERATIONS;   
      }
      break;
    case IMG_DEMO_FILE_OPERATIONS:
      /* Read and Write File Here */
      if(Appli_state == APPLICATION_READY)
      {
        IMG_File_Operations();
        
        img_demo.state = IMG_DEMO_EXPLORER;
      }
      break;
    case IMG_DEMO_EXPLORER:
      /* Display disk content */
      if(Appli_state == APPLICATION_READY)
      {    
        /* Get the BMP file names on root directory */
        ubNumberOfFiles = Storage_GetDirectoryBitmapFiles("/Media", pDirectoryFiles);
        
        if (ubNumberOfFiles == 0)
        {
          for (counter = 0; counter < MAX_BMP_FILES; counter++)
          {
            free(pDirectoryFiles[counter]);
          }
          /* Clear the LCD */
          BSP_LCD_Clear(LCD_COLOR_BLACK);  
          /* Set the Text Color */
          BSP_LCD_SetTextColor(LCD_COLOR_RED);
          /* Set the Back Color */
          BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
          BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"  No Bitmap files...      ");
          while(1) { ; }
        }
        img_demo.state = IMG_DEMO_DISPLAY;
      } 
      break;
    case IMG_DEMO_DISPLAY:
      /* Display disk content */
      if(Appli_state == APPLICATION_READY)
      {
        /* Clear the LCD */
        BSP_LCD_Clear(LCD_COLOR_BLACK);         
        while ((counter) < ubNumberOfFiles)
        {
          /* Step1 : Display on Foreground layer -------------------------------*/
          /* Format the string */
          sprintf ((char*)str, "Media/%-11.11s", pDirectoryFiles[counter]);
          
          if (Storage_CheckBitmapFile((const char*)str, &uwBmplen) == 0)
          {            
            if ((Storage_CheckBitmapFile((const char*)str, &uwBmplen) == 0) || (counter < (ubNumberOfFiles)))
            {
              /* Format the string */
              sprintf ((char*)str, "Media/%-11.11s", pDirectoryFiles[counter]);
              
              /* Open a file and copy its content to an internal buffer */
              Storage_OpenReadFile(uwInternalBuffer, (const char*)str);
              
              /* Write bmp file on LCD frame buffer */
              BSP_LCD_DrawBitmap(0, 0, uwInternalBuffer);
              
              HAL_Delay(2000);
              if(Appli_state == APPLICATION_READY)
              {
                /* Clear the Background Layer */
                BSP_LCD_Clear(LCD_COLOR_BLACK);
              }
              else
              {
                break;
              }
              counter++;
              if(counter == ubNumberOfFiles)
              {
                counter = 0;
              }
            }
            else if (Storage_CheckBitmapFile((const char*)str, &uwBmplen) == 0)
            {
              /* Clear the LCD */
              BSP_LCD_Clear(LCD_COLOR_BLACK);  
              /* Set the Text Color */
              BSP_LCD_SetTextColor(LCD_COLOR_RED);
              /* Set the Back Color */
              BSP_LCD_SetBackColor(LCD_COLOR_BLACK);             
              BSP_LCD_DisplayStringAtLine(7, (uint8_t *) str);
              BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"    File type not supported. ");
              while(1) { ; }
            }
          }       
        }
      }
      break;
    default:
      break;
    }
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
    if(f_mount(&USB_FatFs, "", 0) != FR_OK)
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
     Normal memory, Shareable, write-back */
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
