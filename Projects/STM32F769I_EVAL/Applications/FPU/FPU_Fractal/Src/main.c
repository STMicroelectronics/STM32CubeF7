/**
  ******************************************************************************
  * @file    FPU/FPU_Fractal/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use Floating Point Unit.
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
#include "button.h"

/** @addtogroup STM32F7xx_HAL_Applications
 * @{
 */

/** @addtogroup FPU_Fractal
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define VSYNC           ((uint32_t)  1U)  
#define VBP             ((uint32_t)  1U) 
#define VFP             ((uint32_t)  1U)
#define VACT            ((uint32_t)480U)
#define HSYNC           ((uint32_t)  1U)
#define HBP             ((uint32_t)  1U)
#define HFP             ((uint32_t)  1U)
#define HACT            ((uint32_t)400U)

#define LEFT_AREA         ((uint32_t)1U)
#define RIGHT_AREA        ((uint32_t)2U)

#define __DSI_MASK_TE()   (GPIOJ->AFR[0] &= (0xFFFFF0FFU))   /* Mask DSI TearingEffect Pin*/
#define __DSI_UNMASK_TE() (GPIOJ->AFR[0] |= ((uint32_t)(GPIO_AF13_DSI) << 8)) /* UnMask DSI TearingEffect Pin*/

#define SCREEN_SENSTIVITY ((uint32_t)30U)

#define FRACTAL_MAX_ZOOM  ((uint32_t)1000U)
#define FRACTAL_MIN_ZOOM  ((uint32_t) 100U)
#define FRACTAL_ZOOM_STEP ((uint32_t)  20U)

#define FRACTAL_ZOOM_IN_DIRECTION  ((uint32_t)0U)
#define FRACTAL_ZOOM_OUT_DIRECTION ((uint32_t)1U)


/* Private variables ---------------------------------------------------------*/ 

extern LTDC_HandleTypeDef hltdc_eval;
extern DSI_HandleTypeDef hdsi_eval;

uint8_t pColLeft[]    = {0x00, 0x00, 0x01, 0x8F}; /*   0 -> 399 */
uint8_t pColRight[]   = {0x01, 0x90, 0x03, 0x1F}; /* 400 -> 799 */
uint8_t pPage[]       = {0x00, 0x00, 0x01, 0xDF}; /*   0 -> 479 */
uint8_t pScanCol[]    = {0x02, 0x15};             /* Scan @ 533 */

DMA2D_HandleTypeDef    DMA2D_Handle;

static __IO uint32_t pending_buffer = 0;
static __IO uint32_t active_area = 0;

DSI_CmdCfgTypeDef CmdCfg;
DSI_LPCmdTypeDef LPCmd;
DSI_PLLInitTypeDef dsiPllInit;
static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

extern LTDC_HandleTypeDef  hltdc_eval;
uint32_t L8_CLUT[ITERATION];

uint8_t  text[50];

uint8_t SizeIndex = 0;
uint16_t SizeTable[][6]={{600, 252}, {500, 220},{400, 200},{300, 180},{200, 150},{160, 120}};

uint16_t XSize = 600;
uint16_t YSize = 252;

uint32_t  CurrentZoom = FRACTAL_MIN_ZOOM;  
uint32_t  DirectionZoom = FRACTAL_ZOOM_IN_DIRECTION; 

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
static uint8_t LCD_Init(void);
static void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address);
static void LTDC_Init(void);
static void LCD_BriefDisplay(void);
static void Touch_Handler(void);
static void print_Size(void);
static void EXTI9_5_IRQHandler_Config(void);


/* Private functions ---------------------------------------------------------*/

/**
 * @brief   Main program
 * @param  None
 * @retval None
 */

int main(void)
{
  /* Disable the MPU */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32F7xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
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
  
  /* Configure the SDRAM */
  BSP_SDRAM_Init();

  /* Init Julia C-LUT*/
  InitCLUT(L8_CLUT);
  
  /*##-1- Initialize the Touch Screen ##################################################*/
  BSP_TS_Init(LCD_X_SIZE, LCD_Y_SIZE);
  TS_State.touchDetected = 0;
  BSP_TS_ITConfig();
  BSP_TS_ITClear();
  EXTI9_5_IRQHandler_Config(); 
  
 /*##-2- LCD Configuration (DSI in Command mode) ##################################################*/  
  /* Initialize the LCD   */
  LCD_Init(); 
  
  LCD_LayertInit(0, LCD_FRAME_BUFFER);
  BSP_LCD_SelectLayer(0); 
  
  HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft);
  HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_PASET, pPage);
  
  /* Update pitch : the draw is done on the whole physical X Size */
  HAL_LTDC_SetPitch(&hltdc_eval, BSP_LCD_GetXSize(), 0);

  /* Display Example description */  
  LCD_BriefDisplay();
  
  pending_buffer = 1;
  active_area = LEFT_AREA;
  
  HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pScanCol);

  /* Send Display On DCS Command to display */
  HAL_DSI_ShortWrite(&(hdsi_eval),
                     0,
                     DSI_DCS_SHORT_PKT_WRITE_P1,
                     OTM8009A_CMD_DISPON,
                     0x00);
  
  /*Wait until the DSI ends the refresh of previous frame (Left and Right area)*/  
  while(pending_buffer != 0)
  {
  }  

  /* Init xsize and ysize used by fractal algo */
  XSize = SizeTable[SizeIndex][0];
  YSize = SizeTable[SizeIndex][1];
  
  /*print fractal image size*/
  print_Size();  
  
  /*Copy Pause and Zoom buttons to the LCD screen*/
  DMA2D_CopyButton(PAUSE_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
  DMA2D_CopyButton(ZOOM_IN_OFF_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);
  DMA2D_CopyButton(ZOOM_OUT_BUTTON,(uint32_t *)LCD_FRAME_BUFFER);  
 
 /*
    Initialize the DMA2D in memory to memory with PFC
    to copy fractal image result to the LCD clear.
    DMA2D input Color is L8 (julia fractal image is L8 indexed image)
    DMA2D output Color is ARGB8888 (for LCD display)
  
  */
  DMA2D_Init(XSize, YSize);
  
  while(1)
  {
    /*Infinite loop*/
    if((isplaying != 0) || (playOneFrame != 0))
    {
      playOneFrame = 0;
      
      /*LED1 On*/
      BSP_LED_On(LED1);
      
      /*Get current time*/
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
      
      /*display current frame ellpased time*/
      sprintf((char*)text, "%lu ms",score_fpu);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_DisplayStringAt(600 + 32, 370 + 68 , (uint8_t *)"         ", LEFT_MODE); 
      BSP_LCD_DisplayStringAt(600 + 32, 370 + 68 , (uint8_t *)text, LEFT_MODE);  

      /*display current zoom*/      
      sprintf((char*)text, "Zoom : %lu",CurrentZoom);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_DisplayStringAt(600 + 8 , 370 + 8 , (uint8_t *)"           ", LEFT_MODE); 
      BSP_LCD_DisplayStringAt(600 + 8 , 370 + 8 , (uint8_t *)text, LEFT_MODE);  
      
      
      if(DirectionZoom == FRACTAL_ZOOM_IN_DIRECTION)
      {
        /*Zoom direction is zoom In*/
        
        if (CurrentZoom < FRACTAL_MAX_ZOOM) 
        {
          /* if current zoom < 1000 then increment current zoom by 20*/
          CurrentZoom += FRACTAL_ZOOM_STEP;
        }
        else
        {
          /*Maximum zoom (1000) reached : set zoom direction to zoom out */
          DirectionZoom = FRACTAL_ZOOM_OUT_DIRECTION;
        }
      }      
      else if(DirectionZoom == FRACTAL_ZOOM_OUT_DIRECTION)
      {
        /*Zoom direction is zoom Out*/        
        if (CurrentZoom > FRACTAL_MIN_ZOOM) 
        {
          /* if current zoom > 100 then decrement current zoom by 20*/          
          CurrentZoom -= FRACTAL_ZOOM_STEP;
        }
        else
        {
          /*Minimum zoom (100) reached : set zoom direction to zoom In */          
          DirectionZoom = FRACTAL_ZOOM_IN_DIRECTION;
        }
      }

      /* Copy Result fractal image to the display frame buffer*/      
      DMA2D_CopyBuffer((uint32_t *)FRACTAL_FRAME_BUFFER, (uint32_t *)LCD_FRAME_BUFFER, XSize, YSize);
    }

    /* Handle touch events*/
    Touch_Handler();
  }
}

static void Touch_Handler(void)
{
  if(TouchReleased != 0)  
  {
    TouchReleased = 0;
    
    /*************************Pause/Play buttons *********************/
    if((TS_State.touchX[0] + SCREEN_SENSTIVITY >= PLAY_PAUSE_BUTTON_XPOS) && \
      (TS_State.touchX[0] <= (PLAY_PAUSE_BUTTON_XPOS + PLAY_PAUSE_BUTTON_WIDTH + SCREEN_SENSTIVITY)) && \
        (TS_State.touchY[0] + SCREEN_SENSTIVITY >= PLAY_PAUSE_BUTTON_YPOS) && \
          (TS_State.touchY[0] <= (PLAY_PAUSE_BUTTON_YPOS + PLAY_PAUSE_BUTTON_HEIGHT + SCREEN_SENSTIVITY)))
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
    else if((TS_State.touchX[0] + SCREEN_SENSTIVITY >= ZOOM_IN_BUTTON_XPOS) && \
      (TS_State.touchX[0] <= (ZOOM_IN_BUTTON_XPOS + ZOOM_BUTTON_WIDTH + SCREEN_SENSTIVITY)) && \
        (TS_State.touchY[0] + SCREEN_SENSTIVITY >= ZOOM_IN_BUTTON_YPOS) && \
          (TS_State.touchY[0] <= (ZOOM_IN_BUTTON_YPOS + ZOOM_BUTTON_HEIGHT + SCREEN_SENSTIVITY)))
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
        BSP_LCD_FillRect(2, 112 + 1, 800 - 4, 254);
        
        BSP_LCD_SetBackColor(0xFF0080FF);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);        
        
        print_Size();
        CurrentZoom = 100;
        
        playOneFrame = 1;
        DMA2D_Init(XSize, YSize);
      }  
    }
    /*************************Zoom Out button *********************/
    else if((TS_State.touchX[0] + SCREEN_SENSTIVITY >= ZOOM_OUT_BUTTON_XPOS) && \
      (TS_State.touchX[0] <= (ZOOM_OUT_BUTTON_XPOS + ZOOM_BUTTON_WIDTH + SCREEN_SENSTIVITY)) && \
        (TS_State.touchY[0] + SCREEN_SENSTIVITY >= ZOOM_OUT_BUTTON_YPOS) && \
          (TS_State.touchY[0] <= (ZOOM_OUT_BUTTON_YPOS + ZOOM_BUTTON_HEIGHT + SCREEN_SENSTIVITY)))
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
        BSP_LCD_FillRect(2, 112 + 1, 800 - 4, 254);
        
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
    if(BSP_TS_ITGetStatus())
    {
      BSP_TS_ITClear();
      BSP_TS_GetState(&TS_State);
      
      if(TS_State.touchDetected != 0) /*Touch Detected */
      {
        TouchdOn = 1;
        TS_State.touchDetected = 0;
      }
      else if(TouchdOn == 1) /*Touch Released */
      {        
        TouchdOn = 0;
        TouchReleased = 1;
      }
      
    }
    BSP_IO_ITClear(); /* Clear MFX Interrupt*/
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

/**
  * @brief  Display current fractal image dimensions to the LCD screen
  * @param  None
  * @retval None
  */
static void print_Size(void)
{
  /*wait till current DSI refresh ends if any*/
  while(pending_buffer != 0)
  {
  } 
  
  sprintf((char*)text, "%d x %d",XSize,YSize);


  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(64, 370 + 24 , (uint8_t *)"Size", LEFT_MODE); 
  BSP_LCD_DisplayStringAt(24, 370 + 48 , (uint8_t *)text, LEFT_MODE);

  /* Frame Buffer updated , unmask the DSI TE pin to ask for a DSI refresh*/ 
  pending_buffer = 1;
  /* UnMask the TE */
  __DSI_UNMASK_TE();
  
  while(pending_buffer != 0)
  {
  }   
  
}

/**
  * @brief  Init L8 to ARGB8888 C-LUT used in fractal image colors conversion for display 
  * @param  clut : pointer to color LUT table
  * @retval None
  */
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

/**
  * @brief  Julia Calculation
  * @param  size_x: screen width in pixel
  * @param  size_y: screen height in pixel
  * @param  offset_x: x offset for starting point
  * @param  offset_y: y offset for starting point
  * @param  zoom: Iteration zoom ratio
  * @param  buffer: pointer to output buffer where values are stored
  * @retval None
  */
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

/**
  * @brief  Display Example description.
  * @param  None
  * @retval None
  */
static void LCD_BriefDisplay(void)
{
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(0xFF0080FF);
  BSP_LCD_SetTextColor(0xFF0080FF);
  BSP_LCD_FillRect(2, 2, 800 - 4, 112 - 2);  
  
  /*Title*/
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(2, 24, (uint8_t *)"STM32F769I Fractal Benchmark", CENTER_MODE);
  BSP_LCD_DisplayStringAt(2, 72, (uint8_t *)SCORE_FPU_MODE, CENTER_MODE);  
  
  /*Fractal Display area */
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillRect(2, 112 + 1, 800 - 4, 254);
   

  
  /*image Size*/  
  BSP_LCD_SetBackColor(0xFF0080FF);
  BSP_LCD_SetTextColor(0xFF0080FF);  
  BSP_LCD_FillRect(2, 370, 200 - 2, 112 - 4);

  /*Calculation Time*/      
  BSP_LCD_SetBackColor(0xFF0080FF);
  BSP_LCD_SetTextColor(0xFF0080FF); 
  BSP_LCD_FillRect(202, 370, 400 - 2, 112 - 4);
  
  BSP_LCD_SetBackColor(0xFF0080FF);
  BSP_LCD_SetTextColor(0xFF0080FF);   
  BSP_LCD_FillRect(602, 370, 200 - 4, 112 - 4);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(600 + 32, 370 + 48 , (uint8_t *)"Duration:", LEFT_MODE);   

}

/**
  * @brief  Initializes the DSI LCD. 
  * The ititialization is done as below:
  *     - DSI PLL ititialization
  *     - DSI ititialization
  *     - LTDC ititialization
  *     - OTM8009A LCD Display IC Driver ititialization
  * @param  None
  * @retval LCD state
  */
static uint8_t LCD_Init(void)
{
  DSI_PHY_TimerTypeDef  PhyTimings;  
  GPIO_InitTypeDef GPIO_Init_Structure;
  
  /* Toggle Hardware Reset of the DSI LCD using
     its XRES signal (active low) */
  BSP_LCD_Reset();
  
  /* Call first MSP Initialize only in case of first initialization
  * This will set IP blocks LTDC, DSI and DMA2D
  * - out of reset
  * - clocked
  * - NVIC IRQ related to IP blocks enabled
  */
  BSP_LCD_MspInit();
  
  /* LCD clock configuration */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 417 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 417 MHz / 5 = 83.4 MHz */
  /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 83.4 / 2 = 41.7 MHz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 417;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  
  /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
  hdsi_eval.Instance = DSI;
  
  HAL_DSI_DeInit(&(hdsi_eval));
  
  dsiPllInit.PLLNDIV  = 100;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV5;
  dsiPllInit.PLLODF  = DSI_PLL_OUT_DIV1;  

  hdsi_eval.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  hdsi_eval.Init.TXEscapeCkdiv = 0x4;
  
  
  HAL_DSI_Init(&(hdsi_eval), &(dsiPllInit));
    
  /* Configure the DSI for Command mode */
  CmdCfg.VirtualChannelID      = 0;
  CmdCfg.HSPolarity            = DSI_HSYNC_ACTIVE_HIGH;
  CmdCfg.VSPolarity            = DSI_VSYNC_ACTIVE_HIGH;
  CmdCfg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
  CmdCfg.ColorCoding           = DSI_RGB888;
  CmdCfg.CommandSize           = HACT;
  CmdCfg.TearingEffectSource   = DSI_TE_EXTERNAL;
  CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
  CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
  CmdCfg.AutomaticRefresh      = DSI_AR_DISABLE;
  CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_ENABLE;
  HAL_DSI_ConfigAdaptedCommandMode(&hdsi_eval, &CmdCfg);
  
  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_ENABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_ENABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_ENABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_ENABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_ENABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_ENABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_ENABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_ENABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_ENABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_ENABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_ENABLE;
  HAL_DSI_ConfigCommand(&hdsi_eval, &LPCmd);

  /* Initialize LTDC */
  LTDC_Init();
  
  /* Start DSI */
  HAL_DSI_Start(&(hdsi_eval));

  /* Configure DSI PHY HS2LP and LP2HS timings */
  PhyTimings.ClockLaneHS2LPTime = 35;
  PhyTimings.ClockLaneLP2HSTime = 35;
  PhyTimings.DataLaneHS2LPTime = 35;
  PhyTimings.DataLaneLP2HSTime = 35;
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime = 10;
  HAL_DSI_ConfigPhyTimer(&hdsi_eval, &PhyTimings);  
    
  /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver)
  */
  OTM8009A_Init(OTM8009A_COLMOD_RGB888, LCD_ORIENTATION_LANDSCAPE);
  
  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_DISABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_DISABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_DISABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_DISABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_DISABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_DISABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_DISABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_DISABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_DISABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_DISABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_DISABLE;
  HAL_DSI_ConfigCommand(&hdsi_eval, &LPCmd);
  
  HAL_DSI_ConfigFlowControl(&hdsi_eval, DSI_FLOW_CONTROL_BTA);
  
  
  /* Enable GPIOJ clock */
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  
  /* Configure DSI_TE pin from MB1166 : Tearing effect on separated GPIO from KoD LCD */
  /* that is mapped on GPIOJ2 as alternate DSI function (DSI_TE)                      */
  /* This pin is used only when the LCD and DSI link is configured in command mode    */
  GPIO_Init_Structure.Pin       = GPIO_PIN_2;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DSI;
  HAL_GPIO_Init(GPIOJ, &GPIO_Init_Structure);   
  
  /* Send Display Off DCS Command to display */
  HAL_DSI_ShortWrite(&(hdsi_eval),
                     0,
                     DSI_DCS_SHORT_PKT_WRITE_P1,
                     OTM8009A_CMD_DISPOFF,
                     0x00);
  
  /* Refresh the display */
  HAL_DSI_Refresh(&hdsi_eval);
  
  return LCD_OK;
}

/**
  * @brief  Initialize the LTDC
  * @param  None
  * @retval None
  */
static void LTDC_Init(void)
{
  /* DeInit */
  HAL_LTDC_DeInit(&hltdc_eval);
  
  /* LTDC Config */
  /* Timing and polarity */
  hltdc_eval.Init.HorizontalSync = HSYNC;
  hltdc_eval.Init.VerticalSync = VSYNC;
  hltdc_eval.Init.AccumulatedHBP = HSYNC+HBP;
  hltdc_eval.Init.AccumulatedVBP = VSYNC+VBP;
  hltdc_eval.Init.AccumulatedActiveH = VSYNC+VBP+VACT;
  hltdc_eval.Init.AccumulatedActiveW = HSYNC+HBP+HACT;
  hltdc_eval.Init.TotalHeigh = VSYNC+VBP+VACT+VFP;
  hltdc_eval.Init.TotalWidth = HSYNC+HBP+HACT+HFP;
  
  /* background value */
  hltdc_eval.Init.Backcolor.Blue = 0;
  hltdc_eval.Init.Backcolor.Green = 0;
  hltdc_eval.Init.Backcolor.Red = 0;
  
  /* Polarity */
  hltdc_eval.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc_eval.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc_eval.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc_eval.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc_eval.Instance = LTDC;

  HAL_LTDC_Init(&hltdc_eval);
}

/**
  * @brief  Initializes the LCD layers.
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
static void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address)
{
  LCD_LayerCfgTypeDef  Layercfg;

  /* Layer Init */
  Layercfg.WindowX0 = 0;
  Layercfg.WindowX1 = BSP_LCD_GetXSize()/2 ;
  Layercfg.WindowY0 = 0;
  Layercfg.WindowY1 = BSP_LCD_GetYSize(); 
  Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  Layercfg.FBStartAdress = Address;
  Layercfg.Alpha = 255;
  Layercfg.Alpha0 = 0;
  Layercfg.Backcolor.Blue = 0;
  Layercfg.Backcolor.Green = 0;
  Layercfg.Backcolor.Red = 0;
  Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  Layercfg.ImageWidth = BSP_LCD_GetXSize() / 2;
  Layercfg.ImageHeight = BSP_LCD_GetYSize();
  
  HAL_LTDC_ConfigLayer(&hltdc_eval, &Layercfg, LayerIndex); 
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
  DMA2D_Handle.Init.ColorMode    = DMA2D_ARGB8888;
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
  * @brief  Copy the fractal result L8 image to the display Frame buffer.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Pointer to destination buffer
  * @param  ImageWidth: image width
  * @param  ImageHeight: image Height 
  * @retval None
  */
static void DMA2D_CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t ImageWidth, uint16_t ImageHeight)
{
  
  uint32_t xPos, yPos, destination;       
  
  /* calculate the destination transfer address  */
  xPos = (LCD_X_SIZE - ImageWidth)/2;
  yPos = ((LCD_Y_SIZE - ImageHeight)/2);  
  
  destination = (uint32_t)pDst + (yPos * LCD_X_SIZE + xPos) * 4;
 
  /* Wait until the DSI ends the refresh of previous frame (Left and Right area) */  
  while(pending_buffer != 0)
  {
  }
  
  /* Start the image copy using DMA2D */
  HAL_DMA2D_Start(&DMA2D_Handle, (uint32_t)pSrc, destination, ImageWidth, ImageHeight);
  /* Poll for end of DMA2D copy */  
  HAL_DMA2D_PollForTransfer(&DMA2D_Handle, 100);

  /* Frame Buffer updated , unmask the DSI TE pin to ask for a DSI refresh*/ 
  pending_buffer = 1;
  /* UnMask the TE */
  __DSI_UNMASK_TE();

  /* Wait until the DSI ends the refresh(Left and Right area) */    
  while(pending_buffer != 0)
  {
  }  

}

/**
  * @brief  Copy Button image to the display Frame buffer.
  * @param  Button: button to copy
  * @param  pDst: Pointer to destination buffer
  * @retval None
  */
static void DMA2D_CopyButton(uint32_t Button, uint32_t *pDst)
{
  uint32_t xPos = 0, yPos = 0, destination = 0, buttonWidth = 0;
  uint32_t *pSrc = 0;

  /* Wait until the DSI ends the refresh of previous frame (Left and Right area) */  
  while(pending_buffer != 0)
  {
  }  
  
  if(PLAY_BUTTON == Button)
  {
    /* Button to copy is Play */
    xPos = PLAY_PAUSE_BUTTON_XPOS;
    yPos = PLAY_PAUSE_BUTTON_YPOS;
    
    buttonWidth = PLAY_PAUSE_BUTTON_WIDTH;
    
    pSrc = (uint32_t *)Play_Button;
  }
  else if(PAUSE_BUTTON == Button)
  {
    /* Button to copy is Pause */    
    xPos = PLAY_PAUSE_BUTTON_XPOS;
    yPos = PLAY_PAUSE_BUTTON_YPOS;

    buttonWidth = PLAY_PAUSE_BUTTON_WIDTH;
    
    pSrc = (uint32_t *)Pause_Button;
  }
  else if(ZOOM_IN_BUTTON == Button)
  {
    /* Button to copy is Zoom In */ 
    xPos = ZOOM_IN_BUTTON_XPOS;
    yPos = ZOOM_IN_BUTTON_YPOS;

    buttonWidth = ZOOM_BUTTON_WIDTH;    
    
    pSrc = (uint32_t *)ZOOM_IN_Button;
  }
  else if(ZOOM_OUT_BUTTON == Button)
  {
    /* Button to copy is Zoom Out */     
    xPos = ZOOM_OUT_BUTTON_XPOS;
    yPos = ZOOM_OUT_BUTTON_YPOS;
    
    buttonWidth = ZOOM_BUTTON_WIDTH;      
    
    pSrc = (uint32_t *)ZOOM_Out_Button;
  }
  else if(ZOOM_IN_OFF_BUTTON == Button)
  {
    /* Button to copy is Zoom In OFF */      
    xPos = ZOOM_IN_BUTTON_XPOS;
    yPos = ZOOM_IN_BUTTON_YPOS;

    buttonWidth = ZOOM_BUTTON_WIDTH;    
    
    pSrc = (uint32_t *)ZOOM_IN_Off_Button;
  }
  else if(ZOOM_OUT_OFF_BUTTON == Button)
  {
    /* Button to copy is Zoom Out OFF */     
    xPos = ZOOM_OUT_BUTTON_XPOS;
    yPos = ZOOM_OUT_BUTTON_YPOS;

    buttonWidth = ZOOM_BUTTON_WIDTH;    
    
    pSrc = (uint32_t *)ZOOM_Out_Off_Button;
  }  
  
  /* Init DMA2D */
  /* Configure the DMA2D Mode, Color Mode and output offset */ 
  DMA2D_Handle.Init.Mode         = DMA2D_M2M_PFC;
  DMA2D_Handle.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  DMA2D_Handle.Init.OutputOffset = LCD_X_SIZE - buttonWidth;     
  
  /*DMA2D Callbacks Configuration */
  DMA2D_Handle.XferCpltCallback  = NULL;
  
  /* Foreground Configuration */
  DMA2D_Handle.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  DMA2D_Handle.LayerCfg[1].InputAlpha = 0xFF;
  DMA2D_Handle.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  DMA2D_Handle.LayerCfg[1].InputOffset = 0;
  
  DMA2D_Handle.Instance          = DMA2D; 

  /* DMA2D Initialization */
  HAL_DMA2D_Init(&DMA2D_Handle);
  HAL_DMA2D_ConfigLayer(&DMA2D_Handle, 1);     
  

  /* calculate the destination transfer address  */  
  destination = (uint32_t)pDst + (yPos * LCD_X_SIZE + xPos) * 4;

  /* Start the image copy using DMA2D */  
  HAL_DMA2D_Start(&DMA2D_Handle, (uint32_t)pSrc, destination, PLAY_PAUSE_BUTTON_WIDTH, PLAY_PAUSE_BUTTON_HEIGHT);
  /* Poll for end of DMA2D copy */    
  HAL_DMA2D_PollForTransfer(&DMA2D_Handle, 100);

  /* Frame Buffer updated , unmask the DSI TE pin to ask for a DSI refresh*/ 
  pending_buffer = 1;
  /* UnMask the TE */
  __DSI_UNMASK_TE();

  /* Wait until the DSI ends the refresh(Left and Right area) */   
  while(pending_buffer != 0)
  {
  } 
  
}

/**
  * @brief  Tearing Effect DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_TearingEffectCallback(DSI_HandleTypeDef *hdsi)
{
  /* Mask the TE */
  __DSI_MASK_TE();
  
  /* Refresh the right part of the display */
  HAL_DSI_Refresh(hdsi);   
}

/**
  * @brief  End of Refresh DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_EndOfRefreshCallback(DSI_HandleTypeDef *hdsi)
{
  if(pending_buffer > 0)
  {
    if(active_area == LEFT_AREA)
    {     
      /* Disable DSI Wrapper */
      __HAL_DSI_WRAPPER_DISABLE(hdsi);
      /* Update LTDC configuration */
      LTDC_LAYER(&hltdc_eval, 0)->CFBAR = LCD_FRAME_BUFFER + 400 * 4;
      __HAL_LTDC_RELOAD_CONFIG(&hltdc_eval);
      /* Enable DSI Wrapper */
      __HAL_DSI_WRAPPER_ENABLE(hdsi);
      
      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColRight);
      /* Refresh the right part of the display */
      HAL_DSI_Refresh(hdsi);    
      
    }
    else if(active_area == RIGHT_AREA)
    {

      /* Disable DSI Wrapper */
      __HAL_DSI_WRAPPER_DISABLE(&hdsi_eval);
      /* Update LTDC configuration */
      LTDC_LAYER(&hltdc_eval, 0)->CFBAR = LCD_FRAME_BUFFER;
      __HAL_LTDC_RELOAD_CONFIG(&hltdc_eval);
      /* Enable DSI Wrapper */
      __HAL_DSI_WRAPPER_ENABLE(&hdsi_eval);
      
      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft); 
      pending_buffer = 0;
    }
    
      active_area = (active_area == LEFT_AREA)? RIGHT_AREA : LEFT_AREA; 
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

static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();
  
  /* Configure the MPU as Strongly ordered for not defined regions */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = SDRAM_DEVICE_ADDR;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes FMC control registers */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0xA0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_8KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
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
  *            PLL_Q                          = 9
  *            PLL_R                          = 7
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
  RCC_OscInitStruct.PLL.PLLR = 7;
  
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

#ifdef USE_FULL_ASSERT

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

/**
 * @}
 */

/**
 * @}
 */


