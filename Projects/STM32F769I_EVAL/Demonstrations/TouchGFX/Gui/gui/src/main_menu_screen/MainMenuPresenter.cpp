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
  


#include <gui/main_menu_screen/MainMenuPresenter.hpp>
#include <gui/main_menu_screen/MainMenuView.hpp>

MainMenuPresenter::MainMenuPresenter(MainMenuView& v) :
    DemoPresenter(v),
    view(v)
{
}

void MainMenuPresenter::activate()
{

}

void MainMenuPresenter::deactivate()
{

}

void MainMenuPresenter::setSelectedDemoScreen(Defines::DemoID elementIndex)
{
    model->setSelectedDemoScreen(elementIndex);
}

void MainMenuPresenter::setPreviousSelectedMenuType(Defines::MainMenuType menuType)
{
    model->setPreviousSelectedMainMenuType(menuType);
}

Defines::DemoID MainMenuPresenter::getSelectedDemoScreen()
{
    return model->getSelectedDemoScreen();
}

Defines::MainMenuType MainMenuPresenter::getPreviousSelectedMenuType()
{
    return model->getPreviousSelectedMainMenuType();
}

void MainMenuPresenter::screenSaverMinorTick()
{
    view.screenSaverMinorTick();
}

void MainMenuPresenter::gotoSTMenu()
{
    // TODO Goto ST menu
#ifndef SIMULATOR
  __HAL_RCC_RTC_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_BKPSRAM_CLK_ENABLE(); 
  
  HAL_PWR_EnableBkUpAccess();

 *(uint32_t *)(0x40024000) = 0x5AA55BBB;      
  NVIC_SystemReset();
#endif
  
}
