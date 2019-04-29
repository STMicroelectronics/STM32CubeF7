/**
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_EnterStandbyMode/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to enter and exit the standby mode with
  *          a wakeup pin or external reset through the STM32F7xx PWR LL API.
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

/** @addtogroup STM32F7xx_LL_Examples
  * @{
  */

/** @addtogroup PWR_EnterStandbyMode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUTTON_MODE_GPIO  0
#define BUTTON_MODE_EXTI  1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t LedSpeed = LED_BLINK_FAST;

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_PWR(void);
void     LED_Init(void);
void     LED_Blinking(uint32_t Period);
void     UserButton_Init(uint32_t Button_Mode);
uint32_t UserButton_GetState(void);
void     EnterStandbyMode(void);
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
  /* Configure the system clock to 216 MHz */
  SystemClock_Config();

  /* Initialize LED1 */
  LED_Init();

  /* Initialize User push-button in GPIO mode */
  UserButton_Init(BUTTON_MODE_GPIO);

  /* Configure Power IP */
  Configure_PWR();

  /* Initialize User push-button in EXTI mode */
  UserButton_Init(BUTTON_MODE_EXTI);
  
  /* Led blinking in RUN mode */
  LED_Blinking(LedSpeed);  
    
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Initialize LED1.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED1 Clock */
  LED1_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED1 */
  LL_GPIO_SetPinMode(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);  
    LL_mDelay(Period);
  }
}

/**
  * @brief  Configures User push-button in GPIO or EXTI Line Mode.
  * @param  ButtonMode: Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability  
  * @retval None
  */
void UserButton_Init(uint32_t Button_Mode)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();
  
  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);

  if(Button_Mode == BUTTON_MODE_EXTI)
  {
    /* Connect External Line to the GPIO*/
    USER_BUTTON_SYSCFG_SET_EXTI();
    
    /* Enable a rising trigger EXTI line 13 Interrupt */
    USER_BUTTON_EXTI_LINE_ENABLE();
    USER_BUTTON_EXTI_RISING_TRIG_ENABLE();
    
    /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
    NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
    NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);  
  }
}

/**
  * @brief  Returns the selected Button state.
  * @param  None
  * @retval The Button GPIO pin value.
  */
uint32_t UserButton_GetState(void)
{
  return LL_GPIO_IsInputPinSet(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN);
}


/**
  * @brief  Function to configure and initialize PWR IP.
  * @param  None
  * @retval None
  */
void Configure_PWR(void)
{
  /* Enable Power Clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  
  /* Check if the system was resumed from StandBy mode */
  if (LL_PWR_IsActiveFlag_SB() != 0)
  { 
    /* Clear Standby flag */
    LL_PWR_ClearFlag_SB(); 
    
    /* Change LED speed to SLOW to indicate exit from standby mode */
    LedSpeed = LED_BLINK_SLOW;
    
    /* Wait that user release the User push-button */
    while(UserButton_GetState() == 1){}
  }

  /* Check and Clear the Wakeup flag */
  if (LL_PWR_IsActiveFlag_WU4() != 0)
  {
    LL_PWR_ClearFlag_WU4();
  }
}

/**
  * @brief  Function to configure and enter in STANDBY Mode.
  * @param  None
  * @retval None
  */
void EnterStandbyMode(void)
{
  /* Wait that user release the User push-button */
  while(UserButton_GetState() == 1){}
  
  /* Disable all used wakeup sources */
  LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN4);
  
  /* Clear all wake up Flag */
  LL_PWR_ClearFlag_WU1();
  LL_PWR_ClearFlag_WU2();
  LL_PWR_ClearFlag_WU3();
  LL_PWR_ClearFlag_WU4();
  LL_PWR_ClearFlag_WU5();
  LL_PWR_ClearFlag_WU6();
	
  /* Enable wakeup pin */
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN4);
  
  /** Request to enter STANDBY mode
    * Following procedure describe in STM32F7xx Reference Manual
    * See PWR part, section Low-power modes, Standby mode
    */
  /* Set STANDBY mode when CPU enters deepsleep */
  LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
  
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  LL_LPM_EnableDeepSleep();
  
  /* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM)
  __force_stores();
#endif
  /* Request Wait For Interrupt */
  __WFI();
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
  *            HSI Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Enable HSE clock */
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_7);

  /* Enable PWR clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* Activation OverDrive Mode */
  LL_PWR_EnableOverDriveMode();
  while(LL_PWR_IsActiveFlag_OD() != 1)
  {
  };

  /* Activation OverDrive Switching */
  LL_PWR_EnableOverDriveSwitching();
  while(LL_PWR_IsActiveFlag_ODSW() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 432, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);

  /* Set systick to 1ms */
  SysTick_Config(216000000 / 1000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 216000000;
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage BUTTON IRQ Handler
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Configure and enter in STANDBY Mode */
  EnterStandbyMode();
  
  /* Here Device is in STANDBY mode */
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
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
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
