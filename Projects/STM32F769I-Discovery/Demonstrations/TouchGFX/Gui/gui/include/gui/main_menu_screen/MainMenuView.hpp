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
  


#ifndef MAIN_MENU_VIEW_HPP
#define MAIN_MENU_VIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/main_menu_screen/MainMenuPresenter.hpp>
#include <gui/common/Defines.hpp>

#define EMBED_IN_ST_DEMO true
class MainMenuView : public DemoView<MainMenuPresenter>
{
public:
    MainMenuView();
    virtual ~MainMenuView();

    virtual void setupScreen() = 0;
    virtual void tearDownScreen() = 0;

    virtual void screenSaverMinorTick() = 0;
protected:

    Defines::DemoID demoIDs[Defines::NUMBER_OF_DEMO_SCREENS];
    Defines::DemoID selectedDemoScreen;

    Button toggleMenuButton;
    Button gotoSTMenuButton;

    Callback<MainMenuView, const AbstractButton&> onMenuButtonPressed;
    void menuButtonPressedHandler(const AbstractButton& button);

    void setSelectedDemoScreen(Defines::DemoID demoId);
    void gotoSelectedDemoScreen();

    virtual void toggleMenu() = 0;
};

#endif // MAIN_MENU_VIEW_HPP
