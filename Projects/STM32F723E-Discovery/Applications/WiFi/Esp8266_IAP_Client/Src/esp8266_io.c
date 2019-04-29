/**
  ******************************************************************************
  * @file    WiFi/ESP8266_IAP_Client/Src/esp8266_io.c
  * @author  MCD Application Team
  * @brief   This file implments the IO operations to deal with the esp8266 wifi
  *          module. It mainly Inits and Deinits the UART interface. Send and
  *          receive data over it.
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
#include "esp8266_io.h"

/* Private define ------------------------------------------------------------*/
#define RING_BUFFER_SIZE                         (1024 * 2)

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint8_t  data[RING_BUFFER_SIZE];
  uint16_t tail; 				
  uint16_t head;
}RingBuffer_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RingBuffer_t WiFiRxBuffer;
UART_HandleTypeDef WiFiUartHandle;

/* Private function prototypes -----------------------------------------------*/
static void WIFI_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ESP8266 IO Initalization.
  *         This function inits the UART interface to deal with the esp8266,
  *         then starts asynchronous listening on the RX port.
  * @param None
  * @retval 0 on success, -1 otherwise.
  */
int8_t ESP8266_IO_Init(void)
{
  /* Set the WiFi USART configuration parameters */
  WiFiUartHandle.Instance        = USARTx;
  WiFiUartHandle.Init.BaudRate   = 115200;
  WiFiUartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  WiFiUartHandle.Init.StopBits   = UART_STOPBITS_1;
  WiFiUartHandle.Init.Parity     = UART_PARITY_NONE;
  WiFiUartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  WiFiUartHandle.Init.Mode       = UART_MODE_TX_RX;
  WiFiUartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

  /* Configure the USART IP */
  if(HAL_UART_Init(&WiFiUartHandle) != HAL_OK)
  {
    return -1;
  }
 
  /* Once the WiFi UART is intialized, start an asynchrounous recursive 
   listening. the HAL_UART_Receive_IT() call below will wait until one char is
   received to trigger the HAL_UART_RxCpltCallback(). The latter will recursively
   call the former to read another char.  */
  WiFiRxBuffer.head = 0;
  WiFiRxBuffer.tail = 0;
 
  HAL_UART_Receive_IT(&WiFiUartHandle, (uint8_t *)&WiFiRxBuffer.data[WiFiRxBuffer.tail], 1);

  return 0;
}

/**
  * @brief  ESP8266 IO Deinitialization.
  *         This function Deinits the UART interface of the ESP8266. When called
  *         the esp8266 commands can't be executed anymore.
  * @param  None.
  * @retval None.
  */
void ESP8266_IO_DeInit(void)
{
  /* Reset USART configuration to default */
  HAL_UART_DeInit(&WiFiUartHandle);
}

/**
  * @brief  Send Data to the ESP8266 module over the UART interface.
  *         This function allows sending data to the  ESP8266 WiFi Module, the
  *          data can be either an AT command or raw data to send over 
             a pre-established WiFi connection.
  * @param pData: data to send.
  * @param Length: the data length.
  * @retval 0 on success, -1 otherwise.
  */
int8_t ESP8266_IO_Send(uint8_t* pData, uint32_t Length)
{
  /* Unlike the ESP8266_IO_Receive(), the ESP8266_IO_Send() is using a blocking call
     to ensure that the AT commands were correctly sent. */
  if (HAL_UART_Transmit(&WiFiUartHandle, (uint8_t*)pData, Length, DEFAULT_TIME_OUT) != HAL_OK)
  {
     return -1;
  }
  
  return 0;
}

/**
  * @brief  Receive Data from the ESP8266 module over the UART interface.
  *         This function receives data from the  ESP8266 WiFi module, the
  *         data is fetched from a ring buffer that is asynchonously and continuously
            filled with the received data.
  * @param  Buffer: a buffer inside which the data will be read.
  * @param  Length: the Maximum size of the data to receive.
  * @retval int32_t: the actual data size that has been received.
  */
int32_t ESP8266_IO_Receive(uint8_t* Buffer, uint32_t Length)
{
  uint32_t ReadData = 0;
  
  /* Loop until data received */
  while (Length--)
  {
    uint32_t tickStart = HAL_GetTick();
    do
    {
      if(WiFiRxBuffer.head != WiFiRxBuffer.tail)
      {
        /* serial data available, so return data to user */
        *Buffer++ = WiFiRxBuffer.data[WiFiRxBuffer.head++];
        ReadData++;
		
        /* check for ring buffer wrap */
        if (WiFiRxBuffer.head >= RING_BUFFER_SIZE)
        {
          /* Ring buffer wrap, so reset head pointer to start of buffer */
          WiFiRxBuffer.head = 0;
        }
        break;
      }
    }while((HAL_GetTick() - tickStart ) < DEFAULT_TIME_OUT);
  }
  
  return ReadData;
}

/**
  * @brief  Rx Callback when new data is received on the UART.
  * @param  UartHandle: Uart handle receiving the data.
  * @retval None.
  */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* If ring buffer end is reached reset tail pointer to start of buffer */
  if(++WiFiRxBuffer.tail >= RING_BUFFER_SIZE)
  {
    WiFiRxBuffer.tail = 0;   
  }
  
  HAL_UART_Receive_IT(UartHandle, (uint8_t *)&WiFiRxBuffer.data[WiFiRxBuffer.tail], 1);
}

/**
  * @brief  Function called when error happens on the UART.
  * @param  UartHandle: Uart handle receiving the data.
  * @retval None.
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
   /* Call  the WIFI_Handler() to deinitialize the UART Interface. */
    WIFI_Handler();
}

/**
  * @brief  Handler to deinialize the ESP8266 UART interface in case of errors.
  * @param  None
  * @retval None.
  */
static void WIFI_Handler(void)
{
  HAL_UART_DeInit(&WiFiUartHandle);
  
  while(1)
  {
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
