/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.10.0 distribution.
  *
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
  

#include <gui/common/FrontendApplication.hpp>
#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <touchgfx/hal/HAL.hpp>

#ifndef SIMULATOR
#include "audio_player_app.h"
#include "storage_app.h"
#include "FreeRTOS.h"
#include "rtc_app.h"

xQueueHandle GUITaskQueue = 0;
extern FATFS USBDISK_FatFs;
extern char USBDISK_Drive[4];  

#define AUDIO_EOF_NOTIFICATION 0x1
#define STORAGE_usb_disk_DISCONNECTED 0x2
#define STORAGE_usb_disk_CONNECTED 0x3
#define STORAGE_USB_DISCONNECTED 0x4
#define STORAGE_USB_CONNECTED 0x5
#endif

#ifdef SIMULATOR
#include <ctime>
#ifndef _MSC_VER
#include <sys/time.h>
#endif /* _MSC_VER*/
#else
RTC_TimeTypeDef RTC_Time;
RTC_DateTypeDef RTC_Date;

extern uint32_t SystemCoreClock;
#endif /* SIMULATOR */

Model::Model() :
    modelListener(0),
    mcuLoadActive(true),
    selectedDemoScreen(Defines::AUDIO_PLAYER),
    previousSelectedMenuType(Defines::ANIMATING_BUTTONS_MENU),
#ifdef USE_STM32769I_DISCO
    scanJunctionTemp(false),
#else
    scanPotentiometer(false),
#endif
    tickCounter(0),
    volume(40),
    brightnessValue(100),
    lastBrightnessValue(100),
    dateYear(0),
    dateMonth(1),
    dateDay(23)
{
#ifndef SIMULATOR
    RTC_GetTime(&RTC_Time);
    RTC_GetDate(&RTC_Date);

    dateYear = RTC_Date.Year;
    if (dateYear > 11)
        dateYear = 0;
    dateMonth = RTC_Date.Month - 1;
    dateDay = RTC_Date.Date - 1;
#endif
}

#ifndef SIMULATOR
//extern volatile uint32_t mcu_load_pct;
static uint8_t mcuLoadLast = 0;
#endif // SIMULATOR

void Model::tick()
{
    tickCounter++;

    lastSecondValue = currentTime.seconds;
#ifdef SIMULATOR
#ifdef _MSC_VER
    time_t rawtime;
    struct tm timenow;
    time(&rawtime);
    localtime_s(&timenow, &rawtime);

    currentTime.hours =   timenow.tm_hour;
    currentTime.minutes = timenow.tm_min;
    currentTime.seconds = timenow.tm_sec;
    currentTime.milliseconds = 0;

#else
    timeval timenow;
    gettimeofday(&timenow, NULL);

    currentTime.hours = (timenow.tv_sec / 60 / 60) % 24;
    currentTime.minutes = (timenow.tv_sec / 60) % 60;
    currentTime.seconds = timenow.tv_sec % 60;
    currentTime.milliseconds = timenow.tv_usec / 1000;
#endif  /*_MSC_VER*/
#else

    uint8_t mcuLoadPct = touchgfx::HAL::getInstance()->getMCULoadPct();
    if (mcuLoadLast != /*mcu_load_pct*/ mcuLoadPct)
    {
        mcuLoadLast = mcuLoadPct;
        modelListener->mcuLoadUpdated(mcuLoadLast);
    }

    uint8_t msg = 0;
    if (xQueueReceive(GUITaskQueue, &msg, 0) == pdPASS)
    {
        if (msg == AUDIO_EOF_NOTIFICATION)
        {
            if (modelListener)
            {
                modelListener->notifyAudioEndOfFile();
            }
        }
    }

    if ((tickCounter % 20) == 0)
    {
      static uint8_t prev_usb_disk_status = 0;
      uint8_t has_sd_card = STORAGE_GetStatus(USB_DISK_UNIT);
      if(prev_usb_disk_status != has_sd_card)
      {
        prev_usb_disk_status = has_sd_card;
        if(prev_usb_disk_status == 0)
        {
          modelListener->notifyMediaDisconnected();
        }
        else
        {
          modelListener->notifyMediaConnected();
        }
      }
    }
#endif /* SIMULATOR */    

#ifdef USE_STM32769I_EVAL
    if ((modelListener != 0) && (scanPotentiometer))
    {
        modelListener->newPotentiometerValue(getPotentiometerValue());
    }
#endif

    if ((tickCounter % 20) == 0)
    {
#ifdef USE_STM32769I_DISCO
      if ((modelListener != 0) && (scanJunctionTemp))
      {
        modelListener->newJunctionTempValue(getTempValue());
      }
#endif

#ifndef SIMULATOR
      RTC_GetTime(&RTC_Time);
      RTC_GetDate(&RTC_Date);

      // TODO - RTC should be used instead
      modelListener->updateTime(RTC_Time.Hours, RTC_Time.Minutes, RTC_Time.Seconds);

      dateYear = RTC_Date.Year;
      dateMonth = RTC_Date.Month - 1;
      dateDay = RTC_Date.Date - 1;
      if (dateYear > 11)
        dateYear = 0;
#else
       modelListener->updateTime(currentTime.hours, currentTime.minutes, currentTime.seconds);
#endif
    }
}

void Model::screenSaverMinorTick()
{
    modelListener->screenSaverMinorTick();
}

void Model::screenSaverMajorTick()
{
    modelListener->screenSaverMajorTick();
}

#ifndef SIMULATOR
#include "stm32f7xx_hal.h"
#ifdef USE_STM32769I_DISCO
int BSP_JunctionTemp_Read();
#include "stm32f769i_discovery.h"
#else
#include "stm32f769i_eval.h"
#endif
#else
#include <stdio.h>
#endif

#ifdef USE_STM32769I_DISCO
extern uint32_t TEMP_SENSOR_GetValue(void);

int Model::getTempValue()
{
#ifndef SIMULATOR
    return TEMP_SENSOR_GetValue();
#else
    return currentTime.getValueInSeconds() % 127;
#endif
}
#endif

#ifdef USE_STM32769I_EVAL
int Model::getPotentiometerValue()
{
#ifndef SIMULATOR
    return BSP_POTENTIOMETER_GetLevel() >> 5;
#else
    return currentTime.getValueInSeconds() % 127;
#endif
}
#endif

#ifndef SIMULATOR
extern "C" void LCD_SetBrightness(int value);
#endif

void Model::setBrightnessValue(uint8_t value)
{
    uint8_t minCutoff = 20;
    uint8_t newValue = minCutoff + (((100 - minCutoff) * value) / 100);
    if (newValue != lastBrightnessValue)
    {
        lastBrightnessValue = newValue;
#ifndef SIMULATOR
        LCD_SetBrightness(newValue);
#else
        printf("New brightness: %d\r\n", newValue);
#endif
    }
}

uint8_t Model::getBrightnessValue()
{
    return brightnessValue;
}

void Model::notifyAudioEndOfFile()
{
    if (modelListener)
    {
        modelListener->notifyAudioEndOfFile();
    }
}


#ifndef SIMULATOR
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_NotifyEndOfFile(void)
{
    AUDIOPLAYER_Stop();
    uint8_t msg = AUDIO_EOF_NOTIFICATION;
    xQueueSend(GUITaskQueue, &msg, 0);
    return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  Notify Storage Unit connection state.
  * @retval None
  */
void  STORAGE_NotifyConnectionChange(uint8_t unit, uint8_t state)
{
  /*
    uint8_t msg = 0;
    if (unit == 0)
    {
        msg = state == 1 ? STORAGE_usb_disk_CONNECTED : STORAGE_usb_disk_DISCONNECTED;
    }
    else
    {
        msg = state == 1 ? STORAGE_USB_CONNECTED : STORAGE_USB_DISCONNECTED;
    }
    xQueueSend(GUITaskQueue, &msg, 0);
  */
}
#endif

void Model::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{
#ifndef SIMULATOR
    RTC_TimeTypeDef cRTC_Time;

    cRTC_Time.Hours = hour;
    cRTC_Time.Minutes = minute;
    cRTC_Time.Seconds = second;
    RTC_SetTime(&cRTC_Time);
#endif
}

void Model::setDate(uint16_t year, uint8_t month, uint8_t day)
{
  dateYear = year; 
  dateMonth = month; 
  dateDay = day; 
#ifndef SIMULATOR
  RTC_DateTypeDef date;
  date.Year = year;
  date.Month = month+1;
  date.Date = day+1;
  date.WeekDay = 1;
  RTC_SetDate(&date);
#endif  
}
