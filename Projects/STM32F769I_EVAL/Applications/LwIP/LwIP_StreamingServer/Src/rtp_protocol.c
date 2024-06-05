/**
  ******************************************************************************
  * @file    LwIP/LwIP_StreamingServer/Src/rtp_protocol.c 
  * @author  MCD Application Team
  * @brief   RTP client module
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
#include "rtp_protocol.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTP_HandleTypeDef RTP_struct;         /* RTP structure */
osSemaphoreId Sending_Semaphore;      /* Semaphore ID to signal transfer frame complete */
osThreadId Thr_Send_Sem;              /* Thread ID */
const char jpegRTP_header[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x4B, 0x14, 0x0f};

/* Exported variables---------------------------------------------------------*/
uint8_t RGB_buffer[RGB_BUFFER_SIZE];        /* RGB888 buffer */

/* Imported variables---------------------------------------------------------*/
extern uint8_t JPEG_buffer[JPEG_BUFFER_SIZE]; /* JPEG buffer */
extern osSemaphoreId RTP_SendSemaphore;
extern uint32_t JPEG_ImgSize;
/* Private function prototypes -----------------------------------------------*/
static void rtp_send_packets(int sock_id, struct sockaddr_in* net_dest,
                                          struct sockaddr_in* local);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize RTP thread 
  * @param  None
  * @retval None
  */
void RTP_Init(void)
{
  /* Reset the RTP structure */
  memset(&RTP_struct, 0x00, sizeof(RTP_struct));
  
  /* RTP state idle */
  RTP_struct.State = RTP_STATE_IDLE;
  
  /* Initialize the camera module */
  if(BSP_CAMERA_Init(CAMERA_R160x120) != CAMERA_OK)
  {
    Error_Handler();
  }

  /* Create new socket */
  RTP_struct.sock_id = socket(AF_INET, SOCK_DGRAM, 0);
  if (RTP_struct.sock_id >= 0)
  {
    /* Prepare local address */
    memset(&RTP_struct.local, 0x00, sizeof(RTP_struct.local));
    RTP_struct.local.sin_family = AF_INET;
    RTP_struct.local.sin_port = PP_HTONS(RTP_PORT);
    RTP_struct.local.sin_addr.s_addr = PP_HTONL(INADDR_ANY);
    
    /* Bind to local address */
    if (bind(RTP_struct.sock_id, (struct sockaddr *)&RTP_struct.local, sizeof(RTP_struct.local)) == 0) 
    {
      /* prepare RTP stream address */
      memset(&RTP_struct.net_dest, 0x00, sizeof(RTP_struct.net_dest));
      RTP_struct.net_dest.sin_family = AF_INET;
      RTP_struct.net_dest.sin_port = PP_HTONS(RTP_PORT);   
      RTP_struct.net_dest.sin_addr.s_addr = TARGET_IP_ADDRESS;
      
      /* Reset rtp packet */
      memset(RTP_struct.rtp_send_packet, 0x00, sizeof(RTP_struct.rtp_send_packet));
      
      osThreadDef(Snd, Send_Sem, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 4);
      Thr_Send_Sem = osThreadCreate (osThread(Snd), NULL);
      
      /*Start the continuous mode of the camera */
      BSP_CAMERA_ContinuousStart((uint8_t *)RGB_buffer);
      
      RTP_struct.rtp_data = (char *)JPEG_buffer;
      
      /* Next state is RTP_STATE_START */
      RTP_struct.State = RTP_STATE_START;
    }
    else
    {
      /* Binding local address failed */
      RTP_struct.State = RTP_STATE_ERROR;
    }
  }
}

/**
  * @brief  send RTP packets 
  * @param  socket's descriptor  
  * @param  destination address

  * @retval None
  */
static void rtp_send_packets(int sock_id, struct sockaddr_in* net_dest, struct sockaddr_in* local)
{
  struct rtp_hdr_t* rtphdr;          /* RTP header */
  uint8_t offset[3];                 /* The offset in the RTP/JPEG header */
  uint8_t* rtp_payload;              /* RTP payload */
  int rtp_payload_size = 0;          /* RTP payload size in the current packet */
  int rtp_data_index;                /* Index in the stream packet */
  
  /* prepare RTP packet */
  rtphdr = (struct rtp_hdr_t*) RTP_struct.rtp_send_packet;
  rtphdr->version = RTP_VERSION;
  rtphdr->pt = 0;
  rtphdr->ssrc = PP_HTONL(0); 

  /* send RTP stream packets */
  rtp_data_index = 0;
  
  /* While the end of JPEG file is not attained */
  while (((rtp_data_index + rtp_payload_size)  <= JPEG_ImgSize) || (rtphdr->pt != 0x9A))
  {
    /* Increment the timestamp value */
    rtphdr->ts = htonl(ntohl(rtphdr->ts) + RTP_TIMESTAMP);
     
    /* Set a payload pointer */
    rtp_payload = RTP_struct.rtp_send_packet + sizeof(struct rtp_hdr_t);
    
    /* Set a payload size */
    rtp_payload_size = min(RTP_PAYLOAD_SIZE_MAX, (JPEG_ImgSize - rtp_data_index));  
    
    /* set a RTP/JPEG header*/
    memcpy(rtp_payload, jpegRTP_header ,1 * sizeof(char));
    offset[0]= (RTP_struct.Offset & 0x00FF0000) >> 16;
    offset[1]= (RTP_struct.Offset & 0x0000FF00) >> 8;
    offset[2]= RTP_struct.Offset & 0x000000FF;
    memcpy(rtp_payload + 1, offset, 3 * sizeof(char));
    memcpy(rtp_payload + 4, jpegRTP_header + 4, 4 * sizeof(char));
    
    /* Set a payload */
    memcpy(rtp_payload + 8, RTP_struct.rtp_data + rtp_data_index, rtp_payload_size); 

    /* Set MARKER bit in RTP header on the last packet of an image */
    rtphdr->pt = RTP_PAYLOAD_TYPE| (((rtp_data_index + rtp_payload_size) >= (JPEG_ImgSize - 1)) ? RTP_MARKER_BIT : 0);   
    
    /* Send RTP stream packet */
    if (sendto(sock_id, RTP_struct.rtp_send_packet, sizeof(struct rtp_hdr_t) + rtp_payload_size + 8, 0, (struct sockaddr *)net_dest, sizeof(struct sockaddr)) > 0) 
    {
      /* Increment the sequence number */
      rtphdr->seq = htons(ntohs(rtphdr->seq) + 1);

      /* Increment the index by payload size */
      rtp_data_index += rtp_payload_size;
      
      /* Increment the offset in the RTP/JPEG header by the offset of the current packet */    
      RTP_struct.Offset += rtp_payload_size;
    }
  } 
}

/**
  * @brief  Sending thread
  * @param  None
  * @retval None
  */
void Send_Sem(void const * arg)
{
  while(1)
  {
    /* Try to obtain the semaphore. */
    if (osSemaphoreWait(RTP_SendSemaphore, 100) == osOK)
    {
      /* Send the JPEG image*/
      rtp_send_packets(RTP_struct.sock_id, &RTP_struct.net_dest, &RTP_struct.local);
    }
  }
}

/**
  * @brief  close RTP connection 
  * @param  None
  * @retval None
  */
uint32_t RTP_Close_Connection(void)
{
  uint32_t res_status = CONNECT_ERROR;
  
  res_status = closesocket(RTP_struct.sock_id);
  
  if(res_status == 0)
  {
    /* RTP connection is closed */
    res_status = CONNECT_CLOSED;
  }
  
  /* Close one end of a full-duplex connection */
  shutdown(RTP_struct.sock_id, 0);
  
  return res_status;
}

/**
  * @brief  Close RTP connection, Stop and reset Camera
  * @param  none
  * @retval 0 success, 1 error
  */                          
uint32_t RTP_Stop(void)
{
 uint32_t res_status = 1;
 
 /* close the RTP connection */
 if(RTP_Close_Connection() == CONNECT_CLOSED)
 {
   /* Stop the camera */
   BSP_CAMERA_Stop();
   
   /* Hardware Reset of the camera */
   BSP_CAMERA_HwReset();
   
   res_status = 0;
 }
 
 return res_status;
}

/**
  * @brief  Return the RTP state
  * @param  RTP_struct_ptr: pointer to a RTP_HandleTypeDef structure that contains
  *               the information for the RTP.
  * @retval RTP state
  */
RTP_StatusTypeDef RTP_GetState(RTP_HandleTypeDef *RTP_struct_ptr)
{
  return RTP_struct_ptr->State;
}
    
