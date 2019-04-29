/**
  ******************************************************************************
  * @file    Examples_LL/TIM/TIM_BreakAndDeadtime/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to configure the TIMER peripheral to generate  
  *          three center aligned PWM and complementary PWM signals, 
  *          to insert a defined dead time value, to use the break feature and 
  *          to lock the desired parameters, using the STM32F7xx TIM LL API.
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

/** @addtogroup TIM_BreakAndDeadtime
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
__STATIC_INLINE void     SystemClock_Config(void);
__STATIC_INLINE void     ConfigureGPIO(void);
__STATIC_INLINE void     ConfigureTIMBreakAndDeadtime(void);
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

  /* Configure GPIO ports */  
  ConfigureGPIO();
  
  /* Configure timer instance */
  ConfigureTIMBreakAndDeadtime();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function enables the peripheral clocks on required GPIOs and
  *         configures PE.09 (TIM1_CH1), PE.08 (TIM1_CH1N), PE.11 (TIM1_CH2),
  *         PE.10 (TIM1_CH2N), PE.13 (TIM1_CH3), PE.12 (TIM1_CH3N) and
  *         PE.15 (TIM1_BKIN). 
  * @param  None
  * @retval None
  */
__INLINE void ConfigureGPIO(void)
{  
  /******************************/
  /* Peripheral clocks enabling */
  /******************************/
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);

  /*************************/
  /* GPIO AF configuration */
  /*************************/
  
  /* GPIO TIM1_CH1 configuration */
  LL_GPIO_SetPinMode(GPIOE, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOE, LL_GPIO_PIN_9, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOE, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_8_15(GPIOE, LL_GPIO_PIN_9, LL_GPIO_AF_1);

  /* GPIO TIM1_CH1N configuration */
  LL_GPIO_SetPinMode(GPIOE, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOE, LL_GPIO_PIN_8, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOE, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_8_15(GPIOE, LL_GPIO_PIN_8, LL_GPIO_AF_1);

  /* GPIO TIM1_CH2 configuration */
  LL_GPIO_SetPinMode(GPIOE, LL_GPIO_PIN_11, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOE, LL_GPIO_PIN_11, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOE, LL_GPIO_PIN_11, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_8_15(GPIOE, LL_GPIO_PIN_11, LL_GPIO_AF_1);

  /* GPIO TIM1_CH2N configuration */
  LL_GPIO_SetPinMode(GPIOE, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOE, LL_GPIO_PIN_10, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOE, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_8_15(GPIOE, LL_GPIO_PIN_10, LL_GPIO_AF_1);

  /* GPIO TIM1_CH3 configuration */
  LL_GPIO_SetPinMode(GPIOE, LL_GPIO_PIN_13, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOE, LL_GPIO_PIN_13, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOE, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_8_15(GPIOE, LL_GPIO_PIN_13, LL_GPIO_AF_1);

  /* GPIO TIM1_CH3N configuration */
  LL_GPIO_SetPinMode(GPIOE, LL_GPIO_PIN_12, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOE, LL_GPIO_PIN_12, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOE, LL_GPIO_PIN_12, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_8_15(GPIOE, LL_GPIO_PIN_12, LL_GPIO_AF_1);
  
  /* GPIO TIM1_BKIN configuration */
  LL_GPIO_SetPinMode(GPIOE, LL_GPIO_PIN_15, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOE, LL_GPIO_PIN_15, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOE, LL_GPIO_PIN_15, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_8_15(GPIOE, LL_GPIO_PIN_15, LL_GPIO_AF_1);
}

/**
  * @brief  This function enables the peripheral clock on TIM1, 
  *         configures the TIM1 counter in center-aligned mode,
  *         configures the output channels to generate complementary PWM signals
  *         with 4 us dead-time insertion, configures the break function,
  *         sets the lock level and starts output signals generation.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
__STATIC_INLINE void ConfigureTIMBreakAndDeadtime(void)
{
  /*****************************/
  /* Peripheral clock enabling */
  /*****************************/
  /* Enable the peripheral clock of TIM1 */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
  
  /***************************/
  /* Time base configuration */
  /***************************/
  /* Select center-aligned mode */
  LL_TIM_SetCounterMode(TIM1, LL_TIM_COUNTERMODE_CENTER_UP);
  
  /* Set the TIM1 prescaler to get counter clock frequency at 10 MHz */
  /* In this example TIM1 input clock (TIM1CLK) frequency is set to APB2      */
  /* (PCLK2), since APB2 pre-scaler is equal to 1.                            */
  /*  TIM1CLK = PCLK2                                                         */
  /*  PCLK2 = HCLK                                                            */
  /*  => TIM1CLK = HCLK = SystemCoreClock (216 Mhz)                           */
  LL_TIM_SetPrescaler(TIM1, __LL_TIM_CALC_PSC(SystemCoreClock, 10000000));
  
  /* Set the TIM1 auto-reload register to get a PWM frequency at 10 KHz */  
  /* Note that in macro call below, targeted PWM frequency must be multiplied */ 
  /* by 2 when the counter operates in center-aligned mode (due to the       */
  /* symmetry of the pattern).                                                */
  LL_TIM_SetAutoReload(TIM1, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM1), 10000*2));

  /*********************************/
  /* Output waveform configuration */
  /*********************************/
  /* Set output channel 1 in PWM1 mode */
  LL_TIM_OC_SetMode(TIM1,  LL_TIM_CHANNEL_CH1,  LL_TIM_OCMODE_PWM1);
  
  /* Output channel 1 configuration:    */
  LL_TIM_OC_ConfigOutput(TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH | LL_TIM_OCIDLESTATE_HIGH);
  
  /*  Set PWM output channel 1 duty cycle to 50% */
  LL_TIM_OC_SetCompareCH1(TIM1, LL_TIM_GetAutoReload(TIM1) * 500 / 1000 );
  
  /* Set output channel 2 in PWM1 mode */
  LL_TIM_OC_SetMode(TIM1,  LL_TIM_CHANNEL_CH2,  LL_TIM_OCMODE_PWM1);

  /* Output channel 2 configuration:    */
  LL_TIM_OC_ConfigOutput(TIM1, LL_TIM_CHANNEL_CH2, LL_TIM_OCPOLARITY_HIGH | LL_TIM_OCIDLESTATE_HIGH);

  /*  Set PWM output channel 2 duty cycle to 25% */
  LL_TIM_OC_SetCompareCH2(TIM1, LL_TIM_GetAutoReload(TIM1) * 250 / 1000 );

  /* Set output channel 3 in PWM1 mode */
  LL_TIM_OC_SetMode(TIM1,  LL_TIM_CHANNEL_CH3,  LL_TIM_OCMODE_PWM1);

  /* Output channel 3 configuration:    */
  LL_TIM_OC_ConfigOutput(TIM1, LL_TIM_CHANNEL_CH3, LL_TIM_OCPOLARITY_HIGH | LL_TIM_OCIDLESTATE_HIGH);

  /*  Set PWM output channel 3 duty cycle to 12.5% */
  LL_TIM_OC_SetCompareCH3(TIM1, LL_TIM_GetAutoReload(TIM1) * 125 / 1000 );
  
  /*  Enable register preload for every output channels */
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH1);
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH2);
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH3);
  
  /* Set dead time to 4 us (4000 ns) */
  LL_TIM_OC_SetDeadTime(TIM1, __LL_TIM_CALC_DEADTIME(SystemCoreClock, LL_TIM_GetClockDivision(TIM1), 4000));
  
  /********************************/
  /* Break function configuration */
  /********************************/
  /* Break input configuration:        */  
  LL_TIM_ConfigBRK(TIM1, LL_TIM_BREAK_POLARITY_HIGH, LL_TIM_BREAK_FILTER_FDIV1);
  
  /* Outputs are automatically re-enabled when the *break input is no longer  */
  /* active                                                                   */
  LL_TIM_EnableAutomaticOutput(TIM1);
  
  /* Select the outputs off state in Idle and Run modes */
  LL_TIM_SetOffStates(TIM1, LL_TIM_OSSI_ENABLE, LL_TIM_OSSR_ENABLE);

  /* Enable the break input: */  
  LL_TIM_EnableBRK(TIM1);
  
  /**************************/
  /* Lock level programming */
  /**************************/
  /* Set lock level to 1:                                                     */
  /* From this point onward, deadtime, outputs idle state, break input        */
  /* configuration and automatic output enabling can no longer be written     */
  LL_TIM_CC_SetLockLevel(TIM1, LL_TIM_LOCKLEVEL_1);
  
  /**********************************/
  /* Start output signal generation */
  /**********************************/
  /* Enable outputs OC1, OC1N, OC2, OC2N, OC3 and OC3N */
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1  |
                                LL_TIM_CHANNEL_CH1N |
                                LL_TIM_CHANNEL_CH2  |
                                LL_TIM_CHANNEL_CH2N |
                                LL_TIM_CHANNEL_CH3  |
                                LL_TIM_CHANNEL_CH3N);
  
  /* Enable TIM1 outputs */
  LL_TIM_EnableAllOutputs(TIM1);
  
  /* Enable counter */
  LL_TIM_EnableCounter(TIM1);
  
  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM1);
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
