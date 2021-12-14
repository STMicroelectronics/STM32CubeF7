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
