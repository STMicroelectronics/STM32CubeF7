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
  


#include <gui/main_menu_screen/MainMenuView.hpp>
#include <BitmapDatabase.hpp>

MainMenuView::MainMenuView() :
    selectedDemoScreen(Defines::GAME2048),
    onMenuButtonPressed(this, &MainMenuView::menuButtonPressedHandler)
{
    // GotoSTMenuButton must be added by the specialized menu view (to get correct z-order)
    gotoSTMenuButton.setBitmaps(Bitmap(BITMAP_ST_MENU_BUTTON_ID), Bitmap(BITMAP_ST_MENU_BUTTON_PRESSED_ID));
    gotoSTMenuButton.setXY(35, 26);
    gotoSTMenuButton.setAction(onMenuButtonPressed);

    // ToggleMenuButton must be added by the specialized menu view (to get correct z-order)
    toggleMenuButton.setBitmaps(Bitmap(BITMAP_MENU_ICON_ID), Bitmap(BITMAP_MENU_ICON_PRESSED_ID));
    toggleMenuButton.setXY(21, 14);
    toggleMenuButton.setAction(onMenuButtonPressed);

    // MCULoad must be added by the specialized menu view (to get correct z-order)
    mcuLoadTxt.setXY(620, 25);
    mcuLoadTxt.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    mcuLoadValueTxt.setPosition(mcuLoadTxt.getRect().right() - 6, mcuLoadTxt.getY(), 50, 30);
    mcuLoadValueTxt.setColor(mcuLoadTxt.getColor());
}

MainMenuView::~MainMenuView()
{
}

void MainMenuView::setSelectedDemoScreen(Defines::DemoID demoId)
{
    selectedDemoScreen = demoId;
}

void MainMenuView::gotoSelectedDemoScreen()
{
    presenter->setSelectedDemoScreen(selectedDemoScreen);

    switch (selectedDemoScreen)
    {
    case Defines::GAME2048:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoGame2048Screen();
        break;
    case Defines::GAME2D:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoGame2DScreen();
        break;
    case Defines::CONTROLS:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoControlsScreen();
        break;
    case Defines::AUDIO_PLAYER:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoAudioPlayerScreen();
        break;
    case Defines::HOME_AUTOMATION:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoHomeAutomationScreen();
        break;
    case Defines::SETTINGS:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoSettingsScreen();
        break;
    case Defines::VIDEO_PLAYER:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoVideoPlayerScreen();
        break;
    case Defines::NUMBER_OF_DEMO_SCREENS:
    case Defines::NO_DEMO_SCREEN:
    default:
        break;
    }
}

void MainMenuView::menuButtonPressedHandler(const AbstractButton& button)
{
    if (&button == &toggleMenuButton)
    {
        // Clear the previous selected menu that is used for returning from
        // a demo screen to the menu. But when toggling between menus this
        // is not needed.
        presenter->setPreviousSelectedMenuType(Defines::NO_MENU);
        presenter->setSelectedDemoScreen(demoIDs[0]);
        toggleMenu();
    }
    else if (&button == &gotoSTMenuButton)
    {
        presenter->gotoSTMenu();
    }
}
