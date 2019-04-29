/**
  ******************************************************************************
  * @file    WiFi/Esp8266_IAP_Client/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the ESP8266 WiFi module to download
  *          a fw from a http server, write it into the flash then jump to run it.
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "esp8266.h"
#ifdef USE_LCD
#include "lcd_log.h"
#endif /* USE_LCD */
#include "flash_if.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HTML_SUCCESS_STRING "200 OK"
#define HTML_CONTENT_LENGTH_STRING "Content-Length:"


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;
static uint8_t HtmlData[DATA_MAX_SIZE];
static uint8_t HtmlRequest[256];
static uint32_t FlashWriteAddress;
uint8_t IpAddress[15];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void ErrorHandler(void);
static void StartWiFiIAP(void);

#ifdef USE_LCD
 static void LCDConfig(void);
#endif /* USE_LCD */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */


 int main(void)
{
  uint32_t Trial = 0;
  ESP8266_StatusTypeDef Status;

  /* Configure the MPU attributes for PSRAM external memory */
  MPU_Config();

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

  /* Configure leds */
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

  /* Configure Key Button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /* Test if Key Tamper -button on STM32F723E-Discovery Board is not pressed */
  if (BSP_PB_GetState(BUTTON_USER) == 0x00)
  { /* Key Tamper -button not pressed: jump to user application */

    /* Check if valid stack address (RAM address) then jump to user application */
    if (((*(__IO uint32_t*)USER_FLASH_FIRST_PAGE_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
    {
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (USER_FLASH_FIRST_PAGE_ADDRESS + 4);
      Jump_To_Application = (pFunction) JumpAddress;

      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
      Jump_To_Application();
    }
  }

#ifdef USE_LCD
  /* Configure LCD */
  LCDConfig();

  /* Initialize the log unit */
  LCD_LOG_Init();

  /* Write the demo header log */
  LCD_LOG_SetHeader((uint8_t *)"WiFi IAP Demo");
  HAL_Delay(500);
#endif /* USE_LCD */

#ifdef USE_LCD
  LCD_UsrLog("Initializing Wifi module...\n");
#endif /* USE_LCD */

  /* Initialize the WiFi module ESP8266 */
  Status = ESP8266_Init();

  /* Check if initialization passed */
  if (Status != ESP8266_OK)
  {
#ifdef USE_LCD
   LCD_ErrLog("Initializing Wifi module  Failed!\n\n");
#endif /* USE_LCD */
    ErrorHandler();
  }
#ifdef USE_LCD
  else
  {
   LCD_UsrLog("O.K!\n\n");
  }

  /* Join Access Point defined by user */
  LCD_UsrLog("Joining Access Point  ' "WIFI_SSID " ' ...\n");
#endif /* USE_LCD */
  while(ESP8266_JoinAccessPoint((uint8_t *)WIFI_SSID, (uint8_t *)WIFI_PASSWORD) != ESP8266_OK)
  {
#if USE_LCD
    LCD_UsrLog("Retrying ( %d )to Join Access Point  "WIFI_SSID"\n\n",  (int)++Trial);
#endif /* USE_LCD */
    if (Trial == MAX_NUM_TRIAL)
      break;
  }

  /* Check if maximum number of trials has been reached */
  if (Trial == MAX_NUM_TRIAL)
  {
#if USE_LCD
    LCD_ErrLog("Joining Access Point  ' "WIFI_SSID" '  Failed!\n\n");
#endif /* USE_LCD */
    ErrorHandler();
  }
  else
  {
	/* Reset the IP address field to 0 */
    memset(IpAddress, '\0', 15);

	/* Access point joined: start getting IP address */
    ESP8266_GetIPAddress(ESP8266_STATION_MODE, IpAddress);
#ifdef USE_LCD
    LCD_UsrLog("OK!\nGot IP Address: %s\n\n", (char *)IpAddress);
#endif /* USE_LCD */
    HAL_Delay(1000);
   }
  /* IP Address acquired: Start the IAP over WiFi (retrieve the binary file
     for server and download it to Flash memory) */
  StartWiFiIAP();
}

static void StartWiFiIAP(void)
{
  uint8_t *FwData;
  uint32_t FwSize;
  uint32_t DataSize = 0;

  uint8_t *Token = 0;
  uint32_t ExpectedFwSize = 0;

  uint32_t Trial = 0;

  ESP8266_ConnectionInfoTypeDef ConnectionInfo;
  ESP8266_StatusTypeDef Result = ESP8266_OK;

  /* Initialize the HTML request field to 0 */
  memset(HtmlRequest, '\0', 128);

  /*
    Send an HTML GET request to download the "FW_FILE", the request is as follows
   ====================================
    GET /<fw_file> HTTP/1.1
    Host: <http_server>
    User-Agent: <esp8266_for_example>
    Accept: * / *  <= no space here
    Connection: keep-alive
   ====================================
   the request MUST end with a "\r\n\r\n" otherwise it won't be processed correctly.
   */
  sprintf((char *)HtmlRequest, "GET /"FW_FILE" HTTP/1.1%c%cHost: "HOST_ADDRESS"%c%cUser-Agent: esp8266/stm32%c%cAccept: */*%c%cConnection: keep-alive%c%c%c%c",
                      '\r','\n','\r','\n','\r','\n','\r','\n','\r','\n','\r','\n');

#ifdef USE_LCD
    LCD_UsrLog("Connecting to %s:%d ...\n", HOST_ADDRESS, HOST_PORT);
#endif /* USE_LCD */

  /* Initialize Connection info structure */
  Trial = 0;
  memset(&ConnectionInfo, '\0', sizeof (ESP8266_ConnectionInfoTypeDef));

  ConnectionInfo.connectionType = ESP8266_TCP_CONNECTION;
  ConnectionInfo.ipAddress = (uint8_t *)HOST_ADDRESS;
  ConnectionInfo.isServer = ESP8266_FALSE;
  ConnectionInfo.port = HOST_PORT;

  /* Wait for communication establishment */
  while (ESP8266_EstablishConnection(&ConnectionInfo) != ESP8266_OK)
  {
#ifdef USE_LCD
    LCD_UsrLog("Retrying( %d ) to connect to %s:%d \n", (int)++Trial, HOST_ADDRESS, HOST_PORT);
    HAL_Delay(1000);
#endif /* USE_LCD */

    if (Trial == MAX_NUM_TRIAL)
    {
      break;
    }
  }

  /* Check if trials number exceeded maximum limit */
  if (Trial == MAX_NUM_TRIAL)
  {
#ifdef USE_LCD
    LCD_ErrLog("Connecting to %s:%d  Failed!\n", HOST_ADDRESS, HOST_PORT);
    LCD_ErrLog("Quitting Access Point...\n\n");
#endif /* USE_LCD */

    /* Leave the access point */
    ESP8266_QuitAccessPoint();

	/* Deinitialize the WiFi module */
    ESP8266_DeInit();

	/* Call the error Handler */
    ErrorHandler();
  }

#ifdef USE_LCD
  else
  {
    LCD_UsrLog("O.K!\n\n");
    HAL_Delay(1000);
  }
#endif /* USE_LCD */

  /* Reset the HTML data field to 0 */
  memset(HtmlData, '\0', ARRAY_SIZE(HtmlData));

#ifdef USE_LCD
    LCD_UsrLog("Downloading the firmware ' "FW_FILE" '...\n");
#endif /* USE_LCD */

  Result = ESP8266_SendData(HtmlRequest, strlen((char *)HtmlRequest));

  /* In case of error, quit the Access point */
  if (Result != ESP8266_OK)
  {
#ifdef USE_LCD
    LCD_ErrLog("Downloading the firmware ' "FW_FILE" '  Failed!\n");
    LCD_ErrLog("Quitting Access Point...\n\n");
#endif /* USE_LCD */

	/* Deinitialize the WiFi module */
    ESP8266_DeInit();

	/* Call the error Handler */
    ErrorHandler();
  }

  /* Check whether the data was correctly received.
     This is can be done by checking the GET request response.
     in case of success the string '200 OK' is received.
     see (https://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html) */
  Result = ESP8266_ReceiveData(HtmlData, DATA_MAX_SIZE, &DataSize);

  /* If data reception failed */
  if (Result != ESP8266_OK)
  {
#ifdef USE_LCD
    LCD_ErrLog("Downloading the firmware ' "FW_FILE" '  Failed!\n");
    LCD_ErrLog("Quitting Access Point...\n\n");
#endif /* USE_LCD */

    /* Leave the access point */
    ESP8266_QuitAccessPoint();

	/* Deinitialize the WiFi module */
    ESP8266_DeInit();

	/* Call the error Handler */
    ErrorHandler();
  }

  /* If data reception passed */
  if(strstr((char *)HtmlData, HTML_SUCCESS_STRING) == NULL)
  {
#ifdef USE_LCD
    LCD_ErrLog("Downloading the firmware ' "FW_FILE" '  Failed!\n");
    LCD_ErrLog("Quitting Access Point...\n\n");
#endif /* USE_LCD */

    /* Leave the access point */
    ESP8266_QuitAccessPoint();

	/* Deinitialize the WiFi module */
    ESP8266_DeInit();

	/* Call the error Handler */
    ErrorHandler();
  }

  /* extract the expected FW size from the HTML header */

  Token = (uint8_t *)strstr((char *)HtmlData, HTML_CONTENT_LENGTH_STRING);

  Token += strlen(HTML_CONTENT_LENGTH_STRING);
  ExpectedFwSize = atoi ((char *)Token);

  /* The actual fw data is located after the HTML HEADER
   the end of header is marked with the string "\r\n\r\n" */
  FwData = (uint8_t *)(strstr((char *)HtmlData, "\r\n\r\n"));

  /* Check if data is null */
  if (FwData != NULL)
  {
	 /* Increment data pointer by 4 to get the data load */
     FwData += 4;

    /* Stop WiFi module to avoid errors when erasing flash */
    ESP8266_DeInit();

    /* The actual fw data is provided  in the html header
       by looking at the "Content-Length" field.
       let's check that we received the tatal data size */
    FwSize = DataSize - (FwData - HtmlData);
    if (FwSize == ExpectedFwSize)
    {
#ifdef USE_LCD
      LCD_UsrLog("Downloaded firmware ' %s ', size = %ld Bytes\n\n", FW_FILE, FwSize);
      HAL_Delay(1000);
#endif
    }
    else
    {
#ifdef USE_LCD
      LCD_ErrLog("Corrupted firmware: Downloaded = %ld Bytes, expected = %ld Bytes\n\n", FwSize, ExpectedFwSize);
#endif /* USE_LCD */
      ErrorHandler();
    }
    /* Now the firmware can be written into Flash memory */
#ifdef USE_LCD
    LCD_UsrLog("State: Programming...\n");
#endif /* USE_LCD */

    /* Initialize the Flash memory module for write operations */
    FLASH_If_Init();

    /* Erase Flash sectors */
    FLASH_If_Erase(USER_FLASH_FIRST_PAGE_ADDRESS);

    /* Set the write address */
    FlashWriteAddress = USER_FLASH_FIRST_PAGE_ADDRESS;

    /* Start writing the binary file into the Flash memory */
    if (FLASH_If_Write(&FlashWriteAddress, (uint32_t*)FwData, FwSize) != 0)
    {
#ifdef USE_LCD
      LCD_ErrLog("Programming Failed !\n");
#endif /* USE_LCD */
		/* Leave the access point */
    ESP8266_QuitAccessPoint();

	  /* Deinitialize the WiFi module */
    ESP8266_DeInit();

	  /* Call the error Handler */
    ErrorHandler();
    }
    else
    {
#ifdef USE_LCD
     LCD_UsrLog("Programming Done !\n");
#endif
    }
  }
  else
  {
#ifdef USE_LCD
    LCD_ErrLog("The firmware ' "FW_FILE" ' is empty\n");
    LCD_ErrLog("Quitting Access Point...\n\n");
#endif /* USE_LCD */

    /* Leave the access point */
    ESP8266_QuitAccessPoint();

	/* Deinitialize the WiFi module */
    ESP8266_DeInit();

	/* Call the error Handler */
    ErrorHandler();
  }

  /* Stop WiFi module */
  ESP8266_DeInit();

  /* Set LED6 On */
  BSP_LED_On(LED6);

  while(ESP8266_TRUE)
  {
    HAL_Delay(300);
    BSP_LED_Toggle(LED6);
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void ErrorHandler(void)
{
  /* Stop WiFi Module */
  ESP8266_DeInit();

  /* Keep blinking LED5 */
  while(1)
  {
    HAL_Delay(100);
    BSP_LED_Toggle(LED5);
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
  HAL_StatusTypeDef  Result = HAL_OK;

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

  Result = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(Result != HAL_OK)
  {
    while(1) { ; }
  }

  /* Activate the OverDrive to reach the 216 MHz Frequency */
  Result = HAL_PWREx_EnableOverDrive();
  if(Result != HAL_OK)
  {
    while(1) { ; }
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  Result = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(Result != HAL_OK)
  {
    while(1) { ; }
  }
}


/**
  * @brief Configure the MPU attributes for PSRAM and SRAM mapped
  * @param None
  * @retval None
  */
static void MPU_Config(void)
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

#ifdef USE_LCD
/**
  * @brief  This function intializes the LCD Configuration.
  * @param  None
  * @retval None
  */
static void LCDConfig(void)
{
  uint8_t LCDStatus = LCD_OK;

  /*  BSP_LCD_InitEx(LCD_ORIENTATION_PORTRAIT); */
  BSP_LCD_Init();

  /* If LCD intilization fails, stop the program */
  while(LCDStatus != LCD_OK)
  {}

  /* Clear the Background Layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
}
#endif /* USE_LCD */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
