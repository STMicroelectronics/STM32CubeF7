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
    selectedDemoScreenIndex(0),
    onToggleButtonPressed(this, &MainMenuView::toggleButtonPressedHandler)
{
    // ToggleMenuButton must be added by the specialized menu view (to get correct z-order)
    toggleMenuButton.setBitmaps(Bitmap(BITMAP_MENU_TOGGLE_BUTTON_ID), Bitmap(BITMAP_MENU_TOGGLE_BUTTON_PRESSED_ID));
    toggleMenuButton.setXY(20, 20);
    toggleMenuButton.setAction(onToggleButtonPressed);

    // MCULoad must be added by the specialized menu view (to get correct z-order)
    mcuLoadTxt.setXY(516, 20);
    mcuLoadTxt.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    mcuLoadValueTxt.setPosition(mcuLoadTxt.getRect().right(), mcuLoadTxt.getY(), 34, 14);
    mcuLoadValueTxt.setColor(mcuLoadTxt.getColor());
}

MainMenuView::~MainMenuView()
{
}

void MainMenuView::setSelectedDemoScreen(int demoIndex)
{
    selectedDemoScreenIndex = demoIndex;
}

void MainMenuView::gotoSelectedDemoScreen()
{
    presenter->setSelectedDemoScreenIndex(selectedDemoScreenIndex);

    switch (demoIDs[selectedDemoScreenIndex])
    {
    case MainMenuView::PRODUCT_PRESENTER:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoProductPresenterScreen();
        break;
    case MainMenuView::GAME2048:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoGame2048Screen();
        break;
    case MainMenuView::GAME2D:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoGame2DScreen();
        break;
    case MainMenuView::CUSTOM_CONTROLS:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoCustomControlsScreen();
        break;
    case MainMenuView::GRAPH:
        static_cast<FrontendApplication*>(Application::getInstance())->gotoGraphScreen();
        break;
    case MainMenuView::NO_DEMO_SCREEN:
        break;
    default:
        break;
    }
}

void MainMenuView::toggleButtonPressedHandler(const AbstractButton& button)
{
    // Clear the previous selected menu that is used for returning from
    // a demo screen to the menu. But when toggling between menus this
    // is not needed.
    presenter->setPreviousSelectedMenuType(MainMenuPresenter::NO_MENU);
    presenter->setSelectedDemoScreenIndex(0);
    toggleMenu();
}
