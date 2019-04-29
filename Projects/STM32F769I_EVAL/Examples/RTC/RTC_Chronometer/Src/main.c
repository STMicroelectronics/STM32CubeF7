/**
  ******************************************************************************
  * @file    RTC/RTC_Chronometer/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F7xx RTC HAL API to configure
  *          Time and Date.
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

/** @addtogroup RTC_Chronometer
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
    typedef struct {
  uint8_t tab[12];
} Table_TypeDef;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RtcHandle;
RTC_TimeTypeDef sTimeStructureGet,sOldTimeStructureGet ;
__IO uint8_t StartEvent, TamperPressed =0;

uint32_t BackupIndex = 0,  Currentpos =0;
__IO uint32_t SecondFraction = 0;
RTC_DateTypeDef  RTC_DateStructureGet;

__IO uint16_t Linenum = 0;
uint8_t LcdTimeStr[16] = {0};
static uint32_t timestore =0;

TS_StateTypeDef  TS_State = {0};
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void RTC_CalendarConfig(void);
static void RTC_Time_InitDisplay(void);
static void RTC_Tamper_Config(void);
static void BSP_LCD_DisplayCenterAtLine(uint16_t Line, uint8_t *ptr);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t  lcd_status = LCD_OK;
  uint16_t x1, y1 =0;
  
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
  
  /* Configure the system clock to 216 MHz */
  SystemClock_Config();
  
  /* Configure LED1, LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  
  /*##-1- Initialize the LCD #################################################*/
  /* Initialize the LCD */
  lcd_status = BSP_LCD_Init();
  while(lcd_status != LCD_OK);
  
  BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS);
  
  /* Set LCD Foreground Layer  */
  BSP_LCD_SelectLayer(1);
  
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
  /* Clear the LCD */ 
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);  
  
  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"Chronometer Example", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 40, (uint8_t *)"Press START to Start", CENTER_MODE);
  
  BSP_LCD_DrawRect( 20, 220, 120, 55);
  BSP_LCD_DrawRect( 630, 220, 120, 55);
  
  BSP_LCD_DisplayStringAt(40, 240, (uint8_t *)"START", LEFT_MODE);
  BSP_LCD_DisplayStringAt(660, 240, (uint8_t *)"SEL", LEFT_MODE);
  
  /*##-1- Configure the RTC peripheral #######################################*/
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follows:
  - Hour Format    = Format 24
  - Asynch Prediv  = Value according to source clock
  - Synch Prediv   = Value according to source clock
  - OutPut         = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType     = Open Drain */ 
  RtcHandle.Instance = RTC; 
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if (HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }  
  Linenum = 3;

  BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
  
  /* Infinite loop */
  while (1)
  {
    BSP_TS_GetState(&TS_State);
    
    /* Check on the event 'start' */
    if(StartEvent != 0x0)  
    {   
      /* Get the Current sub second and time */
      HAL_RTC_GetTime(&RtcHandle, &sTimeStructureGet, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&RtcHandle,&RTC_DateStructureGet,RTC_FORMAT_BIN);
      
      SecondFraction  = (( sTimeStructureGet.SecondFraction - sTimeStructureGet.SubSeconds) * 1000)/( sTimeStructureGet.SecondFraction + 1);
      
      /* Set the LCD Text Color */
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
      
      /* Display time Format : hh:mm:ss */
      sprintf((char *)LcdTimeStr, "%.2d:%.2d:%.2d.%.3lu", sTimeStructureGet.Hours, sTimeStructureGet.Minutes, sTimeStructureGet.Seconds, SecondFraction);
      BSP_LCD_DisplayCenterAtLine(3, (uint8_t *) LcdTimeStr);
      
      if( TamperPressed == 1)
      {
        BSP_LCD_Clear(LCD_COLOR_WHITE);

        /* Set the LCD Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE); 
        BSP_LCD_DisplayStringAt(20, 40, (uint8_t *)"Press SEL to save trials or START to restart", CENTER_MODE);
        /* draw rectangle */
        BSP_LCD_DrawRect( 20, 220, 120, 55);
        BSP_LCD_DrawRect( 630, 220, 120, 55);
        BSP_LCD_DisplayStringAt(40, 240, (uint8_t *)"START", LEFT_MODE);
        BSP_LCD_DisplayStringAt(660, 240, (uint8_t *)"SEL", LEFT_MODE);
        
        /* Display LCD messages */
        BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"Chronometer Example", CENTER_MODE);
        HAL_RTCEx_BKUPWrite(&RtcHandle,(uint32_t) (RTC_BKP_DR0 + BackupIndex) , timestore);
        BackupIndex++;
        HAL_RTCEx_BKUPWrite(&RtcHandle, (uint32_t) (RTC_BKP_DR0 + BackupIndex) , SecondFraction);
        BackupIndex++;
        Currentpos = Currentpos +2;
        
        /* Set the LCD Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
        /* Display time Format : hh:mm:ss */
        sprintf((char *)LcdTimeStr, "%.2d:%.2d:%.2d.%.3lu", sOldTimeStructureGet.Hours, sOldTimeStructureGet.Minutes, sOldTimeStructureGet.Seconds, SecondFraction);
        BSP_LCD_DisplayCenterAtLine(Linenum, (uint8_t *) LcdTimeStr);
        
        TamperPressed =0;
      }
      
    }
    
    else
    {
      /* Set the LCD Text Color */
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
      /* Re-initialize the Display time on the LCD */
      RTC_Time_InitDisplay();
    }
    
    if(TS_State.touchDetected)
    {
      x1 = TS_State.touchX[0];
      y1 = TS_State.touchY[0];
      if (((x1>600) && (x1 < 800))&&((y1>220) && (y1 < 280))&& (StartEvent !=0x0)) 
      {
        if(Linenum == 19)
        {
          HAL_RTC_GetTime(&RtcHandle, &sOldTimeStructureGet, RTC_FORMAT_BIN);
          HAL_RTC_GetDate(&RtcHandle,&RTC_DateStructureGet,RTC_FORMAT_BCD);
          
          /* Get the Current sub second and time */
          SecondFraction  = (( sOldTimeStructureGet.SecondFraction - sOldTimeStructureGet.SubSeconds) * 1000)/( sOldTimeStructureGet.SecondFraction + 1);
          timestore = sOldTimeStructureGet.Hours<<16|sOldTimeStructureGet.Minutes<<8|sOldTimeStructureGet.Seconds;
          
          RTC_Tamper_Config();
          BSP_LCD_SetTextColor(LCD_COLOR_MAGENTA); 
          BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"BKP reg full, Press Tamper to erase it and to ", CENTER_MODE);
          BSP_LCD_DisplayStringAt(0, 40, (uint8_t *)"save the last trials or Press START to Restart", CENTER_MODE);              
        }
        else
        {
          Linenum++;
          HAL_RTC_GetTime(&RtcHandle, &sTimeStructureGet, RTC_FORMAT_BIN);
          HAL_RTC_GetDate(&RtcHandle,&RTC_DateStructureGet,RTC_FORMAT_BCD);
          
          /* Get the Current sub second and time */
          SecondFraction  = (( sTimeStructureGet.SecondFraction - sTimeStructureGet.SubSeconds) * 1000)/( sTimeStructureGet.SecondFraction + 1);
          timestore = ((sTimeStructureGet.Hours&0xFF)<<16)|((sTimeStructureGet.Minutes)&0xFF<<8)|((sTimeStructureGet.Seconds)&0xFF);
          HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR0 + BackupIndex , timestore);
          BackupIndex++;
          HAL_RTCEx_BKUPWrite(&RtcHandle,( RTC_BKP_DR0 + BackupIndex) , SecondFraction);
          BackupIndex++;
          Currentpos = Currentpos + 2;
          
          /* Display time Format : hh:mm:ss */
          sprintf((char *)LcdTimeStr, "%.2d:%.2d:%.2d.%.3lu", sTimeStructureGet.Hours, sTimeStructureGet.Minutes, sTimeStructureGet.Seconds, SecondFraction);
          BSP_LCD_DisplayCenterAtLine(Linenum, (uint8_t *) LcdTimeStr);
          HAL_Delay(100);
        }
      }
      else if ((x1 < 200)&&((y1>220) && (y1 < 280)))
      {
        HAL_Delay(100);
        if(StartEvent == 0x1)
        {
          /* Enetr to idle */
          BackupIndex = 0 ;
          Currentpos = 0;
          /* Deactivate the tamper */
          HAL_RTCEx_DeactivateTamper(&RtcHandle, RTC_TAMPER_1); 
          Linenum = 3;
          BSP_LCD_Clear(LCD_COLOR_WHITE);
          /* Set the LCD Text Color */
          BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE); 
          /* Display LCD messages */
          BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"Chronometer Example", CENTER_MODE);
          RTC_Time_InitDisplay();
          StartEvent =0;
          /* Set the LCD Text Color */
          BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE); 
          BSP_LCD_DisplayStringAt(20, 40, (uint8_t *)"      Press START to Start       ", CENTER_MODE);
          BSP_LCD_DrawRect( 20, 220, 120, 55);
          BSP_LCD_DrawRect( 630, 220, 120, 55);
          BSP_LCD_DisplayStringAt(40, 240, (uint8_t *)"START", LEFT_MODE);
          BSP_LCD_DisplayStringAt(660, 240, (uint8_t *)"SEL", LEFT_MODE);
          
          /* Turn on LED1 */
          BSP_LED_Off(LED1);
        }
        else
        {
          /* Turn on LED1 */
          BSP_LED_On(LED1);
          TamperPressed =0;
          /* start count */
          StartEvent = 0x1;   
          
          /* Configure RTC Calendar */
          RTC_CalendarConfig();
          
          /* Set the LCD Text Color */
          BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE); 
          BSP_LCD_DisplayStringAt(20, 40, (uint8_t *)"Press SEL to save trials or START to restart", CENTER_MODE);
          BSP_LCD_DrawRect( 20, 220, 120, 55);
          BSP_LCD_DrawRect( 630, 220, 120, 55);
          BSP_LCD_DisplayStringAt(40, 240, (uint8_t *)"START", LEFT_MODE);
          BSP_LCD_DisplayStringAt(660, 240, (uint8_t *)"SEL", LEFT_MODE);
        }
      }
    }
  }
}

/**
  * @brief  Displays a maximum of 60 characters on the LCD.
  * @param  Line: Line where to display the character shape
  * @param  ptr: Pointer to string to display on LCD
  */
void BSP_LCD_DisplayCenterAtLine(uint16_t Line, uint8_t *ptr)
{
  BSP_LCD_DisplayStringAt(0, LINE(Line), ptr, CENTER_MODE);
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

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while (1)
  {
  }
}

/**
  * @brief  Tamper 1 callback 
  * @param  hrtc : hrtc handle
  * @retval None
*/
void  HAL_RTCEx_Tamper1EventCallback(RTC_HandleTypeDef *hrtc)
{
  TamperPressed = 1;
  /* Enetr to idle */
  BackupIndex = 0 ;
  Currentpos = 0;
  Linenum = 4;
  
  /* Deactivate the tamper */
  HAL_RTCEx_DeactivateTamper(&RtcHandle, RTC_TAMPER_1);   
}

/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
static void RTC_CalendarConfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;

  /*##-1- Configure the Date #################################################*/
  /* Set Date: Tuesday February 18th 2016 */
  sdatestructure.Year = 0x16;
  sdatestructure.Month = RTC_MONTH_FEBRUARY;
  sdatestructure.Date = 0x18;
  sdatestructure.WeekDay = RTC_WEEKDAY_THURSDAY;
  
  if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the Time #################################################*/
  /* Set Time: 00:00:00 */
  stimestructure.Hours = 0x00;
  stimestructure.Minutes = 0x00;
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

  if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  Initialize the time displays  on the LCD.
  * @param  None.
  * @retval None
  */
static void RTC_Time_InitDisplay(void)
{
  /* Display time Format : hh:mm:ss */
  sprintf((char *)LcdTimeStr, "%.2d:%.2d:%.2d.%.3d", 0, 0, 0, 0);
  BSP_LCD_DisplayCenterAtLine(3, (uint8_t *) LcdTimeStr);
}

/**
* @brief RTC Tamper Configuration.
* @param  None.
* @retval None
*/
static void RTC_Tamper_Config(void)
{   
  RTC_TamperTypeDef TamperStructure;
  TamperStructure.Filter = RTC_TAMPERFILTER_DISABLE;
  TamperStructure.Tamper = RTC_TAMPER_1;
  TamperStructure.Trigger = RTC_TAMPERTRIGGER_FALLINGEDGE;
  TamperStructure.NoErase = RTC_TAMPER_ERASE_BACKUP_ENABLE; 
  TamperStructure.MaskFlag = RTC_TAMPERMASK_FLAG_DISABLE; 
  TamperStructure.Interrupt = RTC_TAMPER1_INTERRUPT;
  TamperStructure.SamplingFrequency = RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV32768;
  TamperStructure.PrechargeDuration = RTC_TAMPERPRECHARGEDURATION_1RTCCLK;
  TamperStructure.TamperPullUp = RTC_TAMPER_PULLUP_DISABLE;
  TamperStructure.TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_DISABLE;
  
  HAL_RTCEx_SetTamper_IT(&RtcHandle, &TamperStructure); 
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
