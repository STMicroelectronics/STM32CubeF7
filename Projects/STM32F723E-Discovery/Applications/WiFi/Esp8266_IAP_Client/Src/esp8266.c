/**
  ******************************************************************************
  * @file    WiFi/ESP8266_IAP_Client/Src/esp8266.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the ESP8266
  *          WiFi devices.
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
#include "esp8266.h"
#include "esp8266_io.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t AtCmd[MAX_AT_CMD_SIZE];
uint8_t RxBuffer[MAX_BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
static ESP8266_StatusTypeDef runAtCmd(uint8_t* cmd, uint32_t Length, const uint8_t* Token);
static ESP8266_StatusTypeDef getData(uint8_t* Buffer, uint32_t Length, uint32_t* RetLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Initialize the esp8266 module.
  *          IT intitalize the IO to communicate between the MCU and the module, then
  *          test that the modules is working using some basic AT commands.
  *          in case of success the string "OK" is returned inside otherwise
  *	         it is an error.
  * @param   None
  * @retval  ESP8266_OK on sucess, ESP8266_ERROR otherwise.
  */
ESP8266_StatusTypeDef ESP8266_Init(void)
{
  ESP8266_StatusTypeDef Ret;
  
  /* Configuration the IO low layer */
  if (ESP8266_IO_Init() < 0)
  {
    return ESP8266_ERROR;
  }
  
  /* Disable the Echo mode */
  
  /* Construct the command */
  memset (AtCmd, '\0', MAX_AT_CMD_SIZE);
  sprintf((char *)AtCmd, "ATE0%c%c", '\r', '\n');
  
  /* Send the command */
  Ret = runAtCmd(AtCmd, strlen((char *)AtCmd), (uint8_t*)AT_OK_STRING);
    
  /* Exit in case of error */
  if (Ret !=  ESP8266_OK)
  {
    return ESP8266_ERROR;
  }
  
  /* Setup the module in Station Mode*/
  
  /* Construct the command */
  memset (AtCmd, '\0', MAX_AT_CMD_SIZE);
  sprintf((char *)AtCmd, "AT+CWMODE=1%c%c", '\r', '\n');
  
  /* Send the command */
  Ret = runAtCmd(AtCmd, strlen((char *)AtCmd), (uint8_t*)AT_OK_STRING);
  
  /* Disable multiple connection by default. */
  
  /* Construct the command */
  memset (AtCmd, '\0', MAX_AT_CMD_SIZE);
  sprintf((char *)AtCmd, "AT+CIPMUX=0%c%c", '\r', '\n');
  
  /* Send the command */
  Ret = runAtCmd(AtCmd, strlen((char *)AtCmd), (uint8_t*)AT_OK_STRING);

  return Ret;
}

/**
  * @brief   Deinitialize the ESP8266 module.
  * @details Restarts the module  and stop the IO. AT command can't be executed
             unless the module is reinitialized.
  * @param   None
  * @retval  ESP8266_OK on sucess, ESP8266_ERROR otherwise.
  */
ESP8266_StatusTypeDef ESP8266_DeInit(void)
{
  ESP8266_StatusTypeDef Ret;

  /* Construct the command */
  sprintf((char *)AtCmd, "AT+RST%c%c", '\r', '\n');
  
  /* Send the command */
  Ret = runAtCmd(AtCmd, strlen((char *)AtCmd), (uint8_t*)AT_OK_STRING);
  
  /* Free resources used by the module */
  ESP8266_IO_DeInit();
  
  return Ret;
}


/**
  * @brief  Restarts the esp8266 module.
  * @param  None
  * @retval ESP8266_OK on sucess, ESP8266_ERROR otherwise.
  */
ESP8266_StatusTypeDef ESP8266_Restart(void)
{
  ESP8266_StatusTypeDef Ret;

  /* Construct the command */
  memset (AtCmd, '\0', MAX_AT_CMD_SIZE);
  sprintf((char *)AtCmd, "AT+RST%c%c", '\r', '\n');
  
  /* Send the command */
  Ret = runAtCmd(AtCmd, strlen((char *)AtCmd), (uint8_t*)AT_OK_STRING);
  
  return Ret;
}

/**
  * @brief  Join an Access point.
  * @param  Ssid: the access point id.
  * @param  Password the Access point password.
  * @retval Returns ESP8266_AT_COMMAND_OK on success and ESP8266_AT_COMMAND_ERROR otherwise.
  */
ESP8266_StatusTypeDef ESP8266_JoinAccessPoint(uint8_t* Ssid, uint8_t* Password)
{
  ESP8266_StatusTypeDef Ret;

  /* List all the available Access points first
   then check whether the specified 'ssid' exists among them or not.*/
  memset(AtCmd, '\0', MAX_AT_CMD_SIZE);
  sprintf((char *)AtCmd, "AT+CWJAP=\"%s\",\"%s\"%c%c", Ssid, Password, '\r', '\n');

  /* Send the command */
  Ret = runAtCmd(AtCmd, strlen((char *)AtCmd), (uint8_t*)AT_OK_STRING);
  
  return Ret;
}

/**
  * @brief  Quit an Access point if any.
  * @param  None
  * @retval returns ESP8266_AT_COMMAND_OK on success and ESP8266_AT_COMMAND_ERROR otherwise.
  */
ESP8266_StatusTypeDef ESP8266_QuitAccessPoint(void)
{
  ESP8266_StatusTypeDef Ret;
  
  /* Construct the CWQAP command */
  memset(AtCmd, '\0', MAX_AT_CMD_SIZE);
  sprintf((char *)AtCmd, "AT+CWQAP%c%c", '\r', '\n');

  /* Send the command */
  Ret = runAtCmd(AtCmd, strlen((char *)AtCmd), (uint8_t*)AT_OK_STRING);
  
  return Ret;
}

/**
  * @brief  Get the IP address for the esp8266 in Station mode.
  * @param  Mode: a ESP8266_ModeTypeDef to choose the Station or AccessPoint mode.
                 only the Station Mode is supported.
  * @param  IpAddress buffer to contain the IP address.
  * @retval Returns ESP8266_OK on success and ESP8266_ERROR otherwise
  */
ESP8266_StatusTypeDef ESP8266_GetIPAddress(ESP8266_ModeTypeDef Mode, uint8_t* IpAddress)
{
  ESP8266_StatusTypeDef Ret = ESP8266_OK;
  char *Token, *temp;
  
  /* Initialize the IP address and command fields */
  strcpy((char *)IpAddress, "0.0.0.0");
  memset(AtCmd, '\0', MAX_AT_CMD_SIZE);
  
  /* Construct the CIFSR command */
  sprintf((char *)AtCmd, "AT+CIFSR%c%c", '\r', '\n');

  /* Send the CIFSR command */
  Ret = runAtCmd(AtCmd, strlen((char *)AtCmd), (uint8_t*)AT_OK_STRING);
  
  /* If ESP8266_OK is returned it means the IP Adress inside the RxBuffer 
     has already been read */
  if ( Ret == ESP8266_OK)
  {
    /* The IpAddress for the Station Mode is returned in the format
     ' STAIP,"ip_address" '
      look for the token "STAIP," , then read the ip address located 
      between two double quotes */
    Token = strstr((char *)RxBuffer, "STAIP,");
    Token+=7;
    
    temp = strstr(Token, "\"");
    *temp = '\0';
 
    /* Get the IP address value */
    strcpy((char *)IpAddress, Token);
  }
  
  return Ret;
}
 
/**
  * @brief  Establish a network connection.
  * @param  Connection_info a pointer to a ESP8266_ConnectionInfoTypeDef struct containing the connection info.
  * @retval Returns ESP8266_AT_COMMAND_OK on success and ESP8266_AT_COMMAND_ERROR otherwise.
  */
ESP8266_StatusTypeDef ESP8266_EstablishConnection(const ESP8266_ConnectionInfoTypeDef* connection_info)
{
  ESP8266_StatusTypeDef Ret;
  
  /* Check the connection mode */
  if (connection_info->isServer)
  {
	/* Server mode not supported for this version yet */
    return ESP8266_ERROR;
  }
  
  /* Construct the CIPSTART command */
  memset(AtCmd, '\0', MAX_AT_CMD_SIZE);
  sprintf((char *)AtCmd, "AT+CIPSTART=\"TCP\",\"%s\",%lu%c%c", (char *)connection_info->ipAddress, connection_info->port,'\r', '\n');
  
  /* Send the CIPSTART command */  
  Ret = runAtCmd(AtCmd, strlen((char *)AtCmd), (uint8_t*)AT_OK_STRING);
  
  return Ret;
}

/**
  * @brief   Close a network connection.
  * @details Use the ALL_CONNECTION_ID to close all connections.
  * @param   Channel_id the channel ID of the connection to close.
  * @retval  Returns ESP8266_AT_COMMAND_OK on success and ESP8266_AT_COMMAND_ERROR otherwise.
  */
ESP8266_StatusTypeDef ESP8266_CloseConnection(const uint8_t channel_id)
{
  /* Working with a single connection, no channel_id is required */
  ESP8266_StatusTypeDef Ret;

  /* Construct the CIPCLOSE command */
  memset(AtCmd, '\0', MAX_AT_CMD_SIZE);
  sprintf((char *)AtCmd, "AT+CIPCLOSE%c%c", '\r', '\n');
          
  /* Send the CIPCLOSE command */
  Ret = runAtCmd(AtCmd, strlen((char *)AtCmd), (uint8_t*)AT_OK_STRING);
  
  return Ret;
}

/**
  * @brief  Send data over the wifi connection.
  * @param  Buffer: the buffer to send
  * @param  Length: the Buffer's data size.
  * @retval Returns ESP8266_OK on success and ESP8266_ERROR otherwise.
  */
ESP8266_StatusTypeDef ESP8266_SendData(uint8_t* Buffer, uint32_t Length)
{
  ESP8266_StatusTypeDef Ret = ESP8266_OK;
  
  if (Buffer != NULL)
  {
    uint32_t tickStart;
	
	/* Construct the CIPSEND command */
    memset(AtCmd, '\0', MAX_AT_CMD_SIZE);
    sprintf((char *)AtCmd, "AT+CIPSEND=%lu%c%c", Length, '\r', '\n');
    
    /* The CIPSEND command doesn't have a return command
       until the data is actually sent. Thus we check here whether
       we got the '>' prompt or not. */
    Ret = runAtCmd(AtCmd, strlen((char *)AtCmd), (uint8_t*)AT_SEND_PROMPT_STRING);
  
    /* Return Error */
    if (Ret != ESP8266_OK)
	{
      return ESP8266_ERROR;
	}
  
   /* Wait before sending data. */
    tickStart = HAL_GetTick();
    while (HAL_GetTick() - tickStart < 500)
    {
    }

	/* Send the data */
	Ret = runAtCmd(Buffer, Length, (uint8_t*)AT_SEND_OK_STRING);
  }
  
  return Ret;
}


/**
  * @brief  receive data over the wifi connection.
  * @param  pData the buffer to fill will the received data.
  * @param  Length the maximum data size to receive.
  * @param RetLength the actual data received.
  * @retval returns ESP8266_OK on success and ESP8266_ERROR otherwise.
  */
ESP8266_StatusTypeDef ESP8266_ReceiveData(uint8_t* pData, uint32_t Length, uint32_t* RetLength)
{
  ESP8266_StatusTypeDef Ret;
  
  /* Receive the data from the host */
  Ret = getData(pData, Length, RetLength);
  
  return Ret;
}

/**
  * @brief  Run the AT command
  * @param  cmd the buffer to fill will the received data.
  * @param  Length the maximum data size to receive.
  * @param  Token the expected output if command runs successfully
  * @retval Returns ESP8266_OK on success and ESP8266_ERROR otherwise.
  */
static ESP8266_StatusTypeDef runAtCmd(uint8_t* cmd, uint32_t Length, const uint8_t* Token)
{
  uint32_t idx = 0;
  uint8_t RxChar;
  
  /* Reset the Rx buffer to make sure no previous data exist */
  memset(RxBuffer, '\0', MAX_BUFFER_SIZE);
  
  /* Send the command */
  if (ESP8266_IO_Send(cmd, Length) < 0)
  {
    return ESP8266_ERROR;
  }

  /* Wait for reception */
  while (1)
  {	
	/* Wait to recieve data */
    if (ESP8266_IO_Receive(&RxChar, 1) != 0)
    {
      RxBuffer[idx++] = RxChar; 
    }
    else
    {
      break;
    }
    
	/* Check that max buffer size has not been reached */
    if (idx == MAX_BUFFER_SIZE)
    {
      break;
    }

	/* Extract the Token */
    if (strstr((char *)RxBuffer, (char *)Token) != NULL)
    {
      return ESP8266_OK;
    }
    
	/* Check if the message contains error code */
    if (strstr((char *)RxBuffer, AT_ERROR_STRING) != NULL)
    {
      return ESP8266_ERROR;
    }
  }

  return ESP8266_ERROR;
}

/**
  * @brief  Receive data from the WiFi module
  * @param  Buffer The buffer where to fill the received data
  * @param  Length the maximum data size to receive.
  * @param  RetLength Length of received data
  * @retval Returns ESP8266_OK on success and ESP8266_ERROR otherwise.
  */
static ESP8266_StatusTypeDef getData(uint8_t* Buffer, uint32_t Length, uint32_t* RetLength)
{
  uint8_t RxChar;
  uint32_t idx = 0;
  uint8_t LengthString[4];
  uint32_t LengthValue;
  uint8_t i = 0;
  ESP8266_Boolean newChunk  = ESP8266_FALSE;
  
  /* Reset the reception data length */
  *RetLength = 0;

  /* Reset the reception buffer */  
  memset(RxBuffer, '\0', MAX_BUFFER_SIZE);

  /* When reading data over a wifi connection the esp8266
     splits it into chunks of 1460 bytes maximum each, each chunk is preceded
     by the string "+IPD,<chunk_size>:". Thus to get the actual data we need to:
       - Receive data until getting the "+IPD," token, a new chunk is marked.
       - Extract the 'chunk_size' then read the next 'chunk_size' bytes as actual data
       - Mark end of the chunk.
       - Repeat steps above until no more data is available. */
  while (1)
  {
    if (ESP8266_IO_Receive(&RxChar, 1) != 0)
    {
      /* The data chunk starts with +IPD,<chunk length>: */
      if ( newChunk == ESP8266_TRUE)
      {
        /* Read the next lendthValue bytes as part from the actual data. */
        if (LengthValue--) 
        {
          *Buffer++ = RxChar;
          (*RetLength)++;
        }
        else
        {
           /* Clear the buffer as the new chunk has ended. */
           newChunk = ESP8266_FALSE;
           memset(RxBuffer, '\0', MAX_BUFFER_SIZE);
           idx = 0;
        }
      }
      RxBuffer[idx++] = RxChar;
    }
    else
    {
     /* Errors while reading return an error. */
      if ((newChunk == ESP8266_TRUE) && (LengthValue != 0))
      {
        return ESP8266_ERROR;
      }
      else
      {
        break;
      }
    }

    if (idx == MAX_BUFFER_SIZE)
    {
      /* In case of Buffer overflow, return error */
      if ((newChunk == ESP8266_TRUE) && (LengthValue != 0))
      {
        return ESP8266_ERROR;
      }
      else
      {
        break;
      }
    }
	
    /* When a new chunk is met, extact its size */
    if ((strstr((char *)RxBuffer, AT_IPD_STRING) != NULL) && (newChunk == ESP8266_FALSE)) 
    {
      i = 0;
      memset(LengthString, '\0', 4);
      do
      {
        ESP8266_IO_Receive(&RxChar, 1);
        LengthString[i++] = RxChar;
      }
      while(RxChar != ':');

	  /* Get the buffer length */
      LengthValue = atoi((char *)LengthString);
      newChunk = ESP8266_TRUE;
    }

	/* Check if message contains error code */
    if (strstr((char *)RxBuffer, AT_ERROR_STRING) != NULL)
    {
      return ESP8266_ERROR;
    }

	/* Check for the chunk end */
    if (strstr((char *)RxBuffer, AT_IPD_OK_STRING) != NULL)
    {
      newChunk = ESP8266_FALSE;
    }
  }
  
  return ESP8266_OK;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
