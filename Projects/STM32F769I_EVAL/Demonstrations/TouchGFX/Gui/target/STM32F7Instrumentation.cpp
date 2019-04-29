/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.10.0 distribution.
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
  


#include <STM32F7Instrumentation.hpp>
#include <touchgfx/hal/HAL.hpp>

#include "stm32f7xx.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_qspi.h"
#include "stm32f7xx_hal_rcc_ex.h"
#include "stm32f7xx_hal_tim.h"

namespace touchgfx
{
TIM_HandleTypeDef tim;

void STM32F7Instrumentation::init()
{
    __TIM2_CLK_ENABLE();
    tim.Instance = TIM2;
    tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    tim.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim.Init.Period = 0xFFFFFFFF;
    tim.Init.Prescaler = 0;
    tim.Init.RepetitionCounter = 1;
    HAL_TIM_Base_Init(&tim);
    HAL_TIM_Base_Start(&tim);
}

//Board specific clockfrequency
unsigned int STM32F7Instrumentation::getElapsedUS(unsigned int start, unsigned int now, unsigned int clockfrequency)
{
    return ((now - start) + (clockfrequency / 2)) / clockfrequency;
}


unsigned int STM32F7Instrumentation::getCPUCycles()
{
    return __HAL_TIM_GET_COUNTER(&tim) * 3;
}

void STM32F7Instrumentation::setMCUActive(bool active)
{
    if (active) //idle task sched out
    {
        uint32_t current_cc = cc_in;
        cc_consumed += getCPUCycles() - current_cc;
    }
    else //idle task sched in
    {
        cc_in = getCPUCycles();
    }
}

}
