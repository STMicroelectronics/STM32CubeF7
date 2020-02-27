/**
  ******************************************************************************
  * @file    USB_Device/CDC_RNDIS_Server/USB_Device/App/usbd_cdc_rndis_interface.c
  * @author  MCD Application Team
  * @brief   Source file for USBD CDC_RNDIS interface
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ethernetif.h"
#include "http_cgi_ssi.h"
#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"




/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4
#endif
__ALIGN_BEGIN uint8_t UserRxBuffer[CDC_RNDIS_ETH_MAX_SEGSZE + 100] __ALIGN_END; /* Received Data over USB are stored in this buffer */

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4
#endif
__ALIGN_BEGIN static uint8_t UserTxBuffer[CDC_RNDIS_ETH_MAX_SEGSZE + 100] __ALIGN_END; /* Received Data over CDC_RNDIS (CDC_RNDIS interface) are stored in this buffer */

static uint8_t CDC_RNDISInitialized = 0U;

/* USB handler declaration */
extern USBD_HandleTypeDef  USBD_Device;
extern struct netif gnetif;

/* Private function prototypes -----------------------------------------------*/
static int8_t CDC_RNDIS_Itf_Init(void);
static int8_t CDC_RNDIS_Itf_DeInit(void);
static int8_t CDC_RNDIS_Itf_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length);
static int8_t CDC_RNDIS_Itf_Receive(uint8_t *pbuf, uint32_t *Len);
static int8_t CDC_RNDIS_Itf_TransmitCplt(uint8_t *Buf, uint32_t *Len, uint8_t epnum);
static int8_t CDC_RNDIS_Itf_Process(USBD_HandleTypeDef *pdev);

static void Netif_Config(void);
static void Netif_User_notification(struct netif *netif);

USBD_CDC_RNDIS_ItfTypeDef USBD_CDC_RNDIS_fops =
{
  CDC_RNDIS_Itf_Init,
  CDC_RNDIS_Itf_DeInit,
  CDC_RNDIS_Itf_Control,
  CDC_RNDIS_Itf_Receive,
  CDC_RNDIS_Itf_TransmitCplt,
  CDC_RNDIS_Itf_Process,
  (uint8_t *)CDC_RNDIS_MAC_STR_DESC,
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  CDC_RNDIS_Itf_Init
  *         Initializes the CDC_RNDIS media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_RNDIS_Itf_Init(void)
{
  if (CDC_RNDISInitialized == 0U)
  {
    /* Initialize the LwIP stack */
    lwip_init();

    /* Configure the Network interface */
    Netif_Config();

    /* Http webserver Init */
    http_server_init();

    CDC_RNDISInitialized = 1U;
  }

  /* Notify user about the network interface config */
  Netif_User_notification(&gnetif);

  /* Set Application Buffers */
  (void)USBD_CDC_RNDIS_SetTxBuffer(&USBD_Device, UserTxBuffer, 0U);
  (void)USBD_CDC_RNDIS_SetRxBuffer(&USBD_Device, UserRxBuffer);

  return (0);
}

/**
  * @brief  CDC_RNDIS_Itf_DeInit
  *         DeInitializes the CDC_RNDIS media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_RNDIS_Itf_DeInit(void)
{
  USBD_CDC_RNDIS_HandleTypeDef *hcdc_cdc_rndis = (USBD_CDC_RNDIS_HandleTypeDef *)(USBD_Device.pClassData);

  /* Notify application layer that link is down */
  hcdc_cdc_rndis->LinkStatus = 0U;

  return (0);
}

/**
  * @brief  CDC_RNDIS_Itf_Control
  *         Manage the CDC_RNDIS class requests
  * @param  Cmd: Command code
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_RNDIS_Itf_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length)
{
  USBD_CDC_RNDIS_HandleTypeDef *hcdc_cdc_rndis = (USBD_CDC_RNDIS_HandleTypeDef *)(USBD_Device.pClassData);

  switch (cmd)
  {
    case CDC_RNDIS_SEND_ENCAPSULATED_COMMAND:
      /* Add your code here */
      break;

    case CDC_RNDIS_GET_ENCAPSULATED_RESPONSE:
      /* Check if this is the first time we enter */
      if (hcdc_cdc_rndis->LinkStatus == 0U)
      {
        /* Setup the Link up at LwIP level */
        netif_set_link_up(&gnetif);
        hcdc_cdc_rndis->LinkStatus = 1U;
      }
      /* Add your code here */
      break;

    default:
      /* Add your code here */
      break;
  }
  UNUSED(length);
  UNUSED(pbuf);
  return (0);
}

/**
  * @brief  Setup the network interface
  * @param  None
  * @retval None
  */
static void Netif_Config(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;

  IP_ADDR4(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP_ADDR4(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
  IP_ADDR4(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

  /* add the network interface */
  (void)netif_add(&gnetif, &ipaddr, &netmask, &gw,
                  NULL, &ethernetif_init, &ethernet_input);

  /*  Registers the default network interface */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif) == 1U)
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }

  /* Set the link callback function, this function is called on change of link status*/
  netif_set_link_callback(&gnetif, ethernetif_update_config);
}


/**
  * @brief  CDC_RNDIS_Itf_Receive
  *         Data received over USB OUT endpoint are sent over CDC_RNDIS interface
  *         through this function.
  * @param  Buf: Buffer of data to be transmitted
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_RNDIS_Itf_Receive(uint8_t *Buf, uint32_t *Len)
{
  /* Get the CDC_RNDIS handler pointer */
  USBD_CDC_RNDIS_HandleTypeDef *hcdc_cdc_rndis = (USBD_CDC_RNDIS_HandleTypeDef *)(USBD_Device.pClassData);

  /* Call Eth buffer processing */
  hcdc_cdc_rndis->RxState = 1U;

  UNUSED(Buf);
  UNUSED(Len);

  return (0);
}


/**
  * @brief  CDC_RNDIS_Itf_TransmitCplt
  *         Data transmited callback
  *
  *         @note
  *         This function is IN transfer complete callback used to inform user that
  *         the submitted Data is successfully sent over USB.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_RNDIS_Itf_TransmitCplt(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
  UNUSED(Buf);
  UNUSED(Len);
  UNUSED(epnum);

  return (0);
}

/**
  * @brief  CDC_RNDIS_Itf_Process
  *         Data received over USB OUT endpoint are sent over CDC_RNDIS interface
  *         through this function.
  * @param  pdef: pointer to the USB Device Handle
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_RNDIS_Itf_Process(USBD_HandleTypeDef *pdev)
{
  /* Get the CDC_RNDIS handler pointer */
  USBD_CDC_RNDIS_HandleTypeDef *hcdc_cdc_rndis = (USBD_CDC_RNDIS_HandleTypeDef *)(pdev->pClassData);

  if ((hcdc_cdc_rndis != NULL) && (hcdc_cdc_rndis->LinkStatus != 0U))
  {
    /* Read a received packet from the Ethernet buffers and send it
      to the lwIP for handling */
    ethernetif_input(&gnetif);

    /* Handle timeouts */
    sys_check_timeouts();
  }

  return (0);
}

/**
  * @brief  Notify the User about the nework interface config status
  * @param  netif: the network interface
  * @retval None
  */
static void Netif_User_notification(struct netif *netif)
{
  if (netif_is_up(netif) == 1U)
 {
    /* Turn On LED 1 to indicate ETH and LwIP init success */
    BSP_LED_On(LED1);
 }
 else
  {
    /* Turn On LED 2 to indicate ETH and LwIP init error */
    BSP_LED_Off(LED1);
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

  if(netif_is_link_up(netif) == 1U)
  {
    BSP_LED_Off(LED1);
    BSP_LED_On(LED1);

    IP_ADDR4(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP_ADDR4(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
    IP_ADDR4(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

    netif_set_addr(netif, &ipaddr , &netmask, &gw);

    /* When the netif is fully configured this function must be called. */
    netif_set_up(netif);
  }
  else
  {
    /*  When the netif link is down this function must be called. */
    netif_set_down(netif);

    BSP_LED_Off(LED1);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
