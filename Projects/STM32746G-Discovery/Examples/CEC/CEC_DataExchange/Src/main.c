/**
  ******************************************************************************
  * @file    CEC/CEC_DataExchange/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use the CEC through 
  *          the STM32F7xx HAL API.
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

/** @addtogroup CEC_DataExchange
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t  Tab_Rx[CEC_MAX_PAYLOAD];   /* Received data buffer. Max size = 16 bytes
                                     * header + opcode followed by up to 14 operands */
uint8_t  Tab_Tx[CEC_MAX_PAYLOAD-1]; /* Transmitted data buffer. 
                                     * header is not included in Tab_Tx.
                                     *  Max size = 15 bytes.                                   
                                     *  one opcode followed by up to 14 operands.
                                     *  When payload size = 0, only the header is sent
                                     *  (ping operation) */                                   
uint8_t ReceivedFrame       = 0x0;  /* Set when a reception occurs */
uint16_t NbOfReceivedBytes  = 0x0;  /* Number of received bytes in addition to the header. 
                                     * when a ping message has been received (header 
                                     * only), NbOfReceivedBytes = 0 */
uint8_t StartSending        = 0x0;  /* Set when a transmission is triggered by the user */
uint32_t TxSize             = 0x0;  /* Number of bytes to transmit in addition to the header. 
                                     * In case of ping operation (only the header sent),
                                     * TxSize = 0 */
uint8_t DestinationAddress = 0;     /* Destination logical address */  
uint8_t InitiatorAddress = 0;       /* Initiator logical address */  
uint8_t TxStatus = 0;                            
CEC_HandleTypeDef hcec;             /* CEC IP handle */ 

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void CEC_Config(CEC_HandleTypeDef *hcec);
static void CEC_FlushRxBuffer(void);
static void CEC_SetHint(void);
static void CPU_CACHE_Enable(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
 /* This sample code shows how to use STM32F7xx CEC HAL API to transmit and 
  * receive data. The device is set in waiting to receive mode and sends
  * messages when the evaluation board buttons are pushed by the user */

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  
  /* STM32F7xx HAL library initialization:
       - Configure the Flash ART accelerator
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 200 MHz MHz */
  SystemClock_Config();


  /*##-1- Initialize the SDRAM  ##############################################*/
  BSP_SDRAM_Init();

  /*##-2- Initialize the LCD #################################################*/
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER);

  /* Enable the LCD */
  BSP_LCD_DisplayOn();

  /* Select the LCD Foreground layer */
  BSP_LCD_SelectLayer(1);

  /* Display test description on screen */
  CEC_SetHint();

  /* -2- Configure touch screen */
  BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
  BSP_TS_ITConfig();  /* Touch screen interrupt configuration and enable */
  
  /* -3- CEC configuration (transfer will take place in Interrupt mode) */
#if defined (DEVICE_1)
  DestinationAddress = 0x3; /* follower address */
  InitiatorAddress = 0x1;
#elif defined (DEVICE_2)
  DestinationAddress = 0x1; /* follower address */
  InitiatorAddress = 0x3;
#endif  
  hcec.Instance = CEC;
  /* Deinitialize CEC to reinitialize from scratch */
  HAL_CEC_DeInit(&hcec);
  /* IP configuration */ 
  CEC_Config(&hcec);
  

  /* -4- CEC transfer general variables initialization */
  ReceivedFrame = 0;
  StartSending = 0;
  NbOfReceivedBytes = 0;
  CEC_FlushRxBuffer();
  
 while (1)
  { 
    /* if no reception has occurred and no error has been detected,
     * transmit a message if the user has pushed a button */
     while( (StartSending == 1) && (ReceivedFrame == 0))
    { 
      HAL_CEC_Transmit_IT(&hcec,InitiatorAddress ,DestinationAddress, (uint8_t *)&Tab_Tx, TxSize);
      /* loop until TX ends or TX error reported */
        while (TxStatus != 1);
      StartSending = 0;
    }  
    
    /* if a frame has been received */
    if (ReceivedFrame == 1)
    { 
      if (Tab_Rx[1] == 0x44) /* Test on the opcode value */
      {
        /* Receive command is equal to Command 1 */
        if (Tab_Rx[2] == 0x41) /* Test on the operand value */
        {       
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
          BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
          BSP_LCD_DisplayStringAt(0, (BSP_LCD_GetYSize()/2)+30, (uint8_t *)"  Received opcode 44, operand 41  ", CENTER_MODE);
        }
        else if (Tab_Rx[2] == 0x42) /* Receive command is equal to Command 2 */
        {
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
          BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
          BSP_LCD_DisplayStringAt(0, (BSP_LCD_GetYSize()/2)+30, (uint8_t *)"  Received opcode 44, operand 42  ", CENTER_MODE);
        }
      }
      else if (Tab_Rx[1] == 0x46) /* Test on the opcode value */
      {
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_SetBackColor(LCD_COLOR_ORANGE);
        BSP_LCD_DisplayStringAt(0, (BSP_LCD_GetYSize()/2)+30, (uint8_t *)"       Received opcode 46         ", CENTER_MODE);
      }
      else if (Tab_Rx[1] == 0x9F) /* Test on the opcode value */
      {
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_SetBackColor(LCD_COLOR_DARKMAGENTA);
        BSP_LCD_DisplayStringAt(0, (BSP_LCD_GetYSize()/2)+30, (uint8_t *)"       Received opcode 9F         ", CENTER_MODE);
      }
      ReceivedFrame = 0;
    }
    else if (ReceivedFrame == 2) /* means CEC error detected */
    { 
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_SetBackColor(LCD_COLOR_RED);
      BSP_LCD_DisplayStringAt(0, (BSP_LCD_GetYSize()/2)+45, (uint8_t *)"            CEC Error             ", CENTER_MODE);
      ReceivedFrame = 0;
    }
  }  
}

/**
  * @brief  Configures the CEC peripheral.
  * @param  None
  * @retval None
  */
static void CEC_Config(CEC_HandleTypeDef *hcec)
{
  /* CEC configuration parameters */    
#if defined (DEVICE_1)
  hcec->Init.OwnAddress          = CEC_OWN_ADDRESS_1;
#elif defined (DEVICE_2)
  hcec->Init.OwnAddress          = CEC_OWN_ADDRESS_3;
#endif  


  hcec->Init.SignalFreeTime            = CEC_DEFAULT_SFT;
  hcec->Init.Tolerance                 = CEC_STANDARD_TOLERANCE;
  hcec->Init.BRERxStop                 = CEC_NO_RX_STOP_ON_BRE;
  hcec->Init.BREErrorBitGen            = CEC_BRE_ERRORBIT_NO_GENERATION;
  hcec->Init.LBPEErrorBitGen           = CEC_LBPE_ERRORBIT_NO_GENERATION;
  hcec->Init.BroadcastMsgNoErrorBitGen = CEC_BROADCASTERROR_NO_ERRORBIT_GENERATION;
  hcec->Init.SignalFreeTimeOption      = CEC_SFT_START_ON_TXSOM;
  hcec->Init.ListenMode                = CEC_FULL_LISTENING_MODE;
  hcec->Init.RxBuffer                  = Tab_Rx;
  
  HAL_CEC_Init(hcec);
}

/**
  * @brief  Display CEC test Hint
  * @param  None
  * @retval None
  */
static void CEC_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set Touchscreen Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 110);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"HDMI CEC", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please use the Touchscreen to send CEC commands", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"Touch top :    send opcode 44, operand 41", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"Touch bottom : send opcode 44, operand 42", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 75, (uint8_t *)"Touch left :   send opcode 46            ", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 90, (uint8_t *)"Touch right :  send opcode 9F            ", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 120, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 130);
  BSP_LCD_DrawRect(11, 121, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 132);
}

/**
  * @brief Tx Transfer completed callback
  * @param hcec: CEC handle
  * @retval None
  */
void HAL_CEC_TxCpltCallback(CEC_HandleTypeDef *hcec)
{
  /* End of transmission */
  TxStatus =1;
}


/**
  * @brief Rx Transfer completed callback
  * @param hcec: CEC handle
  * @retval None
  */
void HAL_CEC_RxCpltCallback(CEC_HandleTypeDef *hcec, uint32_t RxFrameSize)
{
    ReceivedFrame = 1;
}

/**
  * @brief CEC error callbacks
  * @param hcec: CEC handle
  * @retval None
  */
void HAL_CEC_ErrorCallback(CEC_HandleTypeDef *hcec)
{
  ReceivedFrame = 2;
}




/**
  * @brief  Reset CEC reception buffer
  * @param  None
  * @retval None
  */
static void CEC_FlushRxBuffer(void)
{
  uint32_t cpt;
  
  for (cpt = CEC_MAX_PAYLOAD; cpt > 0; cpt--)
  {
    Tab_Rx[cpt-1] = 0;
  }
}


/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  TS_StateTypeDef  TS_State;
  
  if(GPIO_Pin == TS_INT_PIN)
  {
    BSP_TS_GetState(&TS_State);
    if(TS_State.touchDetected)
    {
      if (TS_State.touchY[0] < 30)
      {
        /* Top of the screen touch detected */
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
        BSP_LCD_DisplayStringAt(0, (BSP_LCD_GetYSize()/2)+15, (uint8_t *)"    Send opcode 44, operand 41    ", CENTER_MODE);
        Tab_Tx[0] = 0x44;
        Tab_Tx[1] = 0x41;
        TxSize    = 0x02;
        StartSending = 1;
      }
      else if (TS_State.touchY[0] > (BSP_LCD_GetYSize() - 30))
      {
        /* Bottom of the screen touch detected */
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
        BSP_LCD_DisplayStringAt(0, (BSP_LCD_GetYSize()/2)+15, (uint8_t *)"    Send opcode 44, operand 42    ", CENTER_MODE);
        Tab_Tx[0] = 0x44;
        Tab_Tx[1] = 0x42;
        TxSize    = 0x02;
        StartSending = 1;
      }
      else if (TS_State.touchX[0] < 30)
      {
        /* Left of the screen touch detected */
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_SetBackColor(LCD_COLOR_ORANGE);
        BSP_LCD_DisplayStringAt(0, (BSP_LCD_GetYSize()/2)+15, (uint8_t *)"          Send opcode 46          ", CENTER_MODE);
        Tab_Tx[0] = 0x46;
        TxSize    = 0x01;
        StartSending = 1;
      }
      else if (TS_State.touchX[0] > (BSP_LCD_GetXSize() - 30))
      {
        /* Right of the screen touch detected */
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_SetBackColor(LCD_COLOR_DARKMAGENTA);
        BSP_LCD_DisplayStringAt(0, (BSP_LCD_GetYSize()/2)+15, (uint8_t *)"          Send opcode 9F          ", CENTER_MODE);
        Tab_Tx[0] = 0x9F;
        TxSize    = 0x01;
        StartSending = 1;
      }
      /* Wait for touch screen no touch detected */
      do
      {
        BSP_TS_GetState(&TS_State);
      }while(TS_State.touchDetected > 0);
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
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  
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
