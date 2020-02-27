/**
  ******************************************************************************
  * @file    USB_Device/CDC_RNDIS_Server/USB_Device/App/httpd_cg_ssi.c
  * @author  MCD Application Team
  * @brief   Webserver SSI and CGI handlers
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
#include "lwip/debug.h"
#include "lwip/tcp.h"
#include "lwip/apps/httpd.h"
#include "http_cgi_ssi.h"

#include <string.h>
#include <stdlib.h>

extern __IO uint32_t LEDTimer;

/* Array of tags for the SSI handler */
#define numSSItags 1
char const *theSSItags[numSSItags] = {"tag1"};

void myCGIinit(void);
u16_t mySSIHandler(int iIndex, char *pcInsert, int iInsertLen);
void mySSIinit(void);

/* CGI handler for LED control */
const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

/* Html request for "/leds.cgi" will start LEDS_CGI_Handler */
const tCGI LEDS_CGI={"/leds.cgi", LEDS_CGI_Handler};

/* Cgi call table, only one CGI used */
tCGI CGI_TAB[1];

/* Initialize the CGI handlers */
void myCGIinit(void)
{
  /* Add LED control CGI to the table */
	CGI_TAB[0] = LEDS_CGI;

  /* Give the table to the HTTP server */
  http_set_cgi_handlers(CGI_TAB, 1);
} /* myCGIinit */

/**
  * @brief  CGI handler for LEDs control
  */
const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  uint32_t i;

  /* We have only one SSI handler iIndex = 0 */
  if (iIndex==0)
  {
    /* All LEDs off, or when only one LED used, slower toggling timer value */
    /* BSP_LED_Off(LED1); */
    LEDTimer = LED_TIMER_LONG;

    /* Check cgi parameter : example GET /leds.cgi?led=2&led=4 */
    for (i=0; i<(uint32_t)iNumParams; i++)
    {
      /* check parameter "led" */
      if (strcmp(pcParam[i] , "led")==0)
      {
        /* Switch LED1 ON if 1, or when only one LED used, faster toggling timer value */
        if(strcmp(pcValue[i], "1") ==0)
        {
          /* BSP_LED_On(LED1); */
          LEDTimer = LED_TIMER_SHORT;
        }

      }
    }
  }
  /* uri to send after cgi call*/
  return "/STM32F7xxLED.html";
}

/**** SSI handler ****/

u16_t mySSIHandler(int iIndex, char *pcInsert, int iInsertLen)
{
  /* We have only one SSI handler iIndex = 0 */
  if (iIndex == 0)
  {
    if (BSP_PB_GetState(BUTTON_TAMPER) == 1U)
    {
      char myStr1[] = " BUTTON PRESSED"; /* string to be displayed on web page */

      /* Copy the string to be displayed to pcInsert */
      (void) strcpy(pcInsert, myStr1);

      /* Return number of characters that need to be inserted in html */
      return (u16_t) strlen(myStr1);

    }
    else
    {
      char myStr2[] = " BUTTON NOT PRESSED"; /* string to be displayed on web page */

      /* Copy string to be displayed */
      (void)strcpy(pcInsert, myStr2);

      /* Return number of characters that need to be inserted in html */
      return (u16_t) strlen(myStr2);
    }

  }
  UNUSED(iInsertLen);
  return 0;
} /* mySSIHandler */

/**** Initialize SSI handlers ****/
void mySSIinit(void)
{
  /* Configure SSI handler function */
  /* theSSItags is an array of SSI tag strings to search for in SSI-enabled files */
   http_set_ssi_handler(mySSIHandler, (char const **)theSSItags, numSSItags);
} /* mySSIinit */

/**
  * @brief  Http webserver Init
  */
void http_server_init(void)
{
  /* Httpd Init */
  httpd_init();

  /* configure CGI handlers (LEDs control CGI) */
  myCGIinit();

    /* configure SSI handlers */
  mySSIinit();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
