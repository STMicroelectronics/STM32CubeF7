/**
  ******************************************************************************
  * @file    Applications\WiFi\ESP8266_IAP_Client\Inc\esp8266.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the 
  *          esp8266 WiFi driver.   
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef __ESP8266_H
#define __ESP8266_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f7xx_hal.h"
/* Private define ------------------------------------------------------------*/
#define MAX_BUFFER_SIZE         1500
#define MAX_AT_CMD_SIZE         256
#define AT_OK_STRING            "OK\r\n"
#define AT_IPD_OK_STRING        "OK\r\n\r\n"
#define AT_SEND_OK_STRING       "SEND OK\r\n"
#define AT_SEND_PROMPT_STRING   "OK\r\n>"
#define AT_ERROR_STRING         "ERROR\r\n"
#define AT_IPD_STRING           "+IPD,"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  ESP8266_FALSE         = 0,
  ESP8266_TRUE          = 1
} ESP8266_Boolean;

typedef enum
{ 
  ESP8266_OK                            = 0,
  ESP8266_ERROR                         = 1,
  ESP8266_BUSY                          = 2,
  ESP8266_ALREADY_CONNECTED             = 3,
  ESP8266_CONNECTION_CLOSED             = 4,
  ESP8266_TIMEOUT                       = 5,
  ESP8266_IO_ERROR                      = 6,
} ESP8266_StatusTypeDef;

typedef enum
{
  ESP8266_ENCRYPTION_OPEN         = 0,
  ESP8266_ENCRYPTION_WEP          = 1,
  ESP8266_ENCRYPTION_WPA_PSK      = 2,
  ESP8266_ENCRYPTION_WPA2_PSK     = 3,
  ESP8266_ENCRYPTION_WPA_WPA2_PSK = 4,
} ESP8266_EncryptionTypeDef;

typedef enum
{
  ESP8266_STATION_MODE      = 0,
  ESP8266_ACCESSPPOINT_MODE = 1,
  ESP8266_MIXED_MODE        = 2,
} ESP8266_ModeTypeDef;

typedef enum
{
  NORMAL_MODE      = 0,
  UNVARNISHED_MODE = 1
} ESP8266_TransferModeTypeDef;

typedef enum
{
  ESP8266_GOT_IP_STATUS       = 1,
  ESP8266_CONNECTED_STATUS    = 2,
  ESP8266_DISCONNECTED_STATUS = 3,
} ESP8266_ConnectionStatusTypeDef;

typedef enum
{
  ESP8266_TCP_CONNECTION = 0,
  ESP8266_UDP_CONNECTION = 1,
} ESP8266_ConnectionModeTypeDef;

typedef enum
{
  UDP_PEER_NO_CHANGE   = 0,
  UPD_PEER_CHANGE_ONCE = 1,
  UDP_PEER_CHANGE_ALLOWED = 2,
  UDP_PEER_CHANGE_INVALID = -1,
} ESP8266_ConnectionPolicyTypeDef;

typedef struct
{
  ESP8266_ConnectionStatusTypeDef  connectionStatus;
  ESP8266_ConnectionModeTypeDef    connectionType;
  ESP8266_ConnectionPolicyTypeDef  connectionMode;  /* For UDP connections only */
  uint8_t                          connectionID;
  uint8_t*                         ipAddress;
  uint32_t                         port;
  uint32_t                         localPort;       /* for UDP connection only */
  ESP8266_Boolean                  isServer;  
} ESP8266_ConnectionInfoTypeDef;

typedef struct
{
  uint8_t*                   ssid;
  uint8_t*                   password;
  uint16_t                   channedID;
  ESP8266_EncryptionTypeDef  encryptionMode;
} ESP8266_APConfigTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
ESP8266_StatusTypeDef ESP8266_Init(void);
ESP8266_StatusTypeDef ESP8266_DeInit(void);
ESP8266_StatusTypeDef ESP8266_Restart(void);
  
ESP8266_StatusTypeDef ESP8266_QuitAccessPoint(void);
ESP8266_StatusTypeDef ESP8266_JoinAccessPoint(uint8_t* ssid, uint8_t* password);
ESP8266_StatusTypeDef ESP8266_GetIPAddress(ESP8266_ModeTypeDef mode, uint8_t* ip_address);
ESP8266_StatusTypeDef ESP8266_EstablishConnection(const ESP8266_ConnectionInfoTypeDef* connection_info);
ESP8266_StatusTypeDef ESP8266_CloseConnection(const uint8_t channel_id);

ESP8266_StatusTypeDef ESP8266_SendData(uint8_t* pData, uint32_t Length);
ESP8266_StatusTypeDef ESP8266_ReceiveData(uint8_t* pData, uint32_t Length, uint32_t* retLength);

#ifdef __cplusplus
}
#endif

#endif /* __ESP8266_H */

