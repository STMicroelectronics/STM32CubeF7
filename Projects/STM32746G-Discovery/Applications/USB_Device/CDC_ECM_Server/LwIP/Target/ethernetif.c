/**
  ******************************************************************************
  * @file    USB_Device/CDC_ECM_Server/Src/ethernetif.c
  * @author  MCD Application Team
  * @brief   This file implements Ethernet network interface drivers for lwIP
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
#include "stm32f7xx_hal.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include "usbd_cdc_ecm_if.h"
#include "stm32746g_discovery.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Network interface name */
#define IFNAME0 's'
#define IFNAME1 't'

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern USBD_HandleTypeDef  USBD_Device;

/* Private function prototypes -----------------------------------------------*/
u32_t sys_now(void);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
                       Ethernet MSP Routines
*******************************************************************************/

/*******************************************************************************
                       LL Driver Interface ( LwIP stack --> USB)
*******************************************************************************/
/**
  * @brief In this function, the hardware should be initialized.
  * Called from ethernetif_init().
  *
  * @param netif the already initialized lwip network interface structure
  *        for this ethernetif
  */
static void low_level_init(struct netif *netif)
{
  /* set MAC hardware address length */
  netif->hwaddr_len = ETH_HWADDR_LEN;

  /* Set MAC hardware address */
  netif->hwaddr[0] =  CDC_ECM_MAC_ADDR0;
  netif->hwaddr[1] =  CDC_ECM_MAC_ADDR1;
  netif->hwaddr[2] =  CDC_ECM_MAC_ADDR2;
  netif->hwaddr[3] =  CDC_ECM_MAC_ADDR3;
  netif->hwaddr[4] =  CDC_ECM_MAC_ADDR4;
  netif->hwaddr[5] =  CDC_ECM_MAC_ADDR5;

  /* maximum transfer unit */
  netif->mtu = 1500;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
}

/**
  * @brief This function should do the actual transmission of the packet. The packet is
  * contained in the pbuf that is passed to the function. This pbuf
  * might be chained.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
  * @return ERR_OK if the packet could be sent
  *         an err_t value if the packet couldn't be sent
  *
  * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
  *       strange results. You might consider waiting for space in the DMA queue
  *       to become available since the stack doesn't retry to send a packet
  *       dropped because of memory failure (except for the TCP timers).
  */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
  struct pbuf *q;
  uint8_t *pdata;
  uint32_t Trials = CDC_ECM_MAX_TX_WAIT_TRIALS;
  USBD_CDC_ECM_HandleTypeDef *hcdc = (USBD_CDC_ECM_HandleTypeDef*) (USBD_Device.pClassData);

  /* Check if the TX State is not busy */
  while ((hcdc->TxState != 0U) && (Trials > 0U))
  {
    Trials--;
  }

  /* If no success getting the TX state ready, return error */
  if (Trials == 0U)
  {
    return (err_t)ERR_USE;
  }

  pdata = hcdc->TxBuffer;
  hcdc->TxLength = 0;

  for(q = p; q != NULL; q = q->next)
  {
    (void)memcpy(pdata, q->payload, q->len);
    pdata += q->len;
    hcdc->TxLength += q->len;
  }

  /* Set the TX buffer information */
  (void)USBD_CDC_ECM_SetTxBuffer(&USBD_Device, hcdc->TxBuffer, (uint16_t)hcdc->TxLength);

  /* Start transmitting the Tx buffer */
  if(USBD_CDC_ECM_TransmitPacket(&USBD_Device) == (uint8_t)USBD_OK)
  {
    return (err_t)ERR_OK;
  }
  UNUSED(netif);
  return (err_t)ERR_USE;
}

/**
  * @brief Should allocate a pbuf and transfer the bytes of the incoming
  *        packet from the interface into the pbuf.
  *
  * @param pnetif the lwip network interface structure for this ethernetif
  * @return a pbuf filled with the received packet (including MAC header)
  *         NULL on memory error
  */
static struct pbuf *low_level_input(struct netif *pnetif)
{
  struct pbuf *p = NULL;

  USBD_CDC_ECM_HandleTypeDef *hcdc = (USBD_CDC_ECM_HandleTypeDef*) (USBD_Device.pClassData);
  uint32_t temp = hcdc->RxState;

  /* Get the length of the current buffer */
  if ((hcdc->RxLength > 0U) && (temp == 1U))
  {
    /* Allocate a pbuf chain of pbufs from the Lwip buffer pool */
    p = pbuf_alloc(PBUF_RAW, (uint16_t)hcdc->RxLength, PBUF_POOL);
  }

  /* Check that allocation was done correctly */
  if (p != NULL)
  {
    /* Copy the buffer data in the allocated buffer */
    (void)memcpy(p->payload, (uint8_t *)hcdc->RxBuffer, (uint16_t)hcdc->RxLength);
    p->len = (uint16_t)hcdc->RxLength;
  }
  UNUSED(pnetif);
  return p;
}

/**
  * @brief This function should be called when a packet is ready to be read
  * from the interface. It uses the function low_level_input() that
  * should handle the actual reception of bytes from the network
  * interface. Then the type of the received packet is determined and
  * the appropriate input function is called.
  *
  * @param pnetif the lwip network interface structure for this ethernetif
  */
void ethernetif_input(struct netif *pnetif)
{
  err_t err;
  struct pbuf *p;

  USBD_CDC_ECM_HandleTypeDef *hcdc = (USBD_CDC_ECM_HandleTypeDef*) (USBD_Device.pClassData);

  /* move received packet into a new pbuf */
  p = low_level_input(pnetif);

  /* no packet could be read, silently ignore this */
  if (p == NULL)
  {
    return;
  }

  /* entry point to the LwIP stack */
  err = pnetif->input(p, pnetif);

  if (err != (err_t)ERR_OK)
  {
     LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
    (void)pbuf_free(p);
    p = NULL;
  }

  /* Reset the Received buffer length to zero for next transfer */
  hcdc->RxLength = 0U;
  hcdc->RxState = 0U;

  /* Prepare Out endpoint to receive next packet in current/new frame */
  (void)USBD_LL_PrepareReceive(&USBD_Device, CDC_ECM_OUT_EP,
                               (uint8_t*)(hcdc->RxBuffer), hcdc->MaxPcktLen);

  /* Free the allocated buffer :
     The allocated buffer is freed by low layer ethernet functions.
  */
}

/**
  * @brief Should be called at the beginning of the program to set up the
  * network interface. It calls the function low_level_init() to do the
  * actual setup of the hardware.
  *
  * This function should be passed as a parameter to netif_add().
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return ERR_OK if the loopif is initialized
  *         ERR_MEM if private data couldn't be allocated
  *         any other err_t on error
  */
err_t ethernetif_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;

  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init(netif);

  return (err_t)ERR_OK;
}

/**
  * @brief  Returns the current time in milliseconds
  *         when LWIP_TIMERS == 1 and NO_SYS == 1
  * @param  None
  * @retval Current Time value
  */
u32_t sys_now(void)
{
  return HAL_GetTick();
}

/**
  * @brief  Link callback function, this function is called on change of link status
  *         to update low level driver configuration.
* @param  netif: The network interface
  * @retval None
  */
void ethernetif_update_config(struct netif *netif)
{
  if(netif_is_link_up(netif) == 1U)
  {
    BSP_LED_On(LED1);
  }
  else
  {
    BSP_LED_Off(LED1);
  }

  ethernetif_notify_conn_changed(netif);
}

/**
  * @brief  This function notify user about link status changement.
  * @param  netif: the network interface
  * @retval None
  */
__weak void ethernetif_notify_conn_changed(struct netif *netif)
{
  /* NOTE : This is function clould be implemented in user file
            when the callback is needed,
  */
  UNUSED(netif);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
