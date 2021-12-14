/**
  ******************************************************************************
  * @file    LwIP/LwIP_StreamingServer/Inc/rtsp_protocol.h 
  * @author  MCD Application Team
  * @brief   Header for rtsp_protocol.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTSP_H
#define __RTSP_H

/* Includes ------------------------------------------------------------------*/
#include "rtp_protocol.h"
#include <string.h>
#include <stdlib.h>
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "stm32f769i_eval_camera.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  RTSP Status structures definition  
  */  
typedef enum
{
  SESSION_ALIVE      = 0x00U,
  SESSION_TEARDOWN   = 0x01U
  
} RTSP_SessionStatusTypeDef;

/** 
  * @brief  RTSP Status structures definition  
  */  
typedef enum
{
  RTSP_STATE_IDLE     = 0x00U,
  RTSP_STATE_CREATE   = 0x01U,
  RTSP_STATE_START    = 0x02U,
  RTSP_STATE_SETUP    = 0x03U,
  RTSP_STATE_PLAY     = 0x04U,
  RTSP_STATE_TEARDOWN = 0x05U,
  RTSP_STATE_INVALID  = 0x06U,
  RTSP_STATE_STOPPED  = 0x07U,
  RTSP_STATE_ERROR    = 0x08U,
  RTSP_STATE_UNKNOWN  = 0x09U
}RTSP_StatusTypeDef;

/** 
  * @brief  RTSP Structure definition
  */
typedef struct
{
  __IO RTSP_StatusTypeDef   State;                   /*!< RTSP process state */
  
  __IO RTSP_SessionStatusTypeDef Session_State;      /*!< RTSP session state */
  
  int sock_id;                                       /*!< ID sockets */
  
  int conn_sock_id;                                  /*!< ID accept connection sockets  */
  
  char *rtsp_resp;                                   /*!< RTSP response pointer  */
  
  char *rtsp_seq;                                    /*!< RTSP sequence number pointer */
  
  char *rtsp_transp_type;                            /*!< RTSP transport type pointer */
  
  char *rtsp_req_header;                             /*!< RTSP require header pointer */

}RTSP_HandleTypeDef;

/* Exported constants ------------------------------------------------------------*/

/** 
  * @brief RTSP messages definition  
  */    
#define RTSP_MSG_OK                          "200 OK\r\n"
#define RTSP_MSG_NOT_FOUND                   "404 Not Found\r\n"
#define RTSP_MSG_404                         "404"
#define RTSP_MSG_METHOD_NOT_ALLOWED          "405 Method Not Allowed\r\n"
#define RTSP_MSG_SESSION_NOT_FOUND           "454 Session Not Found\r\n"
#define RTSP_MSG_METHOD_NOT_VALID            "455 Method Not Valid In This State\r\n"
#define RTSP_MSG_TRANSPORT_UNSUPPORTED       "461 Unsupported Transport\r\n"
#define RTSP_MSG_OPTION_NOT_SUPPORTED        "551 Option not supported\r\n"
#define RTSP_MSG_PUBLIC                      "Public: DESCRIBE, SETUP, TEARDOWN, PLAY\r\n\r\n"
#define RTSP_MSG_ALLOWED_HEADER              "Allow: SETUP, TEARDOWN, PLAY\r\n"
#define RTSP_MSG_REQUEST_IS_MULTICAST        "multicast"
#define RTSP_MSG_OPTIONS                     "OPTIONS"
#define RTSP_MSG_DESCRIBE                    "DESCRIBE"
#define RTSP_MSG_SETUP                       "SETUP"
#define RTSP_MSG_PLAY                        "PLAY"   
#define RTSP_MSG_TEARDOWN                    "TEARDOWN"
#define RTSP_MSG_SEQUENCE_NUMBER             "CSeq"
#define RTSP_MSG_TRANSPORT_TYPE_UNSUPPORTED  "Unsupported:"
#define RTSP_MSG_CONN_CLOSE                  "Connection: Close\r\n\r\n"
#define RTSP_MSG_SESSION_ID                  "Session: 1234567890"
#define RTSP_MSG_TRANSPORT_IS_TCP            "RTP/AVP/TCP"
#define RTSP_MSG_CONTENTS                    "Content-base: rtsp://192.168.0.10:554/\r\nContent-Type: application/sdp\r\nContent-Length: 113\r\n\r\n"
#define RTSP_MSG_SDP_ATTRIBUTES              "v=0\r\no=stream 2890844526 1234567890 IANA IP4 192.168.0.10\r\ns=smtDev stream\r\nt=0 0\r\nm=video 49152 RTP/AVP 26\r\n\r\n"

/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
void RTSP_Init(void);
void RTSP_Stop(void);
RTSP_StatusTypeDef RTSP_GetState(RTSP_HandleTypeDef *RTSP_struct_ptr);

#endif /* __RTSP_H */

/**
  * @}
  */
