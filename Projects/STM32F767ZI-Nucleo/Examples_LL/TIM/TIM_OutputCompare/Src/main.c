/**
  ******************************************************************************
  * @file    Examples_LL/TIM/TIM_OutputCompare/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use a timer instance in output 
  *          compare mode using the STM32F7xx TIM LL API.
  *          Peripheral initialization done using LL unitary services functions.
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

/** @addtogroup TIM_OutputCompare
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Number of output compare modes */
#define TIM_OC_MODES_NB 3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Output compare modes */
static uint32_t aOCMode[TIM_OC_MODES_NB] = {
  LL_TIM_OCMODE_TOGGLE,
  LL_TIM_OCMODE_ACTIVE,
  LL_TIM_OCMODE_INACTIVE
};

/* Output compare mode index */
static uint8_t iOCMode = 0;

/* Compare match count */
static uint32_t uwCompareMatchCount = 0;

/* TIM3 Clock */
static uint32_t TimOutClock = 1;

/* Private function prototypes -----------------------------------------------*/
__STATIC_INLINE void     SystemClock_Config(void);
__STATIC_INLINE void     Configure_TIMOutputCompare(void);
__STATIC_INLINE void     Configure_OCMode(uint32_t OCMode);
__STATIC_INLINE void     LED_Init(void);
__STATIC_INLINE void     UserButton_Init(void);
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

  /* Initialize button in EXTI mode */
  UserButton_Init();
  
  /* Configure the timer in output compare mode */
  Configure_TIMOutputCompare();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Configures the timer to generate an output compare signal on the 
  *         OC1 output.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void  Configure_TIMOutputCompare(void)
{
  /*************************/
  /* GPIO AF configuration */
  /*************************/
  /* Enable the peripheral clock of GPIOs */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  /* GPIO TIM3_CH3 configuration */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_0, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_0, LL_GPIO_AF_2);

  /***********************************************/
  /* Configure the NVIC to handle TIM3 interrupt */
  /***********************************************/
  NVIC_SetPriority(TIM3_IRQn, 0);
  NVIC_EnableIRQ(TIM3_IRQn);
  
  /******************************/
  /* Peripheral clocks enabling */
  /******************************/
  /* Enable the timer peripheral clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3); 
  
  /***************************/
  /* Time base configuration */
  /***************************/
  /* Set counter mode */
  /* Reset value is LL_TIM_COUNTERMODE_UP */
  //LL_TIM_SetCounterMode(TIM3, LL_TIM_COUNTERMODE_UP);
  
  /* Set the pre-scaler value to have TIM3 counter clock equal to 10 kHz */
  LL_TIM_SetPrescaler(TIM3, __LL_TIM_CALC_PSC(SystemCoreClock, 10000));
  
  /* Set the auto-reload value to have a counter frequency of 10 Hz */
  /* TIM3CLK = SystemCoreClock / (APB prescaler & multiplier)               */
  TimOutClock = SystemCoreClock/4;
  LL_TIM_SetAutoReload(TIM3, __LL_TIM_CALC_ARR(TimOutClock, LL_TIM_GetPrescaler(TIM3), 10));
    
  /*********************************/
  /* Output waveform configuration */
  /*********************************/
  /* Set output compare mode: TOGGLE */ 
  LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_TOGGLE);

  /* Set output channel polarity: OC is active high */
  LL_TIM_OC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH3, LL_TIM_OCPOLARITY_HIGH);
  
  /* Set output compare active/inactive delay to half of the auto-reload value */
  LL_TIM_OC_SetCompareCH3(TIM3, (LL_TIM_GetAutoReload(TIM2) / 2));
  
  /**************************/
  /* TIM3 interrupts set-up */
  /**************************/
  /* Enable the capture/compare interrupt for channel 1*/
  LL_TIM_EnableIT_CC3(TIM3);
  
  /**********************************/
  /* Start output signal generation */
  /**********************************/
  /* Enable output channel 3 */
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH3);
    
  /* Enable counter */
  LL_TIM_EnableCounter(TIM3);
  
  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM3);
}

/**
  * @brief  Changes the output compare mode.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void Configure_OCMode(uint32_t OCMode)
{
  /* Disable the counter */
  LL_TIM_DisableCounter(TIM3);
  
  /* Reset the counter */
  LL_TIM_SetCounter(TIM3, 0);
  
  /* Reset the compare match count */
  uwCompareMatchCount = 0;
  
  /* Set the output level (active v.s. inactive) according to the new OC mode */
  switch (OCMode)
  {
    case LL_TIM_OCMODE_TOGGLE:
    case LL_TIM_OCMODE_ACTIVE:
      /* Set the output channel to its inactive level (LOW)*/
      LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_FORCED_INACTIVE);
      break;
      
    case LL_TIM_OCMODE_INACTIVE:
      /* Set the output channel to its active level (HIGH)*/
      LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_FORCED_ACTIVE);
      break;
        
    default:
      break;
  }
  
  /* Update the output channel mode */
  LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH3, OCMode);
  
  /* Re-enable the counter */
  LL_TIM_EnableCounter(TIM3);
}

/**
  * @brief  Initialize LED1.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LED_Init(void)
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
  * @brief  Configures User push-button in GPIO or EXTI Line Mode.
  * @param  None  
  * @retval None
  */
__STATIC_INLINE void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();
  
  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);

  /* Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();
    
  /* Enable a rising trigger EXTI line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
    
  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);  
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

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  User button interrupt processing
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Set new OC mode */
  iOCMode = (iOCMode + 1) % TIM_OC_MODES_NB;
    
  /* Switch to next OC mode */
  Configure_OCMode(aOCMode[iOCMode]);
}

/**
  * @brief  Timer capture/compare interrupt processing
  * @note   The capture/compare interrupt is generated whatever the compare
  *         mode is (as long as the timer counter is enabled).
  * @param  None
  * @retval None
  */
void TimerCaptureCompare_Callback(void)
{
   /* Upon compare match, the counter value  should be equal to the */
  /* capture/compare register (TIMx_CCRx) value.                    */
  if(LL_TIM_GetCounter(TIM3) == LL_TIM_OC_GetCompareCH3(TIM3))
  {
    /* Increment the compare match count */
    uwCompareMatchCount++;
  }
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
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
