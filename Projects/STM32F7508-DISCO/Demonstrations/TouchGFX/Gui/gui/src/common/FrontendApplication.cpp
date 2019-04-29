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
  #include <gui/common/FrontendApplication.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap) :
    FrontendApplicationBase(m, heap),
    tickCounter(0),
    screenSaverTick(0),
    lastClickTime(0)
{
}

void FrontendApplication::handleTickEvent()
{
    model.tick();
    FrontendApplicationBase::handleTickEvent();

    tickCounter++;

    // Screen saver functionality:
    if (lastClickTime.hasValue() && (model.getCurrentTime() - lastClickTime) >= SCREEN_SAVER_TIMEOUT)
    {
        screenSaverTick++;
        if (screenSaverTick % MINOR_TICK_FREQ == 0)
        {
            model.screenSaverMinorTick();
        }
        if (screenSaverTick % MAJOR_TICK_FREQ == 0)
        {
            model.screenSaverMajorTick();
            resetScreenSaver();
        }
    }
}

void FrontendApplication::handleClickEvent(const ClickEvent& evt)
{
    MVPApplication::handleClickEvent(evt);

    // A click event has been registered so reset last click time
    resetScreenSaver();
}

void FrontendApplication::handleDragEvent(const DragEvent& evt)
{
    MVPApplication::handleDragEvent(evt);

    // A drag event has been registered so reset last click time
    resetScreenSaver();
}

void FrontendApplication::resetScreenSaver()
{
    lastClickTime = model.getCurrentTime();
    screenSaverTick = 0;
}
