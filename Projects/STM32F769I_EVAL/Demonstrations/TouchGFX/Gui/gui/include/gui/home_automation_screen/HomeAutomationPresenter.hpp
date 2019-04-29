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
  

#ifndef HOME_AUTOMATION_PRESENTER_HPP
#define HOME_AUTOMATION_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class HomeAutomationView;

class HomeAutomationPresenter : public DemoPresenter
{
public:
    HomeAutomationPresenter(HomeAutomationView& v);

    virtual void activate();
    virtual void deactivate();

    virtual ~HomeAutomationPresenter() {};

private:
    HomeAutomationPresenter();

    HomeAutomationView& view;
};

#endif // HOME_AUTOMATION_PRESENTER_HPP
