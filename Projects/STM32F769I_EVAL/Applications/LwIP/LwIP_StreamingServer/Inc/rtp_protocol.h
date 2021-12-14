/**
  ******************************************************************************
  * @file    LwIP/LwIP_StreamingServer/Inc/rtp_protocol.h 
  * @author  MCD Application Team
  * @brief   Header for rtp_protocol.c module
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
#ifndef __RTP_H
#define __RTP_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip/sys.h"
#include "lwip/sys.h"
#include "stm32f769i_eval_camera.h"
#include "lwip/sockets.h"
#include "stm32f7xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
/* Exported constants --------------------------------------------------------*/
#define RTP_VERSION                 0x80
#define RTP_TIMESTAMP               1500
#define RTP_MARKER_BIT              0x80
#define RTP_PACKET_SIZE             1500
#define RTP_PAYLOAD_SIZE_MAX        1400
#define RTP_PAYLOAD_TYPE            0x1A    /* define a rtp payload jpeg 26*/
#define min(x, y) (((x) < (y)) ? (x) : (y))


/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  Connection Status definition  
  */  
typedef enum 
{
  CONNECT_OK         = 0x00U,
  CONNECT_CLOSED     = 0x01U,
  CONNECT_ERROR      = 0x02U

} Connection_StatusTypeDef;


/** 
  * @brief  RTP Status definition  
  */  
typedef enum 
{
  RTP_STATE_IDLE         = 0x00U,
  RTP_STATE_READY        = 0x01U,
  RTP_STATE_START        = 0x02U,
  RTP_STATE_SEND         = 0x03U,
  RTP_STATE_ERROR        = 0x04U,
  RTP_STATE_STOPPED      = 0x05U, 
  RTP_STATE_UNKNOWN      = 0x06U

} RTP_StatusTypeDef;

/** 
  * @brief  RTP Structure definition
  */
typedef struct
{
  __IO RTP_StatusTypeDef       State;         /*!< RTSP process current state   */

  struct sockaddr_in local;                   /*!< A local member in the network */
  
  struct sockaddr_in net_dest;                /*!< A destination member in the network */
  
  char* rtp_data;                             /*!< Pointer to RTP/JPEG packet */
  
  uint32_t Offset;                            /*!< RTP/JPEG Offset */ 
  
  uint8_t rtp_send_packet[RTP_PACKET_SIZE];   /*!< Packet to send */
  
  int sock_id;                                /*!< socket ID */

}RTP_HandleTypeDef;


#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/bpstruct.h"
#endif

PACK_STRUCT_BEGIN
struct rtp_hdr_t {
  PACK_STRUCT_FIELD(u8_t  version);
  PACK_STRUCT_FIELD(u8_t  pt);
  PACK_STRUCT_FIELD(u16_t seq);
  PACK_STRUCT_FIELD(u32_t ts);
  PACK_STRUCT_FIELD(u32_t ssrc);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END

#ifdef PACK_STRUCT_USE_INCLUDES
#include "arch/epstruct.h"
#endif

/* private functions ------------------------------------------------------- */ 
void RTP_Init(void);
void Send_Sem(void const * );
uint32_t RTP_Close_Connection(void);
RTP_StatusTypeDef RTP_GetState(RTP_HandleTypeDef *RTP_struct_ptr);
uint32_t RTP_Stop(void);

#endif /* __RTP_H */

