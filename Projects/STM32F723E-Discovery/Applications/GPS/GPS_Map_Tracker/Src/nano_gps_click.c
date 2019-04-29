/**
  ******************************************************************************
  * @file    GPS/GPS_Map_Tracker/Src/nano_gps_click.c
  * @author  MCD Application Team
  * @brief   This file implments the IO operations to deal with the Nano GPS click
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
#include "nano_gps_click.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef  STMOD_UART_Handle;
UART_HandleTypeDef  VCP_HandleStruct;
char GPS_Time[20];
char GPS_Date[14];
char GPS_Fix[2];
char GPS_SatInView[4];
char GPS_SatForFix[4];
char GPS_latitude[15];
char GPS_Longitude[15];
char GPS_Speed[8];
char GPS_Altitude[8];

char NMEA_String[256] ;



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  GPS Initalization.
  *         This function inits the UART interface to deal with the GPS Nano Click,
  *         then starts asynchronous listening on the RX port.
  * @param None
  * @retval None
  */
void GPS_Init(void)
{
  /* UART init for communication with GPS module */
  STMOD_UART_Handle.Instance          = STMOD_UART;
  STMOD_UART_Handle.Init.BaudRate     = 4800;
  STMOD_UART_Handle.Init.WordLength   = UART_WORDLENGTH_8B;
  STMOD_UART_Handle.Init.StopBits     = UART_STOPBITS_1;
  STMOD_UART_Handle.Init.Parity       = UART_PARITY_NONE;
  STMOD_UART_Handle.Init.Mode         = UART_MODE_TX_RX; /* RX only for GPS */
  STMOD_UART_Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  STMOD_UART_Handle.Init.OverSampling = UART_OVERSAMPLING_16;
  STMOD_UART_Handle.pTxBuffPtr        = 0;
  STMOD_UART_Handle.TxXferSize        = 0;
  STMOD_UART_Handle.TxXferCount       = 0;
  STMOD_UART_Handle.pRxBuffPtr        = 0;
  STMOD_UART_Handle.RxXferSize        = 0;
  STMOD_UART_Handle.RxXferCount       = 0;
  STMOD_UART_Handle.hdmatx            = 0;
  STMOD_UART_Handle.hdmarx            = 0;
  STMOD_UART_Handle.Lock              = HAL_UNLOCKED;
  STMOD_UART_Handle.gState            = HAL_UART_STATE_RESET;
  STMOD_UART_Handle.ErrorCode         = HAL_UART_ERROR_NONE;

  if( HAL_UART_Init( &STMOD_UART_Handle )!=HAL_OK )
  {
    while(1);

  }
}

/**
  * @brief  Receive Data from the GPS Nano module over the UART interface.
  *         This function receives data from the  GPS module, the
  *         data is fetched from a ring buffer that is asynchonously and continuously
            filled with the received data.
  * @param  Buffer: a buffer inside which the data will be read.
  * @param  Length: the Maximum size of the data to receive.
  * @retval None
  */
void GPS_Start(uint8_t* Buffer, uint32_t Length)
{
  /* Enable UART Reception */
  HAL_UART_Receive_DMA(&STMOD_UART_Handle, (uint8_t*) Buffer, Length );
  
}

/**
  * @brief  GPS Receive Message.
  *         This function receive GPS frame from the UART interface,
  * @param None
  * @retval None
  */
void GPS_ReceiveMsg(uint16_t* rd_idx, uint32_t timeout )
{
  
  uint32_t tickstart = HAL_GetTick();
  char car;
  uint32_t idex =0;   
  do
  {

    if ( __HAL_DMA_GET_COUNTER(STMOD_UART_Handle.hdmarx)!=(*rd_idx) )
    {

	car = *(STMOD_UART_Handle.pRxBuffPtr + STMOD_RX_BUF_SIZE-(*rd_idx));

	NMEA_String[idex] = car;    
	idex++;

	if ((idex == 255) || (car =='\n'))
	{
          NMEA_String[idex]= '\0';
          idex = 0;
          
          VCP_SendMessage("%s",NMEA_String);
          GPS_DecodeFrameNMEA(NMEA_String);
	}

      /* A new char on the UART_Rx buffer */
      (*rd_idx)--;
      if( (*rd_idx)==0 )
        *rd_idx = STMOD_RX_BUF_SIZE;

    }
  }
 while( HAL_GetTick()<(tickstart+timeout) );
}

/**
* @brief  Decode NMEA Frame 
* @param  Nmea
* @retval None
*/
void GPS_DecodeFrameNMEA (char * Nmea)
{
  uint32_t field_index = 0;
  uint32_t field_size = 0;
  char * nmea_begin;
  char * nmea_end;
  char * target_field;
  
  if (strlen(Nmea) >256) return;
  
  /* initialize first pointer */
  nmea_begin = (char*)strchr(Nmea,',');
  
  if (nmea_begin == NULL) return;
  
  /* Decode GGA frame */
  if (strstr(Nmea,"$GPGGA")!= NULL)
  {
    while ((field_index <14) && ((nmea_end = (char*)strchr(nmea_begin+1,',')) != NULL))
    {
      if (field_index == 0) 
      { 
        target_field = GPS_Time;
      }
      else if (field_index == 1) 
      {
        target_field = GPS_latitude;
        nmea_end = (char*)strchr(nmea_end+1, ',');
        field_index++;
      }
      else if (field_index == 3) 
      {
        target_field = GPS_Longitude;
        nmea_end = (char*)strchr(nmea_end+1,',');
        field_index++;
      }
      else if (field_index == 6) 
      {
        target_field = GPS_SatForFix;
      }
      else if (field_index == 8) 
      {
        target_field = GPS_Altitude;
      }
      else 
      {
        target_field = NULL;
      }
      
      if (target_field != NULL)
      {
        field_size = nmea_end-nmea_begin - 1;
        if (field_size) 
        {
          strncpy(target_field,nmea_begin+1, field_size);
        }
        target_field[field_size] = '\0';
      }
      nmea_begin = nmea_end;
      field_index++; 
    }
    return;
  }
  /* Decode GSA frame */
  if (strstr(Nmea,"$GPGSA")!= NULL)
  {
    while ((field_index <4) && ((nmea_end = (char*)strchr(nmea_begin+1,',')) != NULL))
    {
      if (field_index == 1) 
      {
        target_field = GPS_Fix;
      }
      else 
      {
        target_field = NULL;
      }
      
      if (target_field != NULL)
      {
        field_size = nmea_end - nmea_begin - 1;
        strncpy(target_field, nmea_begin+1, field_size);
        target_field[field_size] = '\0';
      }
      nmea_begin = nmea_end;
      field_index++;
    }
    return;
  }
  /* Decode GSV frame */
  if (strstr(Nmea,"$GPGSV")!= NULL)
  {
    while ((field_index <4) && ((nmea_end = (char*)strchr(nmea_begin+1,',')) != NULL))
    {
      if (field_index == 2) 
      {
        target_field = GPS_SatInView;
      }
      else
      {
        target_field = NULL;
      }
      
      if (target_field != NULL)
      {
        field_size = nmea_end - nmea_begin - 1;
        strncpy(target_field,nmea_begin+1, field_size);
        target_field[field_size] = '\0';
        
      }
      nmea_begin = nmea_end;
      field_index++;
    }
    return;
  }
  /* Decode RMC frame */
  if (strstr(Nmea,"$GPRMC")!= NULL)
  {
    while ((field_index <12) && ((nmea_end = (char*)strchr(nmea_begin+1,',')) != NULL))
    {
      if (field_index == 0) 
      {
        target_field = GPS_Time;
      }
      else if (field_index == 2) 
      {
        target_field = GPS_latitude;
        nmea_end = (char*)strchr(nmea_end+1, ',');
        field_index++;
      }
      else if (field_index == 4) 
      {
        target_field = GPS_Longitude;
        nmea_end = (char*)strchr(nmea_end+1, ',');
        field_index++;
      }
      else if (field_index == 6) 
      {
        target_field = GPS_Speed;
      }
      else if (field_index == 8) 
      {
        target_field = GPS_Date;
      }
      else 
      {
        target_field = NULL;
      }
      
      if (target_field != NULL)
      {
        field_size = nmea_end - nmea_begin - 1;
        strncpy(target_field, nmea_begin+1, field_size);
        target_field[field_size] = '\0';
      }
      nmea_begin = nmea_end;
      field_index++;
    }
    return;
  }
  
  return;
}

/**
  * @brief  Nano GPS click Deinitialization.
  *         This function Deinits the UART interface of the Nano GPS. When called
  *         the Nano GPS commands can't be executed anymore.
  * @param  None.
  * @retval None.
  */
void GPS_DeInit(void)
{
  /* Reset USART configuration to default */
  HAL_UART_DeInit(&STMOD_UART_Handle);
}

/**
  * @brief  VCP Initalization.
  *         This function inits the UART interface to deal with the ST-link,
  * @param None
  * @retval None
  */
void VCP_Init(void)
{
  VCP_HandleStruct.Instance          = VCP_PORT;

  VCP_HandleStruct.Init.BaudRate     = 4800;
  VCP_HandleStruct.Init.WordLength   = UART_WORDLENGTH_8B;
  VCP_HandleStruct.Init.StopBits     = UART_STOPBITS_1;
  VCP_HandleStruct.Init.Parity       = UART_PARITY_NONE;
  VCP_HandleStruct.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  VCP_HandleStruct.Init.Mode         = UART_MODE_TX_RX;
  VCP_HandleStruct.Init.OverSampling = UART_OVERSAMPLING_16;

  VCP_HandleStruct.pTxBuffPtr        = 0;
  VCP_HandleStruct.TxXferSize        = 0;
  VCP_HandleStruct.TxXferCount       = 0;
  VCP_HandleStruct.pRxBuffPtr        = 0;
  VCP_HandleStruct.RxXferSize        = 0;
  VCP_HandleStruct.RxXferCount       = 0;
  VCP_HandleStruct.hdmatx            = 0;
  VCP_HandleStruct.hdmarx            = 0;
  VCP_HandleStruct.Lock              = HAL_UNLOCKED;
  VCP_HandleStruct.gState            = HAL_UART_STATE_RESET;
  VCP_HandleStruct.ErrorCode         = HAL_UART_ERROR_NONE;
  if (HAL_UART_Init(&VCP_HandleStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/**
  * @brief  Send message Via VCP ST link.
  * @param  fmt
  * @retval None
  */
void VCP_SendMessage(char *fmt, ...)
{
  uint8_t i=0, str_size;
  va_list args;
  char str_result[100];
  
  va_start(args, fmt);
  vsprintf(str_result, fmt, args); 
  va_end(args);
  
  str_size = strlen(str_result);
  
  for(i=0; i<str_size; i++)
  {
    HAL_UART_Transmit( &VCP_HandleStruct, (uint8_t*) str_result+i, 1, 100);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
