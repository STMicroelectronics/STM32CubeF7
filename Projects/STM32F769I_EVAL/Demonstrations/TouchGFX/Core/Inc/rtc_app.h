/**
  ******************************************************************************
  * @file    rtc_app.h
  * @author  MCD Application Team
  * @brief   Header for rtc_app.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_RTC_H
#define __RTC_RTC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F769xx
#include "stm32f7xx_hal.h"
#else
#include "stm32f4xx_hal.h"
#endif
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void     RTC_CalendarBkupInit(void);
void     RTC_BkupSaveParameter(uint32_t address, uint32_t data);
uint32_t RTC_BkupRestoreParameter(uint32_t address);

void RTC_SetTime  (RTC_TimeTypeDef *Time);
void RTC_GetTime  (RTC_TimeTypeDef *Time);
void RTC_SetDate  (RTC_DateTypeDef *Date);
void RTC_GetDate  (RTC_DateTypeDef *Date);

#ifdef __cplusplus
}
#endif

#endif /*__RTC_RTC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
