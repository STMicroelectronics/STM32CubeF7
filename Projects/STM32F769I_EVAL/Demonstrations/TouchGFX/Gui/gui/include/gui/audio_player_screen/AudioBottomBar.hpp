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
  

#ifndef AUDIO_BOTTOM_BAR_HPP
#define AUDIO_BOTTOM_BAR_HPP

#include <touchgfx/containers/Slider.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <gui/common/BottomBar.hpp>

class AudioBottomBar : public BottomBar
{
public:
    AudioBottomBar();
    virtual ~AudioBottomBar() {}
    void setCurrentTime(int minutes, int seconds);
    void setTotalTime(int minutes, int seconds);
    void setPlayState(PlayerState state);
    void setVolume(int volume);
    void handleVolumeChanged(const Slider& slider, int val);
    void setVolumeAction(GenericCallback<int>& action);
private:
    virtual void buttonClicked(const AbstractButton& btn);

    Callback<AudioBottomBar, const Slider&, int> newVolumeCallback;
    GenericCallback<int>* volumeAction;
    TextAreaWithOneWildcard currentTimeText;
    TextAreaWithOneWildcard totalTimeText;
    TextArea timeSeparator;
    Slider volumeSlider;
    Image volumeIcon;
    Unicode::UnicodeChar currentTimeBuf[6];
    Unicode::UnicodeChar totalTimeBuf[6];
    PlayerState playerState;
#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
    Button equalizerButton;
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */
    int currMinutes;
    int currSeconds;
};

#endif /* AUDIO_BOTTOM_BAR_HPP */
