/**
  ******************************************************************************
  * @file    BSP/Src/camera.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the camera feature in the
  *          stm32446e_eval driver
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
#include "main.h"
#include <stdio.h>
#include "stm32746g_discovery_audio.h"


/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/**
 *  @brief TS_CommandTypeDef
 *  Define Possible touch screen commands
 */
typedef enum
{
  TS_NO_COMMAND     = 0x00,
  TS_BRIGHTNESS_INC = 0x01,
  TS_BRIGHTNESS_DEC = 0x02,
  TS_CONTRAST_INC   = 0x03,
  TS_CONTRAST_DEC   = 0x04,
  TS_EFFECT         = 0x05,
  TS_RESOLUTION     = 0x06,

} TS_CommandTypeDef;

/* Private define ------------------------------------------------------------*/
#define CAMERA_STATUS_TEXT_POS    30            /* Number of lines from bottom of screen */

#define CAMERA_VGA_RES_X          640
#define CAMERA_VGA_RES_Y          480
#define CAMERA_480x272_RES_X      480
#define CAMERA_480x272_RES_Y      272
#define CAMERA_QVGA_RES_X         320
#define CAMERA_QVGA_RES_Y         240
#define CAMERA_QQVGA_RES_X        160
#define CAMERA_QQVGA_RES_Y        120

#define CAMERA_RES_INDEX_MIN      CAMERA_R160x120
#define CAMERA_RES_INDEX_MAX      CAMERA_R640x480

#define CAMERA_CONTRAST_MIN       CAMERA_CONTRAST_LEVEL0
#define CAMERA_CONTRAST_MAX       CAMERA_CONTRAST_LEVEL4
#define CAMERA_BRIGHTNESS_MIN     CAMERA_BRIGHTNESS_LEVEL0
#define CAMERA_BRIGHTNESS_MAX     CAMERA_BRIGHTNESS_LEVEL4

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static uint32_t          CameraResX;
static uint32_t          CameraResY;
static uint32_t          CameraResIndex;
static volatile uint32_t CameraFrameBufferInitComplete = 0;
static volatile uint32_t CameraFrameBufferInitError = 0;
static volatile uint32_t Camera_AllowDma2dCopyCamFrmBuffToLcdFrmBuff = 0;
static uint32_t          offset_cam = 0;
static uint32_t          offset_lcd = 0;
static uint32_t          display_line_counter = 0;
static uint32_t          special_effect = CAMERA_BLACK_WHITE;        /* No special effect applied */
static uint32_t          color_effect = CAMERA_COLOR_EFFECT_BLUE;
static uint32_t          contrast = CAMERA_CONTRAST_LEVEL2;          /* Mid-level brightness */
static uint32_t          brightness = CAMERA_BRIGHTNESS_LEVEL2;      /* Mid-level contrast */
static DMA2D_HandleTypeDef hdma2d_camera;

/* Private function prototypes -----------------------------------------------*/
static void Camera_SetHint(void);

static void ConvertCameraLineRgb565ToLcdLineARGB8888(void *pSrc,
                                                     void *pDst,
                                                     uint32_t xSize);

static void Camera_FrameBuffer_Init_TransferComplete(DMA2D_HandleTypeDef *hdma2d);

static void Camera_FrameBuffer_Init_TransferError(DMA2D_HandleTypeDef *hdma2d);

static uint32_t CameraFrameBufferRgb565_Init(uint32_t sizeX,
                                             uint32_t sizeY,
                                             uint32_t argb8888_Value);

void BSP_CAMERA_LineEventCallback(void);
void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef *hdma2d);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Camera demo
  * @param  None
  * @retval None
  */
void Camera_demo (void)
{
  uint8_t  status = 0;
  uint32_t exit = 0;
  uint32_t camera_status = CAMERA_OK;
  uint32_t argb8888_Value = 0x00FF00FF; /* = 0xF81F in RGB565 format */
  TS_StateTypeDef  TS_State;
  uint32_t TS_command = TS_RESOLUTION;

  Camera_SetHint();

  status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

  if (status != TS_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)"Touchscreen cannot be initialized", CENTER_MODE);
  }

  /*## Camera Initialization and start capture ############################*/

  CameraResIndex = CAMERA_R160x120;     /* Set QQVGA default resolution */
  CameraResX = CAMERA_QQVGA_RES_X;
  CameraResY = CAMERA_QQVGA_RES_Y;

  /* Infinite loop */
  while (exit == 0)
  {
    if (TS_command == TS_RESOLUTION)  /* A camera change resolution has been asked */
    {
      TS_command = TS_NO_COMMAND;

      BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"Wait for camera initialization...", CENTER_MODE);

      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

      switch (CameraResIndex)
      {
        case CAMERA_R160x120:
          CameraResX = CAMERA_QQVGA_RES_X;
          CameraResY = CAMERA_QQVGA_RES_Y;
          /* Initialize the Camera */
          camera_status = BSP_CAMERA_Init(RESOLUTION_R160x120);
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"       QQVGA camera stream       ", CENTER_MODE);
          break;

        case CAMERA_R320x240:
          CameraResX = CAMERA_QVGA_RES_X;
          CameraResY = CAMERA_QVGA_RES_Y;
          /* Initialize the Camera */
          camera_status = BSP_CAMERA_Init(RESOLUTION_R320x240);
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"       QVGA camera stream        ", CENTER_MODE);
          break;

        case CAMERA_R480x272:
          CameraResX = CAMERA_480x272_RES_X;
          CameraResY = CAMERA_480x272_RES_Y;
          /* Initialize the Camera */
          camera_status = BSP_CAMERA_Init(RESOLUTION_R480x272);
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"      480x272 camera stream      ", CENTER_MODE);
          break;

        case CAMERA_R640x480:
          CameraResX = CAMERA_VGA_RES_X;
          CameraResY = CAMERA_VGA_RES_Y;
          /* Initialize the Camera */
          camera_status = BSP_CAMERA_Init(RESOLUTION_R640x480);
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"        VGA camera stream        ", CENTER_MODE);
          break;

        default :
          break;
      }

      if (camera_status != CAMERA_OK)
      {
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS - 15, (uint8_t *)"        CAMERA INIT ERROR        ", CENTER_MODE);
      }
      else
      {
        offset_cam = 0;
        offset_lcd = 0;
        display_line_counter = 0;

        special_effect = CAMERA_BLACK_WHITE;      /* No special effect applied */
        color_effect = CAMERA_BLACK_WHITE_NORMAL;
        contrast = CAMERA_CONTRAST_LEVEL2;        /* Mid-level brightness */
        brightness = CAMERA_BRIGHTNESS_LEVEL2;    /* Mid-level contrast */

        CameraFrameBufferInitComplete = 0;
        CameraFrameBufferInitError    = 0;

        /* Init or Re-Init Camera frame buffer by using DMA2D engine in mode Register to Memory */
        camera_status = CameraFrameBufferRgb565_Init(CAMERA_VGA_RES_X,
                                                     CAMERA_VGA_RES_Y,
                                                     argb8888_Value);
        ASSERT(camera_status != CAMERA_OK);

        /* Wait end of DMA2D operation of error : via IT callback update */
        while((CameraFrameBufferInitComplete == 0) && (CameraFrameBufferInitError == 0)) {;}

        /* Assert if error : no transfer complete */
        ASSERT(CameraFrameBufferInitComplete != 1);

        /* Start / Restart camera stream */
        BSP_CAMERA_ContinuousStart((uint8_t *)CAMERA_FRAME_BUFFER);

        /* Allow DMA2D copy from Camera frame buffer to LCD Frame buffer location */
        Camera_AllowDma2dCopyCamFrmBuffToLcdFrmBuff = 1;
      }
    }  /* (CameraResIndex != CameraCurrentResIndex) */

    if (camera_status == CAMERA_OK)
    {
      /* Check in polling mode in touch screen the touch status and coordinates */
      /* if touch occurred                                                      */
      BSP_TS_GetState(&TS_State);
      if(TS_State.touchDetected >= 1)
      {
        /* Check touch position to apply brightness or contrast change */
        if (TS_State.touchDetected == 1)
        {
          if ((TS_State.touchY[0] < 30) && (brightness < CAMERA_BRIGHTNESS_MAX))
          {
            TS_command = TS_BRIGHTNESS_INC;
            brightness++;
          }
          else if ((TS_State.touchY[0] > (BSP_LCD_GetYSize() - 30)) && (brightness > CAMERA_BRIGHTNESS_MIN) )
          {
            TS_command = TS_BRIGHTNESS_DEC;
            brightness--;
          }
          else if ((TS_State.touchX[0] < 30) && (contrast > CAMERA_CONTRAST_MIN))
          {
            TS_command = TS_CONTRAST_DEC;
            contrast--;
          }
          else if ((TS_State.touchX[0] > (BSP_LCD_GetXSize() - 30)) && (contrast < CAMERA_CONTRAST_MAX))
          {
            TS_command = TS_CONTRAST_INC;
            contrast++;
          }
          else
          {
            TS_command = TS_NO_COMMAND;
          }
        }

        /* Special effect change */
        if (TS_State.touchDetected == 2)
        {
          TS_command = TS_EFFECT;
          switch (special_effect)
          {
            case CAMERA_BLACK_WHITE :
              if (color_effect < CAMERA_BLACK_WHITE_NORMAL)
              {
                /* From BW effect to another BW effect */
                color_effect++;
              }
              else
              {
                /* From BW or none effect to color effect */
                special_effect = CAMERA_COLOR_EFFECT;
                color_effect = CAMERA_COLOR_EFFECT_BLUE;
              }
              break;

            case CAMERA_COLOR_EFFECT :
              if (color_effect < CAMERA_COLOR_EFFECT_ANTIQUE)
              {
                /* From color effect to another color effect */
                color_effect++;
              }
              else
              {
                /* From color effect to BW effect */
                special_effect = CAMERA_BLACK_WHITE;
                color_effect = CAMERA_BLACK_WHITE_BW;
              }
              break;

            default :
              /* None effect */
              special_effect = CAMERA_BLACK_WHITE;
              color_effect = CAMERA_BLACK_WHITE_NORMAL;
              break;
          }
        }

        if (TS_command == TS_NO_COMMAND)
        {
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"                                 ", CENTER_MODE);
        }

        if ((TS_command == TS_BRIGHTNESS_INC) || (TS_command == TS_BRIGHTNESS_DEC) || (TS_command == TS_CONTRAST_INC) || (TS_command == TS_CONTRAST_DEC))
        {
          /* A brightness change has been asked */
          BSP_CAMERA_ContrastBrightnessConfig(contrast, brightness);
          if ((TS_command == TS_BRIGHTNESS_INC) || (TS_command == TS_BRIGHTNESS_DEC))
          {
            BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"        Brightness change        ", CENTER_MODE);
          }
          else
          {
            BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"         Contrast change         ", CENTER_MODE);
          }
        }

        if (TS_command == TS_EFFECT)
        {
          /* A color effect change has been asked */
          switch (special_effect)
          {
            case CAMERA_BLACK_WHITE :
              BSP_CAMERA_BlackWhiteConfig(color_effect);
              switch (color_effect)
              {
                case CAMERA_BLACK_WHITE_NORMAL :
                  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"        No effect applied        ", CENTER_MODE);
                  break;
                case CAMERA_BLACK_WHITE_BW :
                  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)" Black and white effect applied  ", CENTER_MODE);
                  break;
                case CAMERA_BLACK_WHITE_NEGATIVE :
                  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"     Negative effect applied     ", CENTER_MODE);
                  break;
                case CAMERA_BLACK_WHITE_BW_NEGATIVE :
                  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"   BW negative effect applied    ", CENTER_MODE);
                  break;
              }
              break;

            case CAMERA_COLOR_EFFECT :
              BSP_CAMERA_ColorEffectConfig(color_effect);
              switch (color_effect)
              {
                case CAMERA_COLOR_EFFECT_ANTIQUE :
                  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"   Sepia color effect applied    ", CENTER_MODE);
                  break;
                case CAMERA_COLOR_EFFECT_BLUE :
                  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"    Blue color effect applied    ", CENTER_MODE);
                  break;
                case CAMERA_COLOR_EFFECT_GREEN :
                  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"   Green color effect applied    ", CENTER_MODE);
                  break;
                case CAMERA_COLOR_EFFECT_RED :
                  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"     Red color effect applied    ", CENTER_MODE);
                  break;
              }
              break;
          }
        }

        if (TS_State.touchDetected >= 3)  /* 3 or more fingers touch to change resolution */
        {
          TS_command = TS_RESOLUTION;
          if (CameraResIndex < CAMERA_RES_INDEX_MAX)
          {
            CameraResIndex++;     /* Switch to higher resolution */
          }
          else
          {
            CameraResIndex = CAMERA_RES_INDEX_MIN;
          }

          /* Disallow DMA2D copy from Camera frame buffer to LCD Frame buffer location */
          Camera_AllowDma2dCopyCamFrmBuffToLcdFrmBuff = 0;

          /* Insert 10 ms delay */
          HAL_Delay(10);

          /* Stop camera stream */
          camera_status = BSP_CAMERA_Stop();
          ASSERT(camera_status != CAMERA_OK);

          /* Clear screen */
          Camera_SetHint();
        }

        /* Wait for touch screen no touch detected */
        do
        {
          BSP_TS_GetState(&TS_State);
        }while(TS_State.touchDetected > 0);
      }
    }

    if (CheckForUserInput() > 0)
    {
      exit = 1;
    }
  }

  if (camera_status == CAMERA_OK)
  {
    /* Stop camera stream */
    camera_status = BSP_CAMERA_Stop();
    ASSERT(camera_status != CAMERA_OK);
  }
  /* End of camera demo */
}

/**
  * @brief  Display Camera demo hint
  * @param  None
  * @retval None
  */
static void Camera_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set Camera Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 90);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"CAMERA EXAMPLE", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"     Press 3 fingers for next resolution     ", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"Press Top/Bottom screen to change brightness ", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"Press Left/Right screen to change contrast   ", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 75, (uint8_t *)"Press 2 fingers to change visual effect      ", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 100, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 110);
  BSP_LCD_DrawRect(11, 101, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 112);
}

/**
  * @brief  DMA2D Camera frame buffer init Transfer completed callback
  * @param  hdma2d: DMA2D handle.
  * @retval None
  */
static void Camera_FrameBuffer_Init_TransferComplete(DMA2D_HandleTypeDef *hdma2d)
{
 CameraFrameBufferInitComplete = 1;
}

/**
  * @brief  DMA2D Camera frame buffer init Transfer error callback
  * @param  hdma2d: DMA2D handle.
  * @retval None
  */
static void Camera_FrameBuffer_Init_TransferError(DMA2D_HandleTypeDef *hdma2d)
{
 CameraFrameBufferInitError = 1;
}


/**
  * @brief  Init camera frame buffer with fixed color in format RGB565
  *         to a LCD display frame buffer line in format ARGB8888 using DMA2D service.
  * @param  sizeX: Size in X of rectangular region of the Camera frame buffer to initialize (in pixels unit)
  * @param  sizeX: Size in X of rectangular region of the Camera frame buffer to initialize (in pixels unit)
  * @param  argb_Value : Initialization value (pattern ARGB8888) to be applied to all rectangular region selected.
  * @retval Status CAMERA_OK or CAMERA_ERROR
  */
static uint32_t CameraFrameBufferRgb565_Init(uint32_t sizeX, uint32_t sizeY, uint32_t argb8888_Value)
{
  HAL_StatusTypeDef hal_status = HAL_OK;
  uint32_t status = CAMERA_ERROR;

  if((sizeX <= CAMERA_VGA_RES_X) && (sizeY <= CAMERA_VGA_RES_Y))
  {
    /* Register to memory mode with RGB565 as colorMode */
    hdma2d_camera.Init.Mode         = DMA2D_R2M; /* Mode Register to Memory */
    hdma2d_camera.Init.ColorMode    = DMA2D_RGB565; /* Output color mode */
    hdma2d_camera.Init.OutputOffset = 0x0; /* No offset in output */

    /* Set callback functions on transfer complete and transfer error */
    hdma2d_camera.XferCpltCallback  = Camera_FrameBuffer_Init_TransferComplete;
    hdma2d_camera.XferErrorCallback = Camera_FrameBuffer_Init_TransferError;

    hdma2d_camera.Instance = DMA2D;

    hal_status = HAL_DMA2D_Init(&hdma2d_camera);
    ASSERT(hal_status != HAL_OK);

    hal_status = HAL_DMA2D_Start_IT(&hdma2d_camera,
                                    argb8888_Value, /* Color value in Register to Memory DMA2D mode */
                                    (uint32_t)CAMERA_FRAME_BUFFER,  /* DMA2D output buffer */
                                    sizeX,  /* width of buffer in pixels */
                                    sizeY); /* height of buffer in lines */
    ASSERT(hal_status != HAL_OK);

    status = CAMERA_OK;
  }

  return (status);
}

/**
  * @brief  Converts a camera buffer line of format RGB565
  *         to a LCD display frame buffer line in format ARGB8888 using DMA2D service.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Pointer to destination buffer
  * @param  xSize: Buffer width
  * @retval None
  */
static void ConvertCameraLineRgb565ToLcdLineARGB8888(void *pSrc, void *pDst, uint32_t xSize)
{
  HAL_StatusTypeDef hal_status = HAL_OK;
  
  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_camera.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_camera.Init.ColorMode    = DMA2D_ARGB8888;
  hdma2d_camera.Init.OutputOffset = 0;

  /* Foreground Configuration */
  hdma2d_camera.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_camera.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_camera.LayerCfg[1].InputColorMode = DMA2D_RGB565;
  hdma2d_camera.LayerCfg[1].InputOffset = 0;

  hdma2d_camera.Instance = DMA2D;

  /* DMA2D Initialization */
  if (HAL_DMA2D_Init(&hdma2d_camera) == HAL_OK)
  {
    if (HAL_DMA2D_ConfigLayer(&hdma2d_camera, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hdma2d_camera, (uint32_t)pSrc, (uint32_t)pDst, xSize, 1) == HAL_OK)
      {
        /* Polling For DMA transfer */
        hal_status = HAL_DMA2D_PollForTransfer(&hdma2d_camera, 10);
        ASSERT(hal_status != HAL_OK);
      }
    }
  }
}

/**
  * @brief  Camera line event callback
  * @param  None
  * @retval None
  */
void BSP_CAMERA_LineEventCallback(void)
{
  uint32_t LcdResX = BSP_LCD_GetXSize();
  uint32_t LcdResY = BSP_LCD_GetYSize();
#if 0
  uint32_t           bgr, i, sizex;
  uint32_t           *ptr;
#endif

  if (Camera_AllowDma2dCopyCamFrmBuffToLcdFrmBuff)
  {
    if ((offset_lcd == 0) && (CameraResX < LcdResX) && (CameraResY < LcdResY))
    {
      /* If Camera resolution is lower than LCD resolution, set display in the middle of the screen */
      offset_lcd =   ((((LcdResY - CameraResY) / 2) * LcdResX)   /* Middle of the screen on Y axis */
                      +   ((LcdResX - CameraResX) / 2))             /* Middle of the screen on X axis */
                     * sizeof(uint32_t);

      if (CameraResY == CAMERA_QQVGA_RES_Y)
      { /* Add offset for QQVGA */
        offset_lcd += 40 * LcdResX * sizeof(uint32_t);
      }
    }

    if (display_line_counter < CameraResY)
    {
      if (display_line_counter < LcdResY)
      {
        if (CameraResX < LcdResX)
        {
          ConvertCameraLineRgb565ToLcdLineARGB8888((uint32_t *)(CAMERA_FRAME_BUFFER + offset_cam),
                                                   (uint32_t *)(LCD_FRAME_BUFFER + offset_lcd),
                                                   CameraResX);
        }
        else
        {
          ConvertCameraLineRgb565ToLcdLineARGB8888((uint32_t *)(CAMERA_FRAME_BUFFER + offset_cam),
                                                   (uint32_t *)(LCD_FRAME_BUFFER + offset_lcd),
                                                   LcdResX);
        }

#if 0
        /* Convert RGB -> BGR */
        ptr = (uint32_t*)(LCD_FRAME_BUFFER + offset_lcd);
        if (CameraResX < LcdResX)
          sizex = CameraResX;
        else
          sizex = LcdResX;

        for (i = 0; i < sizex; i++)
        {
          bgr = ptr[i] & 0xFF00FF00;  /* Alpha and green */
          bgr |= (ptr[i] & 0x00FF0000) >> 16 ; /* blue */
          bgr |= (ptr[i] & 0x000000FF) << 16 ; /* red  */
          ptr[i] = bgr;
        }
#endif
        offset_cam  = offset_cam + (CameraResX * sizeof(uint16_t));
        offset_lcd  = offset_lcd + (LcdResX * sizeof(uint32_t));
      }
      display_line_counter++;
    }
    else
    {
      offset_cam = 0;
      offset_lcd = 0;
      display_line_counter = 0;
    }
  }
}

/**
  * @brief  Handles DMA2D interrupt request.
  * @param  None
  * @retval None
  */
void BSP_LCD_DMA2D_IRQHandler(void)
{
  HAL_DMA2D_IRQHandler(&hdma2d_camera);
}

/**
  * @brief DMA2D MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hdma2d: DMA2D handle pointer
  * @retval None
  */
void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef *hdma2d)
{
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /*##-2- NVIC configuration  ################################################*/
  /* NVIC configuration for DMA2D transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA2D_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);
}

/**
  * @}
  */

/**
  * @}
  */

