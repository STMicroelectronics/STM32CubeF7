/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "memory.h"
#include "menu_selector.h"
/** @addtogroup STM32F7xx_HAL_Applications
  * @{
  */

/** @addtogroup ExtMem_Boot
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunction)(void);
/* Private define ------------------------------------------------------------*/
#define STEMWIN_DEMO_ADDRESS   (uint32_t)(0x90000000)
#define TOUCHGFX_DEMO_ADDRESS  (uint32_t)(0x90780000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction JumpToApplication;

uint32_t Demo_StartAddress=STEMWIN_DEMO_ADDRESS;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
static void CPU_CACHE_Disable(void);

static uint32_t Memory_Startup(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*Variable used to check which demo is selected :
  Selected_Demo=1 == > STemWin
  Selected_Demo=2 == > TouchGfx   
  */
  uint8_t Selected_Demo=0;


  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32F7xx HAL library initialization:
       - Configure the Flash ART accelerator on ITCM interface
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 200 MHz */
  /* Configure the Systick to generate an interrupt each 1 msec */
  SystemClock_Config();

  
  /*##-1-  Menu Selector Launch #########################################*/
  Selected_Demo= Run_MenuSelector();
  
  if (Selected_Demo == STEMWIN_DEMO_SELECTED)
  {
     /*Init Application Address or other Specific inti related to STemWin*/   
   Demo_StartAddress=STEMWIN_DEMO_ADDRESS;
  }
  else if (Selected_Demo ==TOUCHGFX_DEMO_SELECTED)
  {
    /*Init Application Address or other Specific inti related to TouchGfx*/   
    Demo_StartAddress=TOUCHGFX_DEMO_ADDRESS;
  }
  else 
      Error_Handler();

  
  
  /* Reset all used resourcee */ 
  Reset_MenuSelector_Ressource();
  
  /*##-2- Configure Required Memory #########################################*/
  if(Memory_Startup() != MEMORY_OK)
  {
    Error_Handler();
  }

  
   /*##-3-  Jump to application #########################################*/
  /* Disable CPU L1 cache before jumping to the QSPI code execution */
  CPU_CACHE_Disable();
  /* Disable Systick interrupt */
  SysTick->CTRL = 0;

  /* Initialize user application's Stack Pointer & Jump to user application */
  JumpToApplication = (pFunction) (*(__IO uint32_t*) (Demo_StartAddress + 4));
  __set_MSP(*(__IO uint32_t*) Demo_StartAddress);
  JumpToApplication();

  /* We should never get here as execution is now on user application */
  while(1)
  {
  }
}

/**
  * @brief  Initialize and configure Volatile and Non-Volatile memory.
  * @param  None
  * @retval None
  */
uint32_t Memory_Startup(void)
{
  uint32_t status = HAL_OK;
  uint32_t count=0;
  /* Volatile Memory Configuration */
  status |= SDRAM_Startup();
  
  if (Demo_StartAddress == TOUCHGFX_DEMO_ADDRESS)
  {
    /*Reset TouchGFX fram Buffer in SDRAM to avoid Demo issue due to Bad SDRAM initial buffer which causes a picture glitch for 1second*/
    for (count=0;(count<(0x3FC00/8));count++)
    {
      *(uint64_t*)(0xC0000000+8*count) = 0;
    }
  }
  
  return status;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1)
  {
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
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 6
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
#if (DATA_AREA == USE_EXTERNAL_SDRAM) || (CODE_AREA == USE_EXTERNAL_SDRAM)
  RCC_OscInitStruct.PLL.PLLN = 400;
#else
  RCC_OscInitStruct.PLL.PLLN = 432;
#endif
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Activate the OverDrive to reach the 216 MHz Frequency */  
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
  * @brief  CPU L1-Cache disable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Disable(void)
{
  /* Disable I-Cache */
  SCB_DisableICache();

  /* Disable D-Cache */
  SCB_DisableDCache();
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

