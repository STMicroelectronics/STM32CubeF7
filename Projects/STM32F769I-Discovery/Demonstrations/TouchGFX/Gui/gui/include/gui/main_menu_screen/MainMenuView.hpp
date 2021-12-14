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
