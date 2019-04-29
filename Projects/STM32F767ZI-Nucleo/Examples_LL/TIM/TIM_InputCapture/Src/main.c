/**
  ******************************************************************************
  * @file    Examples_LL/TIM/TIM_InputCapture/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use a timer instance in input 
  *          capture mode using the STM32F7xx TIM LL API.
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

/** @addtogroup TIM_InputCapture
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Number of frequencies */
#define TIM_FREQUENCIES_NB 10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Frequency table */
static uint32_t aFrequency[TIM_FREQUENCIES_NB] = {
  4000,   /*  4 kHz */
  6000,   /*  6 kHz */
  8000,   /*  8 kHz */
  10000,  /* 10 kHz */
  12000,  /* 12 kHz */
  14000,  /* 14 kHz */
  16000,  /* 16 kHz */
  18000,  /* 18 kHz */
  20000,  /* 20 kHz */
  22000,  /* 22 kHz */
};

/* Frequency index */
static uint8_t iFrequency = 0;

/* Measured frequency */
__IO uint32_t uwMeasuredFrequency = 0;

/* TIM2 Clock */
static uint32_t TimOutClock = 1;

/* Private function prototypes -----------------------------------------------*/
__STATIC_INLINE void     SystemClock_Config(void);
__STATIC_INLINE void     Configure_TIMPWMOutput(void);
__STATIC_INLINE void     Configure_TIMInputCapture(void);
__STATIC_INLINE void     Configure_Frequency(uint32_t Frequency);
__STATIC_INLINE void     LED_Init(void);
__STATIC_INLINE void     LED_Blinking(uint32_t Period);
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
  
  /* Configure TIM1 in input capture mode */
  Configure_TIMInputCapture();

  /* Configure TIM2 in PWM output mode */
  Configure_TIMPWMOutput();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function enables the peripheral clock on TIM1, configures
  *         TIM1_CH1 as input and enables the capture/compare 1 interrupt
  *         It enables also the peripheral clock for GPIOE and configures 
  *         PE.09 as alternate function for TIM1_CH1.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void Configure_TIMInputCapture(void)
{
  /*************************/
  /* GPIO AF configuration */
  /*************************/
  /* Enable the peripheral clock of GPIOs */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);

  /* GPIO TIM1_CH1 configuration */
  LL_GPIO_SetPinMode(GPIOE, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOE, LL_GPIO_PIN_9, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOE, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_8_15(GPIOE, LL_GPIO_PIN_9, LL_GPIO_AF_1);

  /***************************************************************/
  /* Configure the NVIC to handle TIM1 capture/compare interrupt */
  /***************************************************************/
  NVIC_SetPriority(TIM1_CC_IRQn, 0);
  NVIC_EnableIRQ(TIM1_CC_IRQn);
  
  /******************************/
  /* Peripheral clocks enabling */
  /******************************/
  /* Enable the timer peripheral clock */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
  
  /************************************/
  /* Input capture mode configuration */
  /************************************/
  /* Select the active input: IC1 = TI1FP1 */
  LL_TIM_IC_SetActiveInput(TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
  
  /* Configure the input filter duration: no filter needed */
  LL_TIM_IC_SetFilter(TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1);

  /* Set input prescaler: prescaler is disabled */
  LL_TIM_IC_SetPrescaler(TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);

  /* Select the edge of the active transition on the TI1 channel: rising edge */
  LL_TIM_IC_SetPolarity(TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
  
  /**************************/
  /* TIM1 interrupts set-up */
  /**************************/
  /* Enable the capture/compare interrupt for channel 1 */
  LL_TIM_EnableIT_CC1(TIM1);
  
  /***********************/
  /* Start input capture */
  /***********************/
  /* Enable output channel 1 */
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
    
  /* Enable counter */
  LL_TIM_EnableCounter(TIM1);
}

/**
  * @brief  This function enables the peripheral clock on TIM2 and configures
  *         TIM2_CHTIMB_CHX as PWM output.
  *         It enables also the peripheral clock for GPIOA and configures 
  *         PE.09 as alternate function for TIM2_CHTIMB_CHX.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void  Configure_TIMPWMOutput(void)
{
  /*************************/
  /* GPIO AF configuration */
  /*************************/
  /* Enable the peripheral clock of GPIOs */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

  /* GPIO TIM2_CH1 configuration */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_0, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_0, LL_GPIO_AF_1);

  /******************************/
  /* Peripheral clocks enabling */
  /******************************/
  /* Enable the timer peripheral clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
  
  /***************************/
  /* Time base configuration */
  /***************************/
  /* Set counter mode */
  /* Reset value is LL_TIM_COUNTERMODE_UP */
  //LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);
  
  /* Enable TIM2_ARR register preload. Writing to or reading from the         */
  /* auto-reload register accesses the preload register. The content of the   */
  /* preload register are transferred into the shadow register at each update */
  /* event (UEV).                                                             */  
  LL_TIM_EnableARRPreload(TIM2);
  
  /* Set the auto-reload value to have a counter frequency of 2 kHz           */
  /* TIM2CLK = SystemCoreClock / (APB prescaler & multiplier)                 */
  TimOutClock = SystemCoreClock/2;
  /* TIM2 counter frequency = TimOutClock / (ARR + 1)                   */
  LL_TIM_SetAutoReload(TIM2, __LL_TIM_CALC_ARR(TimOutClock, LL_TIM_GetPrescaler(TIM2), aFrequency[0]));
  
  /*********************************/
  /* Output waveform configuration */
  /*********************************/
  /* Set output mode: PWM mode 1 */ 
  LL_TIM_OC_SetMode(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);

  /* Set compare value to half of the counter period (50% duty cycle )*/
  LL_TIM_OC_SetCompareCH1(TIM2, (LL_TIM_GetAutoReload(TIM2) / 2));
  
  /* Enable TIM2_CCR1 register preload. Read/Write operations access the      */
  /* preload register. TIM2_CCR1 preload value is loaded in the active        */
  /* at each update event.                                                    */
  LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH1);
  
  /**********************************/
  /* Start output signal generation */
  /**********************************/
  /* Enable output channel 1 */
  LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
    
  /* Enable counter */
  LL_TIM_EnableCounter(TIM2);
  
  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM2);
}

/**
  * @brief  Changes the frequency of the PWM signal.
  * @note this function is executed within the CC1 interrupt service
  *       routine context.
  * @param  Requested frequency
  * @retval None
  */
__STATIC_INLINE void Configure_Frequency(uint32_t Frequency)
{
  /* Set the auto-reload value to have the requested frequency */
  /* Frequency = TIM2CLK / (ARR + 1)                   */
  LL_TIM_SetAutoReload(TIM2, __LL_TIM_CALC_ARR(TimOutClock, LL_TIM_GetPrescaler(TIM2), Frequency));
 
  /* Set compare value to half of the counter period (50% duty cycle )*/
  LL_TIM_OC_SetCompareCH1(TIM2, (LL_TIM_GetAutoReload(TIM2) / 2));
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
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
__STATIC_INLINE void LED_Blinking(uint32_t Period)
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
  * @note   When the user key button is pressed the frequency of the  
  *         PWM signal generated by TIM2 is updated. 
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Set new PWM signal frequency */
  iFrequency = (iFrequency + 1) % TIM_FREQUENCIES_NB;

  /* Change PWM signal frequency */
  Configure_Frequency(aFrequency[iFrequency]);
}

/**
  * @brief  Timer capture/compare interrupt processing
  * @note TIM1 input capture module is used to capture the value of the counter
  *       after a transition is detected by the corresponding input channel.
  * @param  None
  * @retval None
  */
void TimerCaptureCompare_Callback(void)
{
  /* Capture index */
  static uint16_t uhCaptureIndex = 0;
  
  /* Captured Values */
  static uint32_t uwICValue1 = 0;
  static uint32_t uwICValue2 = 0;
  static uint32_t uwDiffCapture = 0;
  
  uint32_t TIM1CLK;
  uint32_t PSC;
  uint32_t IC1PSC;
  uint32_t IC1Polarity;
  
  if(uhCaptureIndex == 0)
  {
    /* Get the 1st Input Capture value */
    uwICValue1 = LL_TIM_IC_GetCaptureCH1(TIM1);
    uhCaptureIndex = 1;
  }
  else if(uhCaptureIndex == 1)
  {
    /* Get the 2nd Input Capture value */
    uwICValue2 = LL_TIM_IC_GetCaptureCH1(TIM1); 
    
    /* Capture computation */
    if (uwICValue2 > uwICValue1)
    {
      uwDiffCapture = (uwICValue2 - uwICValue1); 
    }
    else if (uwICValue2 < uwICValue1)
    {
      uwDiffCapture = ((TIM1_ARR_MAX - uwICValue1) + uwICValue2) + 1; 
    }
    else
    {
      /* If capture values are equal, we have reached the limit of frequency  */
      /* measures.                                                            */
      LED_Blinking(LED_BLINK_ERROR);
    }
    
    /* The signal frequency is calculated as follows:                         */      
    /* Frequency = (TIM1*IC1PSC) / (Capture*(PSC+1)*IC1Polarity)           */
    /* where:                                                                 */                                                          
    /*  Capture is the difference between two consecutive captures            */
    /*  TIM1CLK is the timer counter clock frequency                           */
    /*  PSC is the timer prescaler value                                      */
    /*  IC1PSC is the input capture prescaler value                           */
    /*  IC1Polarity value depends on the capture sensitivity:                 */
    /*    1 if the input is sensitive to rising or falling edges              */
    /*    2 if the input is sensitive to both rising and falling edges        */
    
    /* Retrieve actual TIM1 counter clock frequency */
    TIM1CLK = SystemCoreClock;
    
    /* Retrieve actual TIM1 prescaler value */
    PSC = LL_TIM_GetPrescaler(TIM1);
    
    /* Retrieve actual IC1 prescaler ratio */
    IC1PSC = __LL_TIM_GET_ICPSC_RATIO(LL_TIM_IC_GetPrescaler(TIM1, LL_TIM_CHANNEL_CH1));
   
    /* Retrieve actual IC1 polarity setting */
    if (LL_TIM_IC_GetPolarity(TIM1, LL_TIM_CHANNEL_CH1) == LL_TIM_IC_POLARITY_BOTHEDGE)
      IC1Polarity = 2;
    else
      IC1Polarity = 1;
    
    /* Calculate input signal frequency */
    uwMeasuredFrequency = (TIM1CLK *IC1PSC) / (uwDiffCapture*(PSC+1)*IC1Polarity);
    
    /* reset capture index */
    uhCaptureIndex = 0;    
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
