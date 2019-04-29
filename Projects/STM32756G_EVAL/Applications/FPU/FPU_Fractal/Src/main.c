/**
  ******************************************************************************
  * @file    FPU/FPU_Fractal/src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
  *          This sample code shows how to use Floating Point Unit.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
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
#include "main.h"
#include "button.h"

/** @addtogroup STM32F7xx_HAL_Applications
  * @{
  */

/** @addtogroup FPU_Fractal
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SCREEN_SENSTIVITY 5
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern LTDC_HandleTypeDef hltdc_eval;
DMA2D_HandleTypeDef    DMA2D_Handle;

extern LTDC_HandleTypeDef  hltdc_eval;
uint32_t L8_CLUT[ITERATION];

uint8_t  text[50];

uint8_t SizeIndex = 0;
uint16_t SizeTable[][6]={{600, 252}, {500, 220},{400, 200},{300, 180},{200, 150},{160, 120}};

uint16_t XSize = 600;
uint16_t YSize = 252;

uint32_t  CurrentZoom = 100;
uint32_t  DirectionZoom = 0;

TS_StateTypeDef TS_State;
__IO uint8_t TouchdOn = 0;
__IO uint8_t TouchReleased = 0;
__IO uint8_t isplaying = 1;
__IO uint8_t playOneFrame = 0;
__IO  uint32_t score_fpu = 0;
__IO  uint32_t tickstart = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void InitCLUT(uint32_t * clut);
static void Generate_Julia_fpu(uint16_t size_x,
                       uint16_t size_y,
                       uint16_t offset_x,
                       uint16_t offset_y,
                       uint16_t zoom,
                       uint8_t * buffer);

static void CPU_CACHE_Enable(void);
static void MPU_Config(void);

static void DMA2D_Init(uint32_t ImageWidth, uint32_t ImageHeight);
static void DMA2D_CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t ImageWidth, uint16_t ImageHeight);
static void DMA2D_CopyButton(uint32_t Button, uint32_t *pDst);
static void LCD_BriefDisplay(void);
static void Touch_Handler(void);
static void print_Size(void);
static void EXTI9_5_IRQHandler_Config(void);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  MPU_Config();

  /* STM32F7xx HAL library initialization:
       - Configure the Flash ART accelerator on ITCM interface
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization: global MSP (MCU Support Package) initialization
   */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure LED1 */
  BSP_LED_Init(LED1);

  InitCLUT(L8_CLUT);

 /*##-1- LCD Configuration ##################################################*/
  /* Initialize the LCD   */
  BSP_LCD_Init();

  BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
  BSP_LCD_SelectLayer(0);


  LCD_BriefDisplay();

  /*##-2- Initialize the Touch Screen ##################################################*/
  BSP_TS_Init(LCD_X_SIZE, LCD_Y_SIZE);
  TS_State.TouchDetected = 0;
  BSP_TS_ITConfig();
  BSP_TS_ITClear();

  EXTI9_5_IRQHandler_Config();

  /* Init xsize and ysize used by fractal algo */
  XSize = SizeTable[SizeIndex][0];
  YSize = SizeTable[SizeIndex][1];

  /*print fractal image size*/
  print_Size();

  /*Copy Pause/Zoom in Off/Zoom out buttons*/
  DMA2D_CopyButton(PAUSE_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
  DMA2D_CopyButton(ZOOM_IN_OFF_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
  DMA2D_CopyButton(ZOOM_OUT_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);

  DMA2D_Init(XSize, YSize);

  while(1)
  {
    if((isplaying != 0) || (playOneFrame != 0))
    {
      playOneFrame = 0;

      BSP_LED_On(LED1);

      tickstart = HAL_GetTick();
      /* Start generating the fractal */
      Generate_Julia_fpu(XSize,
                         YSize,
                         XSize / 2,
                         YSize / 2,
                         CurrentZoom,
                         (uint8_t *)FRACTAL_FRAME_BUFFER);

      /* Get elapsed time */
      score_fpu = (uint32_t)(HAL_GetTick() - tickstart);

      BSP_LED_Off(LED1);

      sprintf((char*)text, "%lu ms",score_fpu);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_DisplayStringAt(440 + 32, 370 + 68 , (uint8_t *)"         ", LEFT_MODE);
      BSP_LCD_DisplayStringAt(440 + 32, 370 + 68 , (uint8_t *)text, LEFT_MODE);

      sprintf((char*)text, "Zoom : %lu",CurrentZoom);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_DisplayStringAt(440 + 8 , 370 + 8 , (uint8_t *)"           ", LEFT_MODE);
      BSP_LCD_DisplayStringAt(440 + 8 , 370 + 8 , (uint8_t *)text, LEFT_MODE);


      if(DirectionZoom == 0)
      {
        if (CurrentZoom < 1000)
        {
          CurrentZoom += 20;
        }
        else
        {
          DirectionZoom = 1;
        }
      }

      if(DirectionZoom == 1)
      {
        if (CurrentZoom > 100)
        {
          CurrentZoom -= 20;
        }
        else
        {
          DirectionZoom = 0;
        }
      }

      /* Copy Result image to the display frame buffer*/
      DMA2D_CopyBuffer((uint32_t *)FRACTAL_FRAME_BUFFER/*buffer*/, (uint32_t *)LCD_FRAME_BUFFER, XSize, YSize);
    }

    Touch_Handler();
  }
}

static void Touch_Handler(void)
{
  if(TouchReleased != 0)
  {
    TouchReleased = 0;

    /*************************Pause/Play buttons *********************/
    if((TS_State.x + SCREEN_SENSTIVITY >= PLAY_PAUSE_BUTTON_XPOS) && \
      (TS_State.x <= (PLAY_PAUSE_BUTTON_XPOS + PLAY_PAUSE_BUTTON_WIDTH + SCREEN_SENSTIVITY)) && \
        (TS_State.y + SCREEN_SENSTIVITY >= PLAY_PAUSE_BUTTON_YPOS) && \
          (TS_State.y <= (PLAY_PAUSE_BUTTON_YPOS + PLAY_PAUSE_BUTTON_HEIGHT + SCREEN_SENSTIVITY)))
    {
      isplaying = 1 - isplaying;

      if(isplaying == 0)
      {
        DMA2D_CopyButton(PLAY_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
      }
      else
      {
        DMA2D_CopyButton(PAUSE_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
      }

      DMA2D_Init(XSize, YSize);
    }
    /*************************Zoom In button *********************/
    else if((TS_State.x + SCREEN_SENSTIVITY >= ZOOM_IN_BUTTON_XPOS) && \
      (TS_State.x <= (ZOOM_IN_BUTTON_XPOS + ZOOM_BUTTON_WIDTH + SCREEN_SENSTIVITY)) && \
        (TS_State.y + SCREEN_SENSTIVITY >= ZOOM_IN_BUTTON_YPOS) && \
          (TS_State.y <= (ZOOM_IN_BUTTON_YPOS + ZOOM_BUTTON_HEIGHT + SCREEN_SENSTIVITY)))
    {
      if (SizeIndex > 0)
      {
        SizeIndex--;
        XSize = SizeTable[SizeIndex][0];
        YSize = SizeTable[SizeIndex][1];

        if(SizeIndex == 0)
        {
          /*zoom in limit reached */
          DMA2D_CopyButton(ZOOM_IN_OFF_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
        }
        else if(SizeIndex == 4)
        {
          /* zoom out limit unreached display zoom out button */
          DMA2D_CopyButton(ZOOM_OUT_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
        }

        /*Clear Fractal Display area */
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_FillRect(2, 112 + 1, 640 - 4, 254);

        BSP_LCD_SetBackColor(0xFF0080FF);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

        print_Size();
        CurrentZoom = 100;

        playOneFrame = 1;
        DMA2D_Init(XSize, YSize);
      }
    }
    /*************************Zoom Out button *********************/
    else if((TS_State.x + SCREEN_SENSTIVITY >= ZOOM_OUT_BUTTON_XPOS) && \
      (TS_State.x <= (ZOOM_OUT_BUTTON_XPOS + ZOOM_BUTTON_WIDTH + SCREEN_SENSTIVITY)) && \
        (TS_State.y + SCREEN_SENSTIVITY >= ZOOM_OUT_BUTTON_YPOS) && \
          (TS_State.y <= (ZOOM_OUT_BUTTON_YPOS + ZOOM_BUTTON_HEIGHT + SCREEN_SENSTIVITY)))
    {
      if (SizeIndex < 5)
      {
        SizeIndex++;
        XSize = SizeTable[SizeIndex][0];
        YSize = SizeTable[SizeIndex][1];

        if(SizeIndex == 5)
        {
          /* zoom out limit reached */
          DMA2D_CopyButton(ZOOM_OUT_OFF_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
        }
        else if (SizeIndex == 1)
        {
          /*zoom in limit unreached  Display zoom in button */
          DMA2D_CopyButton(ZOOM_IN_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
        }

        /*Clear Fractal Display area */
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_FillRect(2, 112 + 1, 640 - 4, 254);

        BSP_LCD_SetBackColor(0xFF0080FF);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

        print_Size();
        CurrentZoom = 100;

        playOneFrame = 1;
        DMA2D_Init(XSize, YSize);
      }
    }
  }
}

/**
  * @brief  EXTI line detection callbacks
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == MFX_IRQOUT_PIN)
  {
    /* Get the IT status register value */
    BSP_TS_GetState(&TS_State);

    if(TS_State.TouchDetected)
    {
      if(TouchdOn < 4)
      {
        TouchdOn++;
      }
      else /*TouchReleased */
      {
        TouchdOn = 0;
        TouchReleased = 1;
      }
    }
    BSP_TS_ITClear();
  }
}

/**
  * @brief  Configures EXTI lines 9 to 5 (connected to PI.8 pin for MFX out) in interrupt mode
  * @param  None
  * @retval None
  */
static void EXTI9_5_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOI clock */
  MFX_IRQOUT_GPIO_CLK_ENABLE();

  /* Configure PI.8 pin for External Interrupt Mode with Rising edge trigger detection  */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = MFX_IRQOUT_PIN;
  HAL_GPIO_Init(MFX_IRQOUT_GPIO_PORT, &GPIO_InitStructure);

  /* Enable and set EXTI lines 9 to 5 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(MFX_IRQOUT_EXTI_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(MFX_IRQOUT_EXTI_IRQn);
}

static void print_Size(void)
{

  sprintf((char*)text, "%u x %u",XSize,YSize);


  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(64, 370 + 24 , (uint8_t *)"Size", LEFT_MODE);
  BSP_LCD_DisplayStringAt(24, 370 + 48 , (uint8_t *)text, LEFT_MODE);

}

static void InitCLUT(uint32_t * clut)
{
  uint32_t  red = 0, green = 0, blue = 0;
  uint32_t  i = 0x00;

  /* Color map generation */
  for (i = 0; i < ITERATION; i++)
  {
    /* Generate red, green and blue values */
    red = (i * 8 * 256 / ITERATION) % 256;
    green = (i * 6 * 256 / ITERATION) % 256;
    blue = (i * 4 * 256 / ITERATION) % 256;

    red = red << 16;
    green = green << 8;

    /* Store the 32-bit value */
    clut[i] = 0xFF000000 | (red + green + blue);
  }
}

static void Generate_Julia_fpu(uint16_t size_x, uint16_t size_y, uint16_t offset_x, uint16_t offset_y, uint16_t zoom, uint8_t * buffer)
{
  float       tmp1, tmp2;
  float       num_real, num_img;
  float       rayon;

  uint8_t       i;
  uint16_t      x, y;

  for (y = 0; y < size_y; y++)
  {
    for (x = 0; x < size_x; x++)
    {
      num_real = y - offset_y;
      num_real = num_real / zoom;
      num_img = x - offset_x;
      num_img = num_img / zoom;
      i = 0;
      rayon = 0;
      while ((i < ITERATION - 1) && (rayon < 4))
      {
        tmp1 = num_real * num_real;
        tmp2 = num_img * num_img;
        num_img = 2 * num_real * num_img + IMG_CONSTANT;
        num_real = tmp1 - tmp2 + REAL_CONSTANT;
        rayon = tmp1 + tmp2;
        i++;
      }
      /* Store the value in the buffer */
      buffer[x+y*size_x] = i;
    }
  }
}

static void LCD_BriefDisplay(void)
{
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(0xFF0080FF);
  BSP_LCD_SetTextColor(0xFF0080FF);
  BSP_LCD_FillRect(2, 2, 640 - 4, 112 - 2);

  /*Title*/
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(2, 24, (uint8_t *)"STM32F756 Fractal Benchmark", CENTER_MODE);
  BSP_LCD_DisplayStringAt(2, 72, (uint8_t *)SCORE_FPU_MODE, CENTER_MODE);

  /*Fractal Display area */
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillRect(2, 112 + 1, 640 - 4, 254);



  /*image Size*/
  BSP_LCD_SetBackColor(0xFF0080FF);
  BSP_LCD_SetTextColor(0xFF0080FF);
  BSP_LCD_FillRect(2, 370, 180 - 2, 112 - 4);

  /*Button area */
  BSP_LCD_SetBackColor(0xFF0080FF);
  BSP_LCD_SetTextColor(0xFF0080FF);
  BSP_LCD_FillRect(182, 370, 260 - 2, 112 - 4);

  /*Calculation Time*/
  BSP_LCD_SetBackColor(0xFF0080FF);
  BSP_LCD_SetTextColor(0xFF0080FF);
  BSP_LCD_FillRect(442, 370, 200 - 4, 112 - 4);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(440 + 32, 370 + 48 , (uint8_t *)"Duration:", LEFT_MODE);

}

/**
  * @brief  Initialize the DMA2D in memory to memory with PFC.
  * @param  ImageWidth: image width
  * @param  ImageHeight: image Height
  * @retval None
  */
static void DMA2D_Init(uint32_t ImageWidth, uint32_t ImageHeight)
{
  DMA2D_CLUTCfgTypeDef CLUTCfg;

  /* Init DMA2D */
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
  DMA2D_Handle.Init.Mode         = DMA2D_M2M_PFC;
  DMA2D_Handle.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  DMA2D_Handle.Init.OutputOffset = LCD_X_SIZE - ImageWidth;

  /*##-2- DMA2D Callbacks Configuration ######################################*/
  DMA2D_Handle.XferCpltCallback  = NULL;

  /*##-3- Foreground Configuration ###########################################*/
  DMA2D_Handle.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  DMA2D_Handle.LayerCfg[1].InputAlpha = 0xFF;

  DMA2D_Handle.LayerCfg[1].InputColorMode = DMA2D_INPUT_L8;
  DMA2D_Handle.LayerCfg[1].InputOffset = 0;

  DMA2D_Handle.Instance          = DMA2D;

  /*##-4- DMA2D Initialization     ###########################################*/
  HAL_DMA2D_Init(&DMA2D_Handle);
  HAL_DMA2D_ConfigLayer(&DMA2D_Handle, 1);

  /* Load DMA2D Foreground CLUT */
  CLUTCfg.CLUTColorMode = DMA2D_CCM_ARGB8888;
  CLUTCfg.pCLUT = (uint32_t *)L8_CLUT;
  CLUTCfg.Size = 255;

  HAL_DMA2D_CLUTLoad(&DMA2D_Handle, CLUTCfg, 1);
  HAL_DMA2D_PollForTransfer(&DMA2D_Handle, 100);
}

/**
  * @brief  Copy the Decoded image to the display Frame buffer.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Pointer to destination buffer
  * @param  ImageWidth: image width
  * @param  ImageHeight: image Height
  * @retval None
  */
static void DMA2D_CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t ImageWidth, uint16_t ImageHeight)
{

  uint32_t xPos, yPos, destination;

  /*##-1- calculate the destination transfer address  ############*/
  xPos = (LCD_X_SIZE - ImageWidth)/2;
  yPos = ((LCD_Y_SIZE - ImageHeight)/2);

  destination = (uint32_t)pDst + (yPos * LCD_X_SIZE + xPos) * 4;


  HAL_DMA2D_Start(&DMA2D_Handle, (uint32_t)pSrc, destination, ImageWidth, ImageHeight);
  HAL_DMA2D_PollForTransfer(&DMA2D_Handle, 100);

}

static void DMA2D_CopyButton(uint32_t Button, uint32_t *pDst)
{
  uint32_t xPos = 0;
  uint32_t yPos = 0;
  uint32_t destination, buttonWidth;
  uint32_t *pSrc = NULL;

  if(PLAY_BUTTON == Button)
  {
    xPos = PLAY_PAUSE_BUTTON_XPOS;
    yPos = PLAY_PAUSE_BUTTON_YPOS;

    buttonWidth = PLAY_PAUSE_BUTTON_WIDTH;

    pSrc = (uint32_t *)Play_Button;
  }
  else if(PAUSE_BUTTON == Button)
  {
    xPos = PLAY_PAUSE_BUTTON_XPOS;
    yPos = PLAY_PAUSE_BUTTON_YPOS;

    buttonWidth = PLAY_PAUSE_BUTTON_WIDTH;

    pSrc = (uint32_t *)Pause_Button;
  }
  else if(ZOOM_IN_BUTTON == Button)
  {
    xPos = ZOOM_IN_BUTTON_XPOS;
    yPos = ZOOM_IN_BUTTON_YPOS;

    buttonWidth = ZOOM_BUTTON_WIDTH;

    pSrc = (uint32_t *)ZOOM_IN_Button;
  }
  else if(ZOOM_OUT_BUTTON == Button)
  {
    xPos = ZOOM_OUT_BUTTON_XPOS;
    yPos = ZOOM_OUT_BUTTON_YPOS;

    buttonWidth = ZOOM_BUTTON_WIDTH;

    pSrc = (uint32_t *)ZOOM_Out_Button;
  }
  else if(ZOOM_IN_OFF_BUTTON == Button)
  {
    xPos = ZOOM_IN_BUTTON_XPOS;
    yPos = ZOOM_IN_BUTTON_YPOS;

    buttonWidth = ZOOM_BUTTON_WIDTH;

    pSrc = (uint32_t *)ZOOM_IN_Off_Button;
  }
  else if(ZOOM_OUT_OFF_BUTTON == Button)
  {
    xPos = ZOOM_OUT_BUTTON_XPOS;
    yPos = ZOOM_OUT_BUTTON_YPOS;

    buttonWidth = ZOOM_BUTTON_WIDTH;

    pSrc = (uint32_t *)ZOOM_Out_Off_Button;
  }

  /* Init DMA2D */
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
  DMA2D_Handle.Init.Mode         = DMA2D_M2M_PFC;
  DMA2D_Handle.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  DMA2D_Handle.Init.OutputOffset = LCD_X_SIZE - buttonWidth;

  /*##-2- DMA2D Callbacks Configuration ######################################*/
  DMA2D_Handle.XferCpltCallback  = NULL;

  /*##-3- Foreground Configuration ###########################################*/
  DMA2D_Handle.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  DMA2D_Handle.LayerCfg[1].InputAlpha = 0xFF;
  DMA2D_Handle.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  DMA2D_Handle.LayerCfg[1].InputOffset = 0;

  DMA2D_Handle.Instance          = DMA2D;

  /*##-4- DMA2D Initialization     ###########################################*/
  HAL_DMA2D_Init(&DMA2D_Handle);
  HAL_DMA2D_ConfigLayer(&DMA2D_Handle, 1);



  destination = (uint32_t)pDst + (yPos * LCD_X_SIZE + xPos) * 4;

  HAL_DMA2D_Start(&DMA2D_Handle, (uint32_t)pSrc, destination, PLAY_PAUSE_BUTTON_WIDTH, PLAY_PAUSE_BUTTON_HEIGHT);
  HAL_DMA2D_PollForTransfer(&DMA2D_Handle, 100);

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 200000000
  *            HCLK(Hz)                       = 200000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 400
  *            PLL_P                          = 2
  *            PLL_Q                          = 8
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef  ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Activate the OverDrive to reach the 216 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/**
  * @brief  Configure the MPU attributes as Write Through for SDRAM.
  * @note   The Base Address is 0xC0000000.
  *         The Region Size is 8MB, it is related to SDRAM memory size.
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
