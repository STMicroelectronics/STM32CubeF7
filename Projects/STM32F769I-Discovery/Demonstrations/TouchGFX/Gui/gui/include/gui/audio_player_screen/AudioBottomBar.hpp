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
