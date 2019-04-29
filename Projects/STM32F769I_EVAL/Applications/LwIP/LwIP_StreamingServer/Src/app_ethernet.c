/**
  ******************************************************************************
  * @file    LwIP/LwIP_StreamingServer/Src/app_ethernet.c 
  * @author  MCD Application Team
  * @brief   Ethernet specefic module
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
#include "lwip/opt.h"
#include "main.h"
#include "lwip/dhcp.h"
#include "app_ethernet.h"
#include "ethernetif.h"
#ifdef USE_LCD
#include "lcd_log.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Notify the User about the nework interface config status 
  * @param  netif: the network interface
  * @retval None
  */
void User_notification(struct netif *netif) 
{
  if (netif_is_up(netif))
  {
#ifdef USE_LCD
    uint8_t iptxt[34];
    
    sprintf((char*)iptxt, "%s:%d.%d.%d.%d", (char*)"Static IP address", IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/4 + 50, (uint8_t *)iptxt, LEFT_MODE);
    
#endif /* USE_LCD */
    /* Turn On LED1 to indicate ETH and LwIP init success */
    BSP_LED_On(LED_GREEN);
  }
  else
  {  
#ifdef USE_LCD
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/4 + 50, (uint8_t *)"The network cable is not connected", LEFT_MODE);
#endif /* USE_LCD */
    
    /* Turn On LED 3 to indicate ETH and LwIP init error */
    BSP_LED_On(LED_RED);
  } 
}

/**
  * @brief  This function notify user about link status changement.
  * @param  netif: the network interface
  * @retval None
  */
void ethernetif_notify_conn_changed(struct netif *netif)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;

  if(netif_is_link_up(netif))
  {
    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);    

#ifdef USE_LCD        
    uint8_t lcd_log_txt[51];

    sprintf((char*)lcd_log_txt, "%s:%d.%d.%d.%d", (char*)"The network cable is now connected", IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/4 + 70, (uint8_t *)lcd_log_txt, LEFT_MODE);
#endif /* USE_LCD */
    BSP_LED_Off(LED_RED);
    BSP_LED_On(LED_GREEN);
    
    netif_set_addr(netif, &ipaddr , &netmask, &gw);
    
    /* When the netif is fully configured this function must be called. */
    netif_set_up(netif);     
  }
  else
  {
    /*  When the netif link is down this function must be called.*/
    netif_set_down(netif);
    
#ifdef USE_LCD
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/4 + 70, (uint8_t *)"The network cable is not connected", LEFT_MODE);
#else
    BSP_LED_Off(LED_GREEN);
    BSP_LED_On(LED_RED);
#endif /* USE_LCD */    
  }
  /* Clear MFX Interrupt flag */
  BSP_IO_ITClear();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
