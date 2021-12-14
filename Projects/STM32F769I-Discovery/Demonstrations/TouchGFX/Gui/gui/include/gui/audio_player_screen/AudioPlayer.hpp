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
  

#ifndef AUDIO_PLAYER_HPP
#define AUDIO_PLAYER_HPP

#include <touchgfx/hal/Types.hpp>

class AudioPlayer
{
public:
#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
    typedef enum
    {
        _100HZ,
        _1KHZ,
        _10KHZ,
        _20KHZ,
        LOUDNESS
    } EqualizerSetting;
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */
    AudioPlayer();
    ~AudioPlayer() {}
    void init(int volume);
    void deInit();
    void selectFile(char* file);
    void play();
    void stop();
    void pause();
    void setVolume(int volume);
    bool isPlaying() const;
    bool isPaused() const;
#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
    int16_t getEqualizerSetting(const EqualizerSetting);
    void setEqualizerSetting(const EqualizerSetting, const int16_t);
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */
    uint32_t getSongProgressMs();
    uint32_t getSongDurationMs();
    void setSongProgressPercentage(uint32_t prog);
private:
    char* filename;
};
#endif /* AUDIO_PLAYER_HPP */
