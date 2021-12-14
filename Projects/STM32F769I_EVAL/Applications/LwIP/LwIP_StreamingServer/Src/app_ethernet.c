/**
  ******************************************************************************
  * @file    LwIP/LwIP_StreamingServer/Src/app_ethernet.c 
  * @author  MCD Application Team
  * @brief   Ethernet specefic module
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

