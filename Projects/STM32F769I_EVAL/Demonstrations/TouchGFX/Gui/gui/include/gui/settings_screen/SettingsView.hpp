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
  

#ifndef SETTINGS_VIEW_HPP
#define SETTINGS_VIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/settings_screen/SettingsPresenter.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>
#include <gui/settings_screen/AnalogClock.hpp>
#include <gui/common/DotIndicator.hpp>
#include <BitmapDatabase.hpp>
#include <gui/settings_screen/ClockSelectorContainer.hpp>
#include <gui/settings_screen/TimeSettingContainer.hpp>
#include <gui/settings_screen/DateSelectorContainer.hpp>


using namespace touchgfx;

class SettingsView : public DemoView<SettingsPresenter>
{
public:
    SettingsView();
    virtual ~SettingsView() { }

    virtual void setupScreen();
    virtual void tearDownScreen();

    void updateTime(uint8_t h, uint8_t m, uint8_t s, bool animate);
    void updateDate(uint16_t year, uint8_t month, uint8_t day);

    void setSelectedClock(int clockIndex);

private:
    enum ActiveContainer
    {
        MAIN_SCREEN,
        CLOCK_SELECTOR,
        TIME_SETTING,
        DATE_SELECTOR,
        NO_ACTIVE_CONTAINER
    } currentActiveContainer;

    ActiveContainer animatingToState;

    Image background;
    MoveAnimator<DotIndicator> dot;
    MoveAnimator<Image> dotBackground;

    TextArea headline;

    MoveAnimator<Button> nextDoneButton;
    FadeAnimator<Button> settingsButton;
    Callback<SettingsView, const AbstractButton&> buttonPressedCallback;
    void buttonPressedHandler(const AbstractButton& button);

    Container containerViewPort;

    MoveAnimator<Container> mainScreenContainer;
    AnalogClock clock;
    Image calendar;
    TextAreaWithOneWildcard mainScreenDateYearText;
    Unicode::UnicodeChar mainScreenDateYearTextBuffer[5];
    TextAreaWithOneWildcard mainScreenDateMonthText;
    Unicode::UnicodeChar mainScreenDateMonthTextBuffer[10];
    TextAreaWithOneWildcard mainScreenDateDayText;
    Unicode::UnicodeChar mainScreenDateDayTextBuffer[5];

    MoveAnimator<Container> settingsContainer;

    ClockSelectorContainer clockSelectorContainer;
    Callback<SettingsView, const ClockSelectorContainer&, int> selectedClockChangedCallback;
    void selectedClockChangedHandler(const ClockSelectorContainer& clockSelector, int selectedClockIndex);

    TimeSettingContainer timeSettingContainer;
    DateSelectorContainer dateSelectorContainer;


    Callback<SettingsView, const MoveAnimator<Container>& > moveAnimationEndedCallback;
    void moveAnimationEndedHandler(const MoveAnimator<Container>& moveAnimator);

    void updateHeadline();
    void animateContainers();

    void setupClock(AnalogClock* analogClock, int clockId);
};

#endif // SETTINGS_VIEW_HPP
