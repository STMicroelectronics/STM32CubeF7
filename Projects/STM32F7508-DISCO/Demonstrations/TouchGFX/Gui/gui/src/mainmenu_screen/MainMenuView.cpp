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
  #include <gui/mainmenu_screen/MainMenuView.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>

MainMenuView::MainMenuView() :
    menuStateChangeInitiatedCallback(this, &MainMenuView::menuStateChangeInitiatedHandler),
    menuStateChangeEndedCallback(this, &MainMenuView::menuStateChangeEndedHandler)
{
}

MainMenuView::~MainMenuView()
{
}

void MainMenuView::setupScreen()
{
    backgroundLeft.setBitmap(Bitmap(BITMAP_MENU_STAGE_STRETCH_LEFT_SIDE_ID));
    backgroundLeft.setXY(0, 0);
    add(backgroundLeft);

    backgroundRight.setBitmap(Bitmap(BITMAP_MENU_STAGE_STRETCH_RIGHT_SIDE_ID));
    backgroundRight.setXY(HAL::DISPLAY_WIDTH - backgroundRight.getWidth(), 0);
    add(backgroundRight);

    menuContainer.setXY(0, 0);
    menuContainer.setSwipeCutoff(80);
    menuContainer.setEndSwipeElasticWidth(50);
    menuContainer.setDotIndicatorBitmaps(Bitmap(BITMAP_SCREEN_SWIPE_DOTS_INACTIVE_ID), Bitmap(BITMAP_SCREEN_SWIPE_DOTS_ACTIVE_ID));
    menuContainer.setDotIndicatorXYWithCenteredX(HAL::DISPLAY_WIDTH / 2, HAL::DISPLAY_HEIGHT - 16);
    menuContainer.setMenuChangeInitiatedCallback(menuStateChangeInitiatedCallback);
    menuContainer.setMenuChangeEndedCallback(menuStateChangeEndedCallback);
    menuContainer.setTouchable(false);

    demoScreenButton00.setTouchable(false);
    demoScreenButton01.setTouchable(false);
    demoScreenButton02.setTouchable(false);
    demoScreenButton03.setTouchable(false);

    // Containers was added to the view by default
    // Remove them and add to container instead
    remove(demoScreensContainer00);
    remove(demoScreensContainer01);
    remove(demoScreensContainer02);
    remove(demoScreensContainer03);

    menuContainer.add(demoScreensContainer00);
    menuContainer.add(demoScreensContainer01);
    menuContainer.add(demoScreensContainer02);
    menuContainer.add(demoScreensContainer03);
    add(menuContainer);

    // Add mcuLoad on top
    remove(mcuLoad);
    add(mcuLoad);

    demoScreenIndex[0] = MainMenuPresenter::CUSTOM_CONTROLS;
    demoScreenIndex[1] = MainMenuPresenter::EASING_EQUATION;
    demoScreenIndex[2] = MainMenuPresenter::PRODUCT_PRESENTER;
    demoScreenIndex[3] = MainMenuPresenter::GRAPH_SCREEN;
}

void MainMenuView::afterTransition()
{
    menuContainer.setTouchable(true);
    demoScreenButton00.setTouchable(true);
    demoScreenButton01.setTouchable(true);
    demoScreenButton02.setTouchable(true);
    demoScreenButton03.setTouchable(true);
}

void MainMenuView::tearDownScreen()
{
    uint8_t demoIndex = menuContainer.getSelectedScreen();
    presenter->demoSelected(demoScreenIndex[demoIndex], demoIndex);
}

void MainMenuView::setSelectedMenuIndex(uint8_t demoIndex)
{
    menuContainer.setSelectedScreen(demoIndex);
}

void MainMenuView::menuStateChangeInitiatedHandler(const SwipeContainer& sc)
{
}

void MainMenuView::menuStateChangeEndedHandler(const SwipeContainer& sc)
{
}

void MainMenuView::updateProcessorLoad(uint8_t mcuLoadValue)
{
    mcuLoad.updateProcessorLoad(mcuLoadValue);
}
