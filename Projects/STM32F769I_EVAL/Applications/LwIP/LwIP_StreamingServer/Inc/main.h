/**
  ******************************************************************************
  * @file    LwIP/LwIP_StreamingServer/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f769i_eval.h"
#include "stm32f769i_eval_lcd.h"
#include "stm32f769i_eval_io.h"

/* Jpeg includes component */
#include "jpeglib.h"
#include "encode.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_LCD        /* enable LCD  */  

/*Static IP ADDRESS*/
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   0
#define IP_ADDR3   10
   
/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   0
#define GW_ADDR3   1

/* Genaral configuration for RTSP protocol */   
#define TARGET_IP_ADDRESS    inet_addr("192.168.0.11")      /* Target IP address */
#define RTSP_URL             "rtsp://192.168.0.10"          /* RTSP source URL */
#define RTSP_SERVER_PORT     ";server_port=49152-49153\r\n" /* RTSP server port */
#define RSTP_PORT            554       /* RTSP network port */
#define MAX_NB_CLIENT        2         /* Max Number of clients, default value is 2 */
   
/* Genaral configuration for RTP protocol */   
#define RTP_PORT                    49152
   
/* LCD Frame Buffer address */
#define LCD_FRAME_BUFFER_START_ADRESS   0xC0025800   /* SDRAM start address + 150k octets */
   
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* encode to JPEG image configuration */
#define IMAGE_HEIGHT    120
#define IMAGE_WIDTH     160
#define IMAGE_QUALITY   80 
#define JPEG_BUFFER_SIZE (7 * 1024)
#define RGB_BUFFER_SIZE  (160 * 120 * 3)   
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */  
void Error_Handler(void);
  
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


