/**
  ******************************************************************************
  * @file    FLASH/FLASH_DualBoot/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example provides a description of how to boot from bank1 or bank2
  *          of the STM32F7xx FLASH.
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

/** @addtogroup FLASH_DualBoot
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1     "STM32F7XX" 
#define MESSAGE2     "Flash Dual-boot example"
#define MESSAGE3     "Device running on" 

#ifdef FLASH_BANK1
 #define MESSAGE4    "FLASH BANK1"
#else
 #define MESSAGE4    "FLASH BANK2"
#endif

#define MESSAGE5     "PUSH BUTTON_TAMPER"
#define MESSAGE6     "to swap bank"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FLASH_OBProgramInitTypeDef    OBInit; 
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Display_ExampleDescription(void);
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

  /* Configure the system clock to 200 MHz */
  SystemClock_Config();

  /* Configure BUTTON_TAMPER */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);

  /* Initialize LED1, LED2 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /* Display messages on LCD */
  Display_ExampleDescription();
  
  /* Turn on LEDs */
  BSP_LED_On(LED1);
  BSP_LED_On(LED2);

  /*--- If Wake-up button is pushed, Modify BootAddr0/BootAddr1 value to enable or disbale 
  boot from Bank2 (active after next reset, regardless of Boot pin value ---*/

  while (1)
  {
    /* Wait for BUTTON_TAMPER is released */
    if (BSP_PB_GetState(BUTTON_TAMPER) == SET)
    {
      while (BSP_PB_GetState(BUTTON_TAMPER) == SET);

      /* Allow Access to Flash control registers and user Flash */
      HAL_FLASH_Unlock();

      /* Allow Access to option bytes sector */ 
      HAL_FLASH_OB_Unlock();
      
      /* Get the Dual boot configuration status */
      HAL_FLASHEx_OBGetConfig(&OBInit);

      /* Turn on LED3 if FLASH is configured in Single Bank mode */
      if((OBInit.USERConfig & OB_NDBANK_SINGLE_BANK) == OB_NDBANK_SINGLE_BANK)
      {
        while(1)
        {
          BSP_LED_Off(LED1);
          BSP_LED_Off(LED2);
          BSP_LED_On(LED3);
        }
      }

      /* Switch Boot bank: the BootAddrx value "0x2040" corresponds to bank2 address: "0x08100000" */
      if (((__HAL_SYSCFG_GET_BOOT_MODE() == SYSCFG_MEM_BOOT_ADD0) && (OBInit.BootAddr0 < 0x2040)) || \
          ((__HAL_SYSCFG_GET_BOOT_MODE() == SYSCFG_MEM_BOOT_ADD1) && (OBInit.BootAddr1 < 0x2040))) 
      {
        /* At startup, if boot is done from bank1, next boot will be executed from bank2 */ 
        OBInit.OptionType = OPTIONBYTE_BOOTADDR_0 | OPTIONBYTE_BOOTADDR_1;
        OBInit.BootAddr0  = __HAL_FLASH_CALC_BOOT_BASE_ADR(0x08100000);
        OBInit.BootAddr1  = __HAL_FLASH_CALC_BOOT_BASE_ADR(0x08100000);
      }
      else
      {
        /* At startup, if boot is done from bank2, next boot will be executed from bank1 */
        OBInit.OptionType = OPTIONBYTE_BOOTADDR_0 | OPTIONBYTE_BOOTADDR_1;
        OBInit.BootAddr0  = __HAL_FLASH_CALC_BOOT_BASE_ADR(0x08000000);
        OBInit.BootAddr1  = __HAL_FLASH_CALC_BOOT_BASE_ADR(0x08000000);
      }
	  
      if(HAL_FLASHEx_OBProgram(&OBInit) != HAL_OK)
      {
        /*
        Error occurred while setting option bytes configuration.
        User can add here some code to deal with this error.
        To know the code error, user can call function 'HAL_FLASH_GetError()'
        */
        /* Infinite loop */
        while (1)
        {
          BSP_LED_On(LED3);
        }
      }
      
      /* Start the Option Bytes programming process */  
      if (HAL_FLASH_OB_Launch() != HAL_OK)
      {
        /*
        Error occurred while reloading option bytes configuration.
        User can add here some code to deal with this error.
        To know the code error, user can call function 'HAL_FLASH_GetError()'
        */
        /* Infinite loop */
        while (1)
        {
          BSP_LED_On(LED3);
        }
      }

      /* Prevent Access to option bytes sector */
      HAL_FLASH_OB_Lock();
    
      /* Disable the Flash option control register access (recommended to protect 
      the option Bytes against possible unwanted operations) */
      HAL_FLASH_Lock();

      /* Initiate a system reset request to reset the MCU */
      HAL_NVIC_SystemReset();
    }
    else
    {
#ifdef FLASH_BANK1        
      /* Toggle LED1 */
      BSP_LED_Toggle(LED1);
#else 
      /* Toggle LED2 */    
      BSP_LED_Toggle(LED2);
#endif    
      
      /* Insert 50 ms delay */
      HAL_Delay(50);
    }
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
  * @brief  Display main example messages
  * @param  None
  * @retval None
  */
static void Display_ExampleDescription(void)
{
  /* Initialize the LCD */
  BSP_LCD_Init();
  
  /* LCD Layer Initialization */
  BSP_LCD_LayerDefaultInit(1, 0xC0130000);
  BSP_LCD_SelectLayer(1);  
  BSP_LCD_DisplayOn();
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);

#ifdef FLASH_BANK1
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
#else
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
#endif /* BOOT_FROM_BANK1 */
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);


  /* Display LCD messages */
#ifdef FLASH_BANK1
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
#else
  BSP_LCD_SetBackColor(LCD_COLOR_RED);
#endif /* BOOT_FROM_BANK1 */
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)MESSAGE1, CENTER_MODE);

  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 40, (uint8_t *)MESSAGE2, CENTER_MODE);

#ifdef FLASH_BANK1
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
#else
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
#endif /* BOOT_FROM_BANK1 */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

  BSP_LCD_DisplayStringAt(0, 95, (uint8_t *)MESSAGE3, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 110, (uint8_t *)MESSAGE4, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 140, (uint8_t *)MESSAGE5, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 155, (uint8_t *)MESSAGE6, CENTER_MODE);
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
