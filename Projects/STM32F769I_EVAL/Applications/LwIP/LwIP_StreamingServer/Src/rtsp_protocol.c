/**
  ******************************************************************************
  * @file    LwIP/LwIP_StreamingServer/Src/rtp_protocol.c 
  * @author  MCD Application Team
  * @brief   RTSP server module
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
#include "rtsp_protocol.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
#define SIZE_RESP_DATA        350       /* Size of response data buffer */
#define RTSP_REQ_HEADER_SIZE  200       /* require header size */
#define MAX_SIZE_DATA_TRANS   300       /* Size of data to send */

/* Private variables ---------------------------------------------------------*/
RTSP_HandleTypeDef rtsp_struct;         /* RTSP structure */
static char* rtsp_version = "RTSP/1.0"; /* RTSP version */

/* Imported variables ---------------------------------------------------------*/
extern RTP_HandleTypeDef RTP_struct;
extern osThreadId Thr_Send_Sem;

/* Private functions ---------------------------------------------------------*/
static void rtsp_thread(void * arg);
static int32_t RTSP_RequireHeader (char* req_header, char* response);
static char* RTSP_ResponseURL(char* require_hdr);
static uint32_t RTSP_TransportCheck(char* request, char* transport, char* response);
static uint32_t RTSP_SessionCheck(char* request, char * response);
static void RTSP_ResponseOptions(char * req,char * resp);
static void RTSP_ResponseSetup(char * req, char* resp);
static void RTSP_ResponsePlay(char* req, char* resp);
static void RTSP_ResponseTeardown (char* req,char* resp);
static void RTSP_ResponseDescribe(char* req, char* resp);
static void RTSP_NotValidMethod(char* req,char* response);
static void RTSP_SetSequence(char* req, char* resp);


/**
  * @brief  Initialize the RTSP server  
  * @param  None
  * @retval None
  */
void RTSP_Init(void)
{ 
  /* Reset RTSP Structure */
  memset(&rtsp_struct, 0x00, sizeof(rtsp_struct));
  
  /*  Set RTSP process to IDLE */
  rtsp_struct.State = RTSP_STATE_IDLE;
  
  /*  Start the RTSP thread */
  sys_thread_new("RTSP", rtsp_thread, NULL,
                         DEFAULT_THREAD_STACKSIZE * 2, osPriorityAboveNormal);
}

/**
  * @brief  RTSP thread 
  * @param  None
  * @retval None
  */
static void rtsp_thread(void * arg)
{  
  struct sockaddr_in local;             /* A local member in the network */
  struct sockaddr_in ToAddr;            /* A destination member in the network */
  socklen_t toddrlen = sizeof(ToAddr);  /* Sizeof destination address */
  char reception_buffer[SIZE_RESP_DATA];     /* response data Buffer */
  RTSP_StatusTypeDef previous_state = RTSP_STATE_UNKNOWN;
  RTSP_StatusTypeDef current_state;
  
  LWIP_UNUSED_ARG(arg);

  for(;;)
  {
    /* Get RTSP current state */
    current_state =  RTSP_GetState(&rtsp_struct);
    switch (current_state)
    {
      case RTSP_STATE_IDLE:
        /* Reset the buffer */
        memset(reception_buffer, 0x00, sizeof(reception_buffer));
  
        /* Allocate memory */
        rtsp_struct.rtsp_resp = (char*)malloc(SIZE_RESP_DATA * sizeof(char));
        
        /* Inilialise the session state to Teardown*/
        rtsp_struct.Session_State = SESSION_TEARDOWN;

        rtsp_struct.State = RTSP_STATE_CREATE;
        
        /* Update previous state */
        previous_state = RTSP_STATE_IDLE;
        
        break;
        
      case RTSP_STATE_CREATE:
        /* Check the previous state */
        if(previous_state == RTSP_STATE_IDLE)
        {
          /* Create new socket */
          rtsp_struct.sock_id = socket(AF_INET , SOCK_STREAM ,0);
  
          if (rtsp_struct.sock_id >= 0) 
          {
            /* Reset local peripheral */
             memset(&local, 0x00, sizeof(local));
      
            /* Configure local address */
            local.sin_family      = AF_INET;
            local.sin_port        = PP_HTONS(RSTP_PORT);
            local.sin_addr.s_addr = PP_HTONL(INADDR_ANY);
            
            /* Bind to local address */
            if (bind(rtsp_struct.sock_id, 
                                (struct sockaddr *)&local, sizeof(local)) == 0) 
            {    
              /* The Bind is OK, so the Session is available */       
              rtsp_struct.Session_State = SESSION_ALIVE;  
        
              /* Reset local address */
              memset(&ToAddr, 0x00, sizeof(ToAddr));
        
              /* Configure local address */
              ToAddr.sin_family      = AF_INET;
              ToAddr.sin_port        = PP_HTONS(INADDR_ANY);
              ToAddr.sin_addr.s_addr = TARGET_IP_ADDRESS;
      
              /* List of connection */
              listen(rtsp_struct.sock_id, MAX_NB_CLIENT);
    
              /* Accept a connection */
              rtsp_struct.conn_sock_id = accept(rtsp_struct.sock_id,
                                              (struct sockaddr*)&ToAddr, &toddrlen);
              
              rtsp_struct.State = RTSP_STATE_START;
            }
          }
          else
          {
            /* Bind failed to local address*/
             rtsp_struct.State = RTSP_STATE_ERROR;            
          }
        }
        else
        {
          /* Failed to create socket */
           rtsp_struct.State = RTSP_STATE_ERROR;
        }
        /* Update previous state */
        previous_state = RTSP_STATE_CREATE;

        break;
      
      case RTSP_STATE_START:
        /* Check session status */
        if(rtsp_struct.Session_State == SESSION_ALIVE)
        {
          /* Receive a request */
          if(recv(rtsp_struct.conn_sock_id, reception_buffer, SIZE_RESP_DATA,0) > 0)
          {    
            /* Reset a response */
            memset(rtsp_struct.rtsp_resp, 0x00, sizeof(&rtsp_struct.rtsp_resp));
              
            /* options response */
            RTSP_ResponseOptions(reception_buffer, rtsp_struct.rtsp_resp);
  
            /* describe response */
            RTSP_ResponseDescribe(reception_buffer, rtsp_struct.rtsp_resp);
              
            /* RTSP received SETUP message */
            if (strncmp(reception_buffer, RTSP_MSG_SETUP, 5) == 0) 
            { 
              if(previous_state == RTSP_STATE_PLAY)
              {
                /* response is not valid */
                rtsp_struct.State = RTSP_STATE_INVALID;
              }
              else
              {
                rtsp_struct.State = RTSP_STATE_SETUP;
              }
            }
            /* RTSP received PLAY message */
            else if(strncmp(reception_buffer, RTSP_MSG_PLAY, 4) == 0)
            {
              if(previous_state == RTSP_STATE_SETUP)
              {
                rtsp_struct.State = RTSP_STATE_PLAY;
              }
              else
              {
                /* response is not valid */
                rtsp_struct.State = RTSP_STATE_INVALID;
              }
            }
            /* RTSP received teardown message */
            else if(strncmp(reception_buffer, RTSP_MSG_TEARDOWN, 8) == 0)
            {
              rtsp_struct.State = RTSP_STATE_TEARDOWN;
            }
          }
        }
        else
        {
          /* Set current RTSP status to STOPPED */
          rtsp_struct.State = RTSP_STATE_STOPPED;
        }
        /* Update previous state */
        previous_state = RTSP_STATE_START;
        break;
    
      case RTSP_STATE_SETUP:
        /* check the URL */
        rtsp_struct.rtsp_resp = RTSP_ResponseURL(reception_buffer);

        if (!strstr(rtsp_struct.rtsp_resp, RTSP_MSG_404))
        {
          /* setup response*/
          RTSP_ResponseSetup(reception_buffer, rtsp_struct.rtsp_resp);
        }
        /* Reset reception_buffer */
        memset(reception_buffer ,0x00, sizeof(reception_buffer));
        
        /* Set current RTSP status to START */
        rtsp_struct.State = RTSP_STATE_START;
        
        /* Update previous state */
        previous_state = RTSP_STATE_SETUP;
        break;
        
      case RTSP_STATE_PLAY:
 
        /* check the URL*/
        rtsp_struct.rtsp_resp = RTSP_ResponseURL(reception_buffer);
      
        if (!strstr(rtsp_struct.rtsp_resp, RTSP_MSG_404))
        {
          /* play response */
          RTSP_ResponsePlay(reception_buffer, rtsp_struct.rtsp_resp);
        }
        /* Reset reception_buffer */
        memset(reception_buffer ,0x00, sizeof(reception_buffer));
        
        /* Set current RTSP status to START */
        rtsp_struct.State = RTSP_STATE_START;
        
        /* Update previous state */
        previous_state = RTSP_STATE_PLAY;
        break;
        
       case RTSP_STATE_TEARDOWN:
         /*check the URL*/
         rtsp_struct.rtsp_resp = RTSP_ResponseURL(reception_buffer);
      
         if (!strstr(rtsp_struct.rtsp_resp, RTSP_MSG_404))
         {
           /* teardown response*/
           RTSP_ResponseTeardown(reception_buffer, rtsp_struct.rtsp_resp);
      
           rtsp_struct.State = RTSP_STATE_STOPPED;
        }
        /* Reset reception_buffer */
        memset(reception_buffer ,0x00, sizeof(reception_buffer));
        
        /* Update previous state */
        previous_state = RTSP_STATE_TEARDOWN;
        break;
        
      case RTSP_STATE_INVALID:
        /* send message: No valid method */
        RTSP_NotValidMethod(reception_buffer ,rtsp_struct.rtsp_resp);
        
        /* Set RTSP status to START */
        rtsp_struct.State = RTSP_STATE_START;
        
        /* Update previous state */
        previous_state = RTSP_STATE_INVALID;
        break;
        
      case RTSP_STATE_ERROR:
      case RTSP_STATE_STOPPED:
        /* RTSP session is closed by the client or an error occurred */
        RTSP_Stop();
        
        /* Set current status to IDLE */
        rtsp_struct.State = RTSP_STATE_IDLE;
        
        /* Update previous state */
        previous_state = RTSP_STATE_STOPPED;
        break;
        
      default:
        /* Something is wrong, go to state ERROR */
        rtsp_struct.State = RTSP_STATE_ERROR;
        previous_state = RTSP_STATE_UNKNOWN;
        break;
    }
  }
}
                   
/**
  * @brief  Require Header Response 
  * @param  received request
  * @param  response
  * @retval 1= require header supported, 0= require header unsupported
  */                   
static int32_t RTSP_RequireHeader (char* req_header, char* response)
{
  int32_t status = -1;
  uint32_t i = 0;
   
  /* Allocate memory */
  rtsp_struct.rtsp_req_header = (char*)malloc(RTSP_REQ_HEADER_SIZE * sizeof(char));
  
  /* search the require header */
  rtsp_struct.rtsp_req_header = strstr(req_header, "\nRequire");
   
  if (rtsp_struct.rtsp_req_header != NULL)
  { 
    /* the require header doesn't contain the implicit play */
    if(!strstr(req_header, "implicit-play"))
    {
      /* reset the response */
      memset(response , 0x00,sizeof(&response));
       
      /* set the 551 message */
      sprintf(response,"%8.8s%27.27s", rtsp_version, RTSP_MSG_OPTION_NOT_SUPPORTED);
       
      /* set the sequence number */
      RTSP_SetSequence(req_header, response);
       
      /* the transport is unsupported */
      sprintf(response + strlen(response), "%s", RTSP_MSG_TRANSPORT_TYPE_UNSUPPORTED); 
       
      /* copy the require type ( different of implicit-play) */
      while(strncmp(rtsp_struct.rtsp_req_header + (i + 9), "\r", 1))
      {
        sprintf(response + strlen(response), "%1.1s",
                           rtsp_struct.rtsp_req_header + (i + 9));
        i++;
      }
      
      /* copy the \n */
      strncat(response, "\r\n",1);
      
      /* send the response of the unsupported required */
      send(rtsp_struct.conn_sock_id, response, strlen(response), 0);
      
             
      /* Update RTSP status */
      status = 0;
    }
    
    /* Now free the memory */
    if(rtsp_struct.rtsp_req_header != NULL)
    {
      free(rtsp_struct.rtsp_req_header);
    }
  }
  
  return status;
}

/**
  * @brief  Parse URL 
  * @param  received request
  * @retval response
  */
static char* RTSP_ResponseURL (char* require_hdr)
{  
  /* reset the response */
  memset(rtsp_struct.rtsp_resp, 0x00, sizeof(&rtsp_struct.rtsp_resp));
         
  /* copy the rtsp version to the response */
  sprintf(rtsp_struct.rtsp_resp, "%8.8s", rtsp_version);
  
  /* if the url is true  or "*", response with OK */
  if(strstr(require_hdr, RTSP_URL) || strstr(require_hdr,"*"))
  {  
    sprintf(rtsp_struct.rtsp_resp + strlen(rtsp_struct.rtsp_resp), "%9.9s", RTSP_MSG_OK);
  }
  
  /* if the url is false , response : not found */
  else
  {
    sprintf(rtsp_struct.rtsp_resp + strlen(rtsp_struct.rtsp_resp),
                                              "%16.16s", RTSP_MSG_NOT_FOUND);
  }
  
  return rtsp_struct.rtsp_resp;
}

/**
  * @brief  Options Response 
  * @param  received request
  * @param  response
  * @retval None
  */
static void RTSP_ResponseOptions(char * req,char * resp)
{
  /* options request */
  if (strncmp(req, RTSP_MSG_OPTIONS, 7) == 0)
  {   
    /* require header supported */
    if (RTSP_RequireHeader(req, resp))
    {   
      /* add the response of url (check the url)*/
      resp = RTSP_ResponseURL(req);
    
      /* set the sequence number */
      RTSP_SetSequence(req, resp);
      
      /* add the public message */
      strcat(resp, RTSP_MSG_PUBLIC);
    
      /* send the options response */
      send(rtsp_struct.conn_sock_id, resp, strlen(resp),0);
    }
  }
}

/**
  * @brief  Describe Response 
  * @param  received request
  * @param  response
  * @retval None
  */
static void RTSP_ResponseDescribe(char* req, char* resp)
{  
  if (strncmp(req, "DESCRIBE", 8) == 0)
  {  
    /* Add the response of url (check the url) */
    resp=RTSP_ResponseURL(req);
    
    /* set the sequence number */
    RTSP_SetSequence(req,resp);
      
    /*set contents messages */
    sprintf(resp + strlen(resp), "%s" , RTSP_MSG_CONTENTS);
           
    /*set SDP attributes */
    sprintf(resp + strlen(resp), "%s", RTSP_MSG_SDP_ATTRIBUTES);
      
    /* set the \r\n */
    strcat(resp, "\r\n");
      
    /* send the describe response */
    send(rtsp_struct.conn_sock_id, resp, strlen(resp), 0);
  }
}

/**
  * @brief  check the session identifier  
  * @param  received request
  * @param  response
  * @retval 0= session not found, 1=session found
  */
static uint32_t RTSP_SessionCheck(char* request, char * response)
{
  if (strstr(request, RTSP_MSG_SESSION_ID))
  {
    return 1;
  }
  else
  {
    /* reset the response */
    memset(response, 0x00, sizeof(&response));
    
    /* set the message 454 */
    sprintf(response, "%8.8s %23.23s", rtsp_version, RTSP_MSG_SESSION_NOT_FOUND);
    
    /* set the sequence number */
    RTSP_SetSequence(request, response);
    
    /* set the \n */
    strcat(response, "\r\n");
    
    /* send the response of unsupported transport */
    send(rtsp_struct.conn_sock_id, response, strlen(response), 0);
    
    /* the transport is unsupported */
    return 0;
  }
}

/**
  * @brief  check the transport  
  * @param  received request
  * @param  transport line
  * @param  response
  * @retval 0= transport unsupported, 1=transport supported
  */
static uint32_t RTSP_TransportCheck(char* request, char* transport, char* response)
{  
  /* if the transport request is TCP or the architecture request is multicast */
  if (strstr(transport, RTSP_MSG_TRANSPORT_IS_TCP) ||
      strstr (transport, RTSP_MSG_REQUEST_IS_MULTICAST))
  {  
    /* reset the response */
    memset(response, 0x00, sizeof(&response));
    
    /* set the message 461 */
    sprintf(response,"%8.8s %26.26s", rtsp_version, RTSP_MSG_TRANSPORT_UNSUPPORTED);
    
    /* set the sequence number */
    RTSP_SetSequence(request, response);
    
    /* set the \n */
    strcat(response, "\r\n");
    
    /* send the response of unsupported transport */
    send(rtsp_struct.conn_sock_id, response, strlen(response),0);
    
    /* the transport is unsupported */
    return 0;
  }
  
  /* the transport is supported */
  return 1;
}
/**
  * @brief  Setup Response 
  * @param  received request
  * @param  response
  * @retval None
  */
static void RTSP_ResponseSetup(char * req, char* resp)
{   
  uint32_t i = 0;
  
  rtsp_struct.rtsp_transp_type = (char*)malloc(RTSP_REQ_HEADER_SIZE * sizeof(char));
    
  /* require header supported */
  if (RTSP_RequireHeader(req, resp))
  {    
    /* set the sequence number */
    RTSP_SetSequence(req, resp);
      
    /* set the session header */
    sprintf(resp + strlen(resp), "%21.21s", "Session: 1234567890 \r\n");

    /* search the transport line */
    rtsp_struct.rtsp_transp_type = strstr(req, "Transport");
    
    /* RTSP transport supported*/
    if (RTSP_TransportCheck(req, rtsp_struct.rtsp_transp_type, resp))
    {  
      /* copy the received line of transport */
      while(strncmp(rtsp_struct.rtsp_transp_type + i, "\r", 1))
      {
        sprintf(resp + strlen(resp), "%1.1s", rtsp_struct.rtsp_transp_type + i);
        i++;
      }
      
      /*add the server port */
      strcat(resp, RTSP_SERVER_PORT);
      
      /* set the \n */
      strcat(resp, "\r\n");

      /* send the setup response */
      send(rtsp_struct.conn_sock_id, resp, strlen(resp), 0);
    }
  }   
}

/**
  * @brief  Play Response 
  * @param  Request received
  * @param  Response
  * @retval None
  */
static void RTSP_ResponsePlay (char* req, char* resp)
{
  /* require header supported */
  if (RTSP_RequireHeader(req, resp) && RTSP_SessionCheck(req,resp))
  {
    /* set the sequence number */
    RTSP_SetSequence(req, resp);
        
    /* set the session header */
    sprintf(resp+strlen(resp), "%23.23s", "Session: 1234567890\r\n\r\n");

    /* send the response */
    send(rtsp_struct.conn_sock_id, resp, strlen(resp), 0);  
    
    /* Calling the RTP Thread to send video packets */ 
    RTP_Init();
  }
}

/**
  * @brief  Teardown Response 
  * @param  received request
  * @param  response
  * @retval None
  */
static void RTSP_ResponseTeardown (char* req,char* resp)
{    
    /* require header supported */
    if (RTSP_RequireHeader (req, resp) && RTSP_SessionCheck(req,resp))
    {  
      /* set the sequence header */
      RTSP_SetSequence(req,resp);
    
      /* Connection header : closed */
      sprintf(resp+strlen(resp), "%s", RTSP_MSG_CONN_CLOSE);
    
      /* send the response */
      send(rtsp_struct.conn_sock_id, resp, strlen(resp), 0);
    }
}

/**
  * @brief  Not Valid Response 
  * @param  received request
  * @param  response
  * @retval None
  */
static void RTSP_NotValidMethod(char* req,char* response)
{
   /* reset response */
   memset(response, 0x00, sizeof(&response));
   
   /* copy version to response */
   sprintf(response, "%8.8s", rtsp_version);
   
   /* set the 455 message */
   sprintf(response + strlen(response), "%36.36s", RTSP_MSG_METHOD_NOT_VALID);
   
   /* set the sequence number */
   RTSP_SetSequence(req, response);
   
   /* copy the '\n' */
   strcat(response, "\r\n");
   
   /* send the "not valid method" response */
   send(rtsp_struct.conn_sock_id, response, strlen(response), 0); 
}
  
/**
  * @brief  Set Sequence Number 
  * @param  received request
  * @param  response
  * @retval None
  */
static void RTSP_SetSequence(char* req, char* resp)
{  
  uint32_t i = 0; 
  
  /* search the sequence number */
  rtsp_struct.rtsp_seq = strstr(req, RTSP_MSG_SEQUENCE_NUMBER);
  
  if(rtsp_struct.rtsp_seq)
  {
    /* copy the sequence number to response*/
    while (strncmp(rtsp_struct.rtsp_seq + i, "\r", 1))
    {
      sprintf(resp+strlen(resp), "%1.1s", rtsp_struct.rtsp_seq + i);
      i++;
    }
  
    /* copy the '\n' */
    strcat(resp, "\r\n");
  }
}

/**
  * @brief  Close RTSP connection
  * @param  none
  * @retval none
  */                          
void RTSP_Stop(void)
{
  /* Close socket */
  close(rtsp_struct.conn_sock_id);      
        
  shutdown(rtsp_struct.conn_sock_id,0);
      
  if(RTP_Stop() == 0)
  {
    /* close the socket */
    if(!close(rtsp_struct.sock_id))
    {  
        /* Reset the offset of the RTP/JPEG header */
        RTP_struct.Offset = 0;
     
        /* Update session state */
        rtsp_struct.Session_State = SESSION_TEARDOWN;
     
        /* Terminate Thr_Send_Sem thread */
        osThreadTerminate(Thr_Send_Sem);
    }
  }
      
  if(rtsp_struct.rtsp_resp != NULL)
  {
    /* Now free the memory */ 
    free(rtsp_struct.rtsp_resp);
  }
}

/**
  * @brief  Return the RTSP state
  * @param  RTSP_struct_ptr: pointer to a RTSP_HandleTypeDef structure that contains
  *               the information for the RTSP.
  * @retval RTSP state
  */
RTSP_StatusTypeDef RTSP_GetState(RTSP_HandleTypeDef *RTSP_struct_ptr)
{
  return RTSP_struct_ptr->State;
}

