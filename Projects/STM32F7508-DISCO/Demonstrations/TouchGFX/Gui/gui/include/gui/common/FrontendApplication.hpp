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
  #ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <gui_generated/common/FrontendApplicationBase.hpp>

class FrontendHeap;

using namespace touchgfx;

class FrontendApplication : public FrontendApplicationBase
{
public:
    FrontendApplication(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplication() { }

    virtual void handleTickEvent();
    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);

    void resetScreenSaver();

private:
    static const int SCREEN_SAVER_TIMEOUT = 180000; // Milliseconds
    static const int MINOR_TICK_FREQ = 3000 / 17; // Ticks - so Milliseconds / frequency (ms per frame)
    static const int MAJOR_TICK_FREQ = 10000 / 17; // Ticks - so Milliseconds / frequency (ms per frame)

    int tickCounter;
    int screenSaverTick;
    Time lastClickTime;
};

#endif // FRONTENDAPPLICATION_HPP
