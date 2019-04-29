/**
  ******************************************************************************
  * @file    WiFi/Esp8266_IAP_Client/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the ESP8266 WiFi module to download 
  *          a fw from a http server, write it into the flash then jump to run it.
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
#define VSYNC               1 
#define VBP                 1 
#define VFP                 1
#define VACT                480
#define HSYNC               1
#define HBP                 1
#define HFP                 1
#define HACT                800

#define LAYER0_ADDRESS               (LCD_FB_START_ADDRESS)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;
static uint8_t HtmlData[DATA_MAX_SIZE];
static uint8_t HtmlRequest[256];
static uint32_t FlashWriteAddress;
uint8_t IpAddress[15];

/* Private typedef -----------------------------------------------------------*/
extern LTDC_HandleTypeDef hltdc_discovery;
extern DSI_HandleTypeDef hdsi_discovery;
DSI_CmdCfgTypeDef CmdCfg;
DSI_LPCmdTypeDef LPCmd;
DSI_PLLInitTypeDef dsiPllInit;
static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void ErrorHandler(void);
static void StartWiFiIAP(void);
static void CPU_CACHE_Enable(void);


#ifdef USE_LCD
 static void LCDConfig(void);
 static void LTDC_Init(void);
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

  /* Initialize the SDRAM */
  BSP_SDRAM_Init();
  
  /* Configure leds */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  
  /* Configure Key Button */      
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);  

  /* Test if Key Tamper -button on STM32F769I-Discovery Board is not pressed */
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
    BSP_LCD_LayerDefaultInit(0, LAYER0_ADDRESS);     
  BSP_LCD_SelectLayer(0); 
  
  /* Initialize the log unit */
  LCD_LOG_Init();
  /* Send Display On DCS Command to display */
  HAL_DSI_ShortWrite(&(hdsi_discovery),
                     0,
                     DSI_DCS_SHORT_PKT_WRITE_P1,
                     OTM8009A_CMD_DISPON,
                     0x00);

  BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
  BSP_LCD_FillRect(0, 0, 800, 112);
  
  /* Write the demo header log */
  LCD_LOG_SetHeader((uint8_t *)"WiFi IAP Demo");
    /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
  HAL_Delay(1000);
#endif /* USE_LCD */
  
#ifdef USE_LCD
  LCD_UsrLog("Initializing Wifi module...\n");
    /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
#endif /* USE_LCD */

  /* Initialize the WiFi module ESP8266 */
  Status = ESP8266_Init();
   
  /* Check if initialization passed */
  if (Status != ESP8266_OK)
  {
#ifdef USE_LCD
   LCD_ErrLog("Initializing Wifi module  Failed!\n\n");
     /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
#endif /* USE_LCD */
    ErrorHandler();
  }
#ifdef USE_LCD
  else
  {
   LCD_UsrLog("O.K!\n\n");
     /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
  }
  
  /* Join Access Point defined by user */
  LCD_UsrLog("Joining Access Point  ' "WIFI_SSID " ' ...\n");
    /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
#endif /* USE_LCD */
  while(ESP8266_JoinAccessPoint((uint8_t *)WIFI_SSID, (uint8_t *)WIFI_PASSWORD) != ESP8266_OK)
  {
#if USE_LCD
    LCD_UsrLog("Retrying ( %d )to Join Access Point  "WIFI_SSID"\n\n",  (int)++Trial);
      /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
#endif /* USE_LCD */
    if (Trial == MAX_NUM_TRIAL)
      break;
  }
  
  /* Check if maximum number of trials has been reached */
  if (Trial == MAX_NUM_TRIAL)
  {
#if USE_LCD
    LCD_ErrLog("Joining Access Point  ' "WIFI_SSID" '  Failed!\n\n");
      /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
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
      /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
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
      /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
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
      /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
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
      /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
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
      /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
    HAL_Delay(1000);
  }
#endif /* USE_LCD */
  
  /* Reset the HTML data field to 0 */
  memset(HtmlData, '\0', ARRAY_SIZE(HtmlData));

#ifdef USE_LCD
    LCD_UsrLog("Downloading the firmware ' "FW_FILE" '...\n");
      /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
#endif /* USE_LCD */

  Result = ESP8266_SendData(HtmlRequest, strlen((char *)HtmlRequest));

  /* In case of error, quit the Access point */  
  if (Result != ESP8266_OK)
  {
#ifdef USE_LCD
    LCD_ErrLog("Downloading the firmware ' "FW_FILE" '  Failed!\n");
    LCD_ErrLog("Quitting Access Point...\n\n");
      /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
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
      /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
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
      /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
#endif /* USE_LCD */

    /* Leave the access point */
    ESP8266_QuitAccessPoint();
	
	/* Deinitialize the WiFi module */
    ESP8266_DeInit();
    
	/* Call the error Handler */
    ErrorHandler();
  }
  
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
    
    /* The actual fw data size can be retrieved from the html header
       by looking at the "Content-Length" field.
       but let's do it in a faster way: FwSize = DataSize - (FwData - HtmlData) */
    FwSize = DataSize - (FwData - HtmlData);
  
#ifdef USE_LCD
    LCD_UsrLog("Downloaded firmware ' %s ', size = %ld Bytes\n\n", FW_FILE, FwSize);
      /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
    HAL_Delay(1000);
#endif /* USE_LCD */
  
    /* Now the firmware can be written into Flash memory */
#ifdef USE_LCD
    LCD_UsrLog("State: Programming...\n");
      /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
#endif /* USE_LCD */

    /* Initialize the Flash memory module for write operations */
    FLASH_If_Init();

    /* Erase 2 sectors, 128KB each (the downloaded firmware size doesn't exceed 192KB) */
    FLASH_If_Erase(USER_FLASH_FIRST_PAGE_ADDRESS);
    FLASH_If_Erase(USER_FLASH_SECOND_PAGE_ADDRESS);

	/* Set the write address */
    FlashWriteAddress = USER_FLASH_FIRST_PAGE_ADDRESS; 
  
    /* Start writing the binary file into the Flash memory */
    if (FLASH_If_Write(&FlashWriteAddress, (uint32_t*)FwData, FwSize) != 0)
    {
#ifdef USE_LCD
      LCD_ErrLog("Programming Failed !\n");
        /*Refresh the LCD display*/
      HAL_DSI_Refresh(&hdsi_discovery);
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
       /*Refresh the LCD display*/
     HAL_DSI_Refresh(&hdsi_discovery);
#endif
    }
  }
  else
  {
#ifdef USE_LCD
    LCD_ErrLog("The firmware ' "FW_FILE" ' is empty\n");
    LCD_ErrLog("Quitting Access Point...\n\n");
      /*Refresh the LCD display*/
    HAL_DSI_Refresh(&hdsi_discovery);
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

  /* Set LED2 On */
  BSP_LED_On(LED2);

  while(ESP8266_TRUE)
  {
    HAL_Delay(300);
    BSP_LED_Toggle(LED2);
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
  
  /* Keep blinking LED1 */
  while(1)
  {
    HAL_Delay(100);
    BSP_LED_Toggle(LED1);
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
  RCC_OscInitStruct.PLL.PLLN = 400;  
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  RCC_OscInitStruct.PLL.PLLR = 7;

  
  Result = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(Result != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Activate the OverDrive to reach the 200 MHz Frequency */  
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
  DSI_PHY_TimerTypeDef  PhyTimings;
  
  /* Toggle Hardware Reset of the DSI LCD using
  * its XRES signal (active low) */
  BSP_LCD_Reset();
  
  /* Call first MSP Initialize only in case of first initialization
  * This will set IP blocks LTDC, DSI and DMA2D
  * - out of reset
  * - clocked
  * - NVIC IRQ related to IP blocks enabled
  */
  BSP_LCD_MspInit();
  
  /* LCD clock configuration */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 417 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 417 MHz / 5 = 83.4 MHz */
  /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 83.4 / 2 = 41.7 MHz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 417;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  
  /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
  hdsi_discovery.Instance = DSI;
  
  HAL_DSI_DeInit(&(hdsi_discovery));
  
  dsiPllInit.PLLNDIV  = 100;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV5;
  dsiPllInit.PLLODF   = DSI_PLL_OUT_DIV1;  

  hdsi_discovery.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  hdsi_discovery.Init.TXEscapeCkdiv = 0x4;
  HAL_DSI_Init(&(hdsi_discovery), &(dsiPllInit));
    
  /* Configure the DSI for Command mode */
  CmdCfg.VirtualChannelID      = 0;
  CmdCfg.HSPolarity            = DSI_HSYNC_ACTIVE_HIGH;
  CmdCfg.VSPolarity            = DSI_VSYNC_ACTIVE_HIGH;
  CmdCfg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
  CmdCfg.ColorCoding           = DSI_RGB888;
  CmdCfg.CommandSize           = HACT;
  CmdCfg.TearingEffectSource   = DSI_TE_DSILINK;
  CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
  CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
  CmdCfg.AutomaticRefresh      = DSI_AR_DISABLE;
  CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_ENABLE;
  HAL_DSI_ConfigAdaptedCommandMode(&hdsi_discovery, &CmdCfg);
  
  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_ENABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_ENABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_ENABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_ENABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_ENABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_ENABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_ENABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_ENABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_ENABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_ENABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_ENABLE;
  HAL_DSI_ConfigCommand(&hdsi_discovery, &LPCmd);

  /* Initialize LTDC */
  LTDC_Init();
  
  /* Start DSI */
  HAL_DSI_Start(&(hdsi_discovery));

  /* Configure DSI PHY HS2LP and LP2HS timings */
  PhyTimings.ClockLaneHS2LPTime = 35;
  PhyTimings.ClockLaneLP2HSTime = 35;
  PhyTimings.DataLaneHS2LPTime = 35;
  PhyTimings.DataLaneLP2HSTime = 35;
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime = 10;
  HAL_DSI_ConfigPhyTimer(&hdsi_discovery, &PhyTimings);  
    
  /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver)
  *  depending on configuration set in 'hdsivideo_handle'.
  */
  OTM8009A_Init(OTM8009A_COLMOD_RGB888, LCD_ORIENTATION_LANDSCAPE);
  
  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_DISABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_DISABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_DISABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_DISABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_DISABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_DISABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_DISABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_DISABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_DISABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_DISABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_DISABLE;
  HAL_DSI_ConfigCommand(&hdsi_discovery, &LPCmd);
  
   HAL_DSI_ConfigFlowControl(&hdsi_discovery, DSI_FLOW_CONTROL_BTA);

  /* Send Display Off DCS Command to display */
  HAL_DSI_ShortWrite(&(hdsi_discovery),
                     0,
                     DSI_DCS_SHORT_PKT_WRITE_P1,
                     OTM8009A_CMD_DISPOFF,
                     0x00);
 
  /* Refresh the display */
    HAL_DSI_Refresh(&hdsi_discovery);
}

/**
  * @brief  
  * @param  None
  * @retval None
  */
static void LTDC_Init(void)
{
  /* DeInit */
  HAL_LTDC_DeInit(&hltdc_discovery);
  
  /* LTDC Config */
  /* Timing and polarity */
  hltdc_discovery.Init.HorizontalSync = HSYNC;
  hltdc_discovery.Init.VerticalSync = VSYNC;
  hltdc_discovery.Init.AccumulatedHBP = HSYNC+HBP;
  hltdc_discovery.Init.AccumulatedVBP = VSYNC+VBP;
  hltdc_discovery.Init.AccumulatedActiveH = VSYNC+VBP+VACT;
  hltdc_discovery.Init.AccumulatedActiveW = HSYNC+HBP+HACT;
  hltdc_discovery.Init.TotalHeigh = VSYNC+VBP+VACT+VFP;
  hltdc_discovery.Init.TotalWidth = HSYNC+HBP+HACT+HFP;
  
  /* background value */
  hltdc_discovery.Init.Backcolor.Blue = 0;
  hltdc_discovery.Init.Backcolor.Green = 0;
  hltdc_discovery.Init.Backcolor.Red = 0;
  
  /* Polarity */
  hltdc_discovery.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc_discovery.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc_discovery.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc_discovery.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc_discovery.Instance = LTDC;

  HAL_LTDC_Init(&hltdc_discovery);
}

#endif /* USE_LCD */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
