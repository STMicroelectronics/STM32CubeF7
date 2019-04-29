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
  #include <touchgfx/hal/OSWrappers.hpp>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <touchgfx/hal/GPIO.hpp>
#include <touchgfx/hal/HAL.hpp>

using namespace touchgfx;

static xSemaphoreHandle frame_buffer_sem;
static xQueueHandle vsync_q = 0;

// Just a dummy value to insert in the VSYNC queue.
static uint8_t dummy = 0x5a;

void OSWrappers::initialize()
{
    vSemaphoreCreateBinary(frame_buffer_sem);
    // Create a queue of length 1
    vsync_q = xQueueGenericCreate(1, 1, 0);
}

void OSWrappers::takeFrameBufferSemaphore()
{
    xSemaphoreTake(frame_buffer_sem, portMAX_DELAY);
}
void OSWrappers::giveFrameBufferSemaphore()
{
    xSemaphoreGive(frame_buffer_sem);
}

void OSWrappers::tryTakeFrameBufferSemaphore()
{
    xSemaphoreTake(frame_buffer_sem, 0);
}

void OSWrappers::giveFrameBufferSemaphoreFromISR()
{
    // Since this is called from an interrupt, FreeRTOS requires special handling to trigger a
    // re-scheduling. May be applicable for other OSes as well.
    portBASE_TYPE px = pdFALSE;
    xSemaphoreGiveFromISR(frame_buffer_sem, &px);
    portEND_SWITCHING_ISR(px);
}

void OSWrappers::signalVSync()
{
    if (vsync_q)
    {
        // Since this is called from an interrupt, FreeRTOS requires special handling to trigger a
        // re-scheduling. May be applicable for other OSes as well.
        portBASE_TYPE px = pdFALSE;
        xQueueSendFromISR(vsync_q, &dummy, &px);
        portEND_SWITCHING_ISR(px);
    }
}

void OSWrappers::waitForVSync()
{
    // First make sure the queue is empty, by trying to remove an element with 0 timeout.
    xQueueReceive(vsync_q, &dummy, 0);

    // Then, wait for next VSYNC to occur.
    xQueueReceive(vsync_q, &dummy, portMAX_DELAY);
}

void OSWrappers::taskDelay(uint16_t ms)
{
    vTaskDelay(ms);
}

static portBASE_TYPE IdleTaskHook(void* p)
{
    if ((int)p) //idle task sched out
    {
        touchgfx::HAL::getInstance()->setMCUActive(true);
    }
    else //idle task sched in
    {
        touchgfx::HAL::getInstance()->setMCUActive(false);
    }
    return pdTRUE;
}

// FreeRTOS specific handlers
extern "C"
{
    void vApplicationStackOverflowHook(xTaskHandle xTask,
                                       signed portCHAR* pcTaskName)
    {
        while (1);
    }

    void vApplicationMallocFailedHook(xTaskHandle xTask,
                                      signed portCHAR* pcTaskName)
    {
        while (1);
    }

    void vApplicationIdleHook(void)
    {
        // Set task tag in order to have the "IdleTaskHook" function called when the idle task is
        // switched in/out. Used solely for measuring MCU load, and can be removed if MCU load
        // readout is not needed.
        vTaskSetApplicationTaskTag(NULL, IdleTaskHook);
    }
}
