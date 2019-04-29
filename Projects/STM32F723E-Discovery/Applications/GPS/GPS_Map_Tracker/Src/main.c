/**
  ******************************************************************************
  * @file    GPS/GPS_Map_Tracker/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the nano click GPS module to identify
  *          global earth position.
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
#include "nano_gps_image.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LCD_USE
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if defined LCD_USE  
uint8_t Refresh = 1, RefreshSat =1, RefreshAlt =1, RefreshFix=1, RefreshLat =1,  RefreshTime = 1;
uint8_t RefreshLong =1, RefreshSpeed = 1;
extern char GPS_Time[];
extern char GPS_Date[];
extern char GPS_Fix[];
extern char GPS_SatInView[];
extern char GPS_SatForFix[];
extern char GPS_latitude[];
extern char GPS_Longitude[];
extern char GPS_Speed[];
extern char GPS_Altitude[];

char PrevGPS_Time[20];
char PrevGPS_Date[14];
char PrevGPS_Fix[2];
char PrevGPS_SatInView[4];
char PrevGPS_SatForFix[4];
char PrevGPS_latitude[15];
char PrevGPS_Longitude[15];
char PrevGPS_Speed[8];
char PrevGPS_Altitude[8];
#endif
extern UART_HandleTypeDef  STMOD_UART_Handle;
uint8_t  GPS_Rxbuffer[STMOD_RX_BUF_SIZE];  /* GPS UART reception buffer */

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void ErrorHandler(void);
static void MPU_ConfigPSRAM(void);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t                status;
  
  /* Configure the MPU attributes for PSRAM external memory */
  MPU_ConfigPSRAM();
  
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  
  /* STM32F7xx HAL library initialization:
  - Configure the Flash ART accelerator on ITCM interface
  - Configure the Systick to ge nerate an interrupt each 1 msec
  - Set NVIC Group Priority to 4
  - Global MSP (MCU Support Package) initialization
  */
  HAL_Init();
  
  /* Configure the system clock to 216 MHz */
  SystemClock_Config();
  
  /* Configure leds */
  BSP_LED_Init(LED5);

#if defined LCD_USE  
  status = BSP_LCD_InitEx(LCD_ORIENTATION_LANDSCAPE_ROT180);
  
  if (status != LCD_OK)
  {
    ErrorHandler();
  }
  
  BSP_LCD_DisplayOn();
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  
  /* Initialize the TS in IT mode if not already initialized */
  if (TouchScreen_IsCalibrationDone() == 0)
  {  
    Touchscreen_Calibration();
  }
  BSP_TS_ITConfig();
  
  
  BSP_LCD_Clear( LCD_COLOR_WHITE );
#endif  
  /* Start GPS Demo */
  GPS_Demo();
  
  while(1); 
}

/**
  * @brief  Start GPS demo
  * @param  None
  * @retval None
  */
void GPS_Demo( void )
{
  uint16_t read_idx = STMOD_RX_BUF_SIZE;       /* read index on the reception buffer*/
  
  /* Clear STMOD RX Buffer */
  memset(GPS_Rxbuffer, 0x00, STMOD_RX_BUF_SIZE); 
#if defined LCD_USE    
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
  
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(10, 10, (uint8_t *)"GPS Demo ", CENTER_MODE );
  
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"Connect Fanout board");
  BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"to the connector P1");
   BSP_LCD_DisplayStringAtLine(6, (uint8_t *)"& connect GPS module");
  BSP_LCD_DisplayStringAtLine(7, (uint8_t *)"on CN10 and CN11");
  
  BSP_LCD_DrawBitmap(170, 200, (uint8_t *)next);
  
  while(TouchScreen_GetTouchButtonPosition() != 1);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
  BSP_LCD_ClearStringLine(4);
  BSP_LCD_ClearStringLine(5);
  BSP_LCD_ClearStringLine(6);
  BSP_LCD_ClearStringLine(7);
  BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"ST-LINK interface is ");
  BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"used as VCP to Google");
  BSP_LCD_DisplayStringAtLine(6, (uint8_t *)"earth application");

  while(TouchScreen_GetTouchButtonPosition() != 1);
#endif
  /* Initialize GPS module */  
  GPS_Init();

  /* Enable UART Reception */
  GPS_Start((uint8_t*) GPS_Rxbuffer, STMOD_RX_BUF_SIZE);
 
#if defined LCD_USE    
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
  BSP_LCD_ClearStringLine(4);
  BSP_LCD_ClearStringLine(5);
  BSP_LCD_ClearStringLine(6);
  BSP_LCD_ClearStringLine(7);
  BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"Open Google Earth App:");
  BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"Choose Tools => GPS");
  BSP_LCD_DisplayStringAtLine(6, (uint8_t *)"=>Realtime => ");
  BSP_LCD_DisplayStringAtLine(7, (uint8_t *)"Automatically follow");
  BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"=> Start");
  
  while(TouchScreen_GetTouchButtonPosition() != 1);
  BSP_LCD_Clear( LCD_COLOR_WHITE );
  
  /* Set Display frame hint */
  BSP_LCD_DrawBitmap(159, 159, (uint8_t *)earth);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(10, 5, (uint8_t *)"GPS data ", CENTER_MODE );  
  BSP_LCD_SetFont(&Font16);
#endif  
  /* Initialize VCP: STlink as VCP */
  VCP_Init();
    
  while(1)
  {
    GPS_ReceiveMsg( &read_idx, 1000 );
#if defined LCD_USE      
    GPS_UpdateDisplay();
#endif
  }
  
}

#if defined LCD_USE  
/**
* @brief  Display NMEA Frame 
* @param  None
* @retval None
*/
void GPS_UpdateDisplay(void)
{
  char *nofix[]={"None", "2D","3D","---","------"};
  char *tmp_str;
  
  uint8_t LcdTimeStr[30] = {0};
  uint8_t LcdFixStr[10] = {0};
  uint8_t LcdSatStr[20] = {0};
  uint8_t LcdDateStr[20] = {0};
  uint8_t LcdLatStr[20] = {0};
  uint8_t LcdLongStr[20] = {0};
  uint8_t LcdAltStr[20] = {0};
  uint8_t LcdSpeedStr[20] = {0};
  
  /* Display GPS FIX */
  if(strcasecmp(GPS_Fix, PrevGPS_Fix) !=0)
  {
    strcpy(PrevGPS_Fix, GPS_Fix );     
    switch (GPS_Fix[0])
    {
    case '1': 
      tmp_str= nofix[0];
      break;
    case '2': 
      tmp_str= nofix[1];
      break;
    case '3': 
      tmp_str= nofix[2]; 
      break;
    default: 
      tmp_str= nofix[3]; 
      break;
    }
    
    sprintf((char*)LcdFixStr,"Fix: %s", tmp_str);
    BSP_LCD_ClearStringLine(2);
    BSP_LCD_DisplayStringAtLine(2, (uint8_t *) LcdFixStr);
    RefreshFix = 1;
  }
  else if( RefreshFix == 1)
  {
    switch (PrevGPS_Fix[0])
    {
    case '1': 
      tmp_str= nofix[0];
      break;
    case '2': 
      tmp_str= nofix[1];
      break;
    case '3': 
      tmp_str= nofix[2]; 
      break;
    default: 
      tmp_str= nofix[3]; 
      break;
    }
    
    sprintf((char*)LcdFixStr,"Fix: %s", tmp_str);
    BSP_LCD_ClearStringLine(2);
    BSP_LCD_DisplayStringAtLine(2, (uint8_t *) LcdFixStr);
    RefreshFix =0;
  }
  
  /* Display GPS satelite */
  if(strcasecmp(GPS_SatForFix, PrevGPS_SatForFix) !=0)
  {
    strcpy(PrevGPS_SatForFix, GPS_SatForFix );     
    if ( GPS_SatForFix[0] != '\0' ) 
    {
      tmp_str = GPS_SatForFix;
    }
    else
    {
      tmp_str= nofix[3];
    }
    
    /* Display time Format : hh:mm:ss */
    sprintf((char*)LcdSatStr,"Sat for Fix: %s",tmp_str);
    BSP_LCD_ClearStringLine(3);
    BSP_LCD_DisplayStringAtLine(3, (uint8_t *) LcdSatStr);
    RefreshSat = 1;
  }
  else if( RefreshSat == 1)
  {
    if ( PrevGPS_SatForFix[0] != '\0' ) 
    {
      
      tmp_str = PrevGPS_SatForFix;
      
    }
    else
    {
      tmp_str = nofix[3];
    }

    sprintf((char*)LcdSatStr,"Sat for Fix: %s",tmp_str);
    BSP_LCD_ClearStringLine(3);
    BSP_LCD_DisplayStringAtLine(3, (uint8_t *) LcdSatStr);
    RefreshSat = 0;
  }
  
  /* Display GPS satelite */
  if(strcasecmp(GPS_Time, PrevGPS_Time) !=0)
  {
    strcpy(PrevGPS_Time, GPS_Time ); 
    /* Dispaly time */
    if (GPS_Time[0] != '\0') 
    {
      tmp_str = GPS_Time;
    }
    else 
    {
      tmp_str= nofix[4];
    }
    
    /* Display time Format : hh:mm:ss */
    sprintf((char*)LcdTimeStr,"Time: %c%c:%c%c:%c%c (UTC)", tmp_str[0],tmp_str[1],tmp_str[2],tmp_str[3],tmp_str[4],tmp_str[5]);
    BSP_LCD_ClearStringLine(4);
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *) LcdTimeStr);
        RefreshTime = 1;
  }
  else if (RefreshTime==1)
  {
    /* Dispaly time */
    if (PrevGPS_Time[0] != '\0') 
    {
      tmp_str = PrevGPS_Time;
    }
    else 
    {
      tmp_str= nofix[4];
    }
    
    /* Display time Format : hh:mm:ss */
    sprintf((char*)LcdTimeStr,"Time: %c%c:%c%c:%c%c (UTC)", tmp_str[0],tmp_str[1],tmp_str[2],tmp_str[3],tmp_str[4],tmp_str[5]);
    BSP_LCD_ClearStringLine(4);
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *) LcdTimeStr);
     RefreshTime = 0;
  }
  
  /* Display Date */
  if(strcasecmp(GPS_Date, PrevGPS_Date) !=0)
  {
    strcpy(PrevGPS_Date, GPS_Date );  
    
    if ( GPS_Date[0] != '\0' ) 
    {
      tmp_str = GPS_Date;
    }
    else  
    { 
      tmp_str= nofix[4];
    }
    
    sprintf((char*)LcdDateStr,"Date: %c%c/%c%c/%c%c", tmp_str[0],tmp_str[1],tmp_str[2],tmp_str[3],tmp_str[4],tmp_str[5]);
    BSP_LCD_ClearStringLine(5);
    BSP_LCD_DisplayStringAtLine(5, (uint8_t *) LcdDateStr);
    Refresh = 1;
  }
  else if( Refresh == 1)
  {
    if ( PrevGPS_Date[0] != '\0' ) 
    {
      
      tmp_str = PrevGPS_Date;
      
    }
    else
    {
      tmp_str= nofix[4];
    }
    sprintf((char*)LcdDateStr,"Date: %c%c/%c%c/%c%c", tmp_str[0],tmp_str[1],tmp_str[2],tmp_str[3],tmp_str[4],tmp_str[5]);
    BSP_LCD_ClearStringLine(5);
    BSP_LCD_DisplayStringAtLine(5, (uint8_t *) LcdDateStr);
    Refresh = 0;
  }
  
  /* Display GPS latitude */
  if(strcasecmp(GPS_latitude, PrevGPS_latitude) !=0)
  {
    strcpy(PrevGPS_latitude, GPS_latitude );   
    if ( GPS_latitude[0] != ',' ) 
    {
      tmp_str = GPS_latitude;
    }
    else 
    {
      tmp_str= nofix[4];
    }
    
    sprintf((char*)LcdLatStr,"Lat: %s", tmp_str);
    BSP_LCD_ClearStringLine(6);
    BSP_LCD_DisplayStringAtLine(6, (uint8_t *) LcdLatStr);
    RefreshLat = 1;
  }
  else if( RefreshLat == 1)
  {
    if ( PrevGPS_latitude[0] != ',' ) 
    {
      
      tmp_str = PrevGPS_latitude;
      
    }
    else
    {
      tmp_str= nofix[4];
    }
    sprintf((char*)LcdLatStr,"Lat: %s", tmp_str);
    BSP_LCD_ClearStringLine(6);
    BSP_LCD_DisplayStringAtLine(6, (uint8_t *) LcdLatStr);
    RefreshLat = 0;
  }
  
  /* Display GPS longitude */
  if(strcasecmp(GPS_Longitude, PrevGPS_Longitude) !=0)
  {
    strcpy(PrevGPS_Longitude, GPS_Longitude );
    
    if ( GPS_Longitude[0] != ',' ) 
    {
      tmp_str = GPS_Longitude;
    }
    else  
    {
      tmp_str= nofix[4];
    }
    
    sprintf((char*)LcdLongStr,"Long: %s", tmp_str);
    BSP_LCD_ClearStringLine(7);
    BSP_LCD_DisplayStringAtLine(7, (uint8_t *) LcdLongStr);
    RefreshLong = 1;
  }
  else if( RefreshLong == 1)
  {
    if ( PrevGPS_Longitude[0] != ',' ) 
    {
      
      tmp_str = PrevGPS_Longitude;
      
    }
    else
    {
      tmp_str = nofix[4];
    }
    
    sprintf((char*)LcdLongStr,"Long: %s", tmp_str);
    BSP_LCD_ClearStringLine(7);
    BSP_LCD_DisplayStringAtLine(7, (uint8_t *) LcdLongStr);
    RefreshLong = 0;
  }
  
  /* Display GPS altitude */
  if(strcasecmp(GPS_Altitude, PrevGPS_Altitude) !=0)
  {
    strcpy(PrevGPS_Altitude, GPS_Altitude );
    
    if ( GPS_Altitude[0] != '\0' ) 
    {
      tmp_str = GPS_Altitude;
    }
    else  
    {
      tmp_str= nofix[3];
    }
    
    sprintf((char*)LcdAltStr,"Alt: %s M", tmp_str);
    BSP_LCD_ClearStringLine(8);
    BSP_LCD_DisplayStringAtLine(8, (uint8_t *) LcdAltStr);
    RefreshAlt =1;
  }
  else if( RefreshAlt == 1)
  {
    if ( PrevGPS_Altitude[0] != '\0' ) 
    {
      tmp_str = PrevGPS_Altitude;
    }
    else  
    {
      tmp_str= nofix[3];
    }
    sprintf((char*)LcdAltStr,"Alt: %s M", tmp_str);
    BSP_LCD_ClearStringLine(8);
    BSP_LCD_DisplayStringAtLine(8, (uint8_t *) LcdAltStr);
    RefreshAlt = 0;
  }
  
  /* Display Speed */
  if(strcasecmp(GPS_Speed, PrevGPS_Speed) !=0)
  {
    /* save  speed*/
    strcpy(PrevGPS_Speed, GPS_Speed );
    if ( GPS_Speed[0] != '\0' ) 
    {
      
      tmp_str = GPS_Speed;
    }
    else  
    {
      tmp_str= nofix[3];
    }
    
    sprintf((char*)LcdSpeedStr,"Speed: %s kts", tmp_str);
    BSP_LCD_ClearStringLine(9);
    BSP_LCD_DisplayStringAtLine(9, (uint8_t *) LcdSpeedStr);
    RefreshSpeed =1;
  }
  else if( RefreshSpeed == 1)
  {
    if ( PrevGPS_Speed[0] != '\0' ) 
    {
      
      tmp_str = PrevGPS_Speed;
    }
    else  
    {
      tmp_str= nofix[3];
    }
    sprintf((char*)LcdSpeedStr,"Speed: %s kts", tmp_str);
    BSP_LCD_ClearStringLine(9);
    BSP_LCD_DisplayStringAtLine(9, (uint8_t *) LcdSpeedStr);
    RefreshSpeed = 0;
  }
}
#endif

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void ErrorHandler(void)
{ 
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
  * @brief  Configure the MPU attributes as Write Back for PSRAM mapped on FMC
  *         BANK2.
  * @note   The Base Address is 0x64000000 (for Display purposes)
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
     Normal memory, Shareable, write-back (Display purposes) */
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
