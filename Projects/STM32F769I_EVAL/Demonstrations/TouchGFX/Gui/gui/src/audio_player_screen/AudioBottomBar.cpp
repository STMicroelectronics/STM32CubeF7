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
  


#include <gui/audio_player_screen/AudioBottomBar.hpp>

#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

AudioBottomBar::AudioBottomBar() : BottomBar(),
    newVolumeCallback(this, &AudioBottomBar::handleVolumeChanged),
    volumeAction(0),
    playerState(STOPPED),
    currMinutes(0xFFFF),
    currSeconds(0xFFFF)
{
    volumeIcon.setBitmap(Bitmap(BITMAP_VOLUME_ICON_ID));
    volumeIcon.setXY(0, (getHeight() - volumeIcon.getHeight()) / 2);

    volumeSlider.setXY(volumeIcon.getX() + volumeIcon.getWidth() - 8, 25);
    volumeSlider.setBitmaps(Bitmap(BITMAP_VOLUME_BAR_ID), Bitmap(BITMAP_VOLUME_BAR_FILL_ID), Bitmap(BITMAP_VOLUME_KNOB_ID));
    volumeSlider.setupHorizontalSlider(10, 9, 0, 0, 170);
    volumeSlider.setValueRange(0, 100);
    volumeSlider.setValue(0);
    volumeSlider.setNewValueCallback(newVolumeCallback);

    timeSeparator.setTypedText(TypedText(T_AUDIO_TIME_SEPARATOR));
    timeSeparator.setXY((getWidth() - timeSeparator.getWidth()) / 2, (getHeight() - timeSeparator.getHeight()) / 2);
    timeSeparator.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));

    currentTimeBuf[0] = 0;
    totalTimeBuf[0] = 0;

    currentTimeText.setTypedText(TypedText(T_AUDIO_CURRENT_TIME));
    currentTimeText.setWildcard(currentTimeBuf);
    Unicode::snprintf(currentTimeBuf, 6, "%02d:%02d", 0, 0);
    currentTimeText.resizeToCurrentText();
    currentTimeText.setXY(timeSeparator.getX() - currentTimeText.getWidth() - 10, timeSeparator.getY());

    totalTimeText.setTypedText(TypedText(T_AUDIO_CURRENT_TIME));
    totalTimeText.setWildcard(totalTimeBuf);
    Unicode::snprintf(totalTimeBuf, 6, "%02d:%02d", 0, 0);
    totalTimeText.resizeToCurrentText();
    totalTimeText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    totalTimeText.setXY(timeSeparator.getX() + timeSeparator.getWidth() + 10, timeSeparator.getY());

    currentTimeText.setVisible(false);
    totalTimeText.setVisible(false);
    timeSeparator.setVisible(false);

    playlistButton.setBitmaps(Bitmap(BITMAP_PLAYLIST_ICON_ID), Bitmap(BITMAP_PLAYLIST_ICON_PRESSED_ID));
    playlistButton.setXY(getWidth() - (20 + 64 * 3), 0);

    folderButton.setBitmaps(Bitmap(BITMAP_FOLDER_ICON_ID), Bitmap(BITMAP_FOLDER_ICON_PRESSED_ID));
    folderButton.setXY(getWidth() - (20 + 64 * 1), 0);

#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
    equalizerButton.setBitmaps(Bitmap(BITMAP_EQUALIZER_ICON_ID), Bitmap(BITMAP_EQUALIZER_ICON_PRESSED_ID));
    equalizerButton.setXY(getWidth() - (20 + 64 * 2), 0);
    equalizerButton.setAction(buttonCallback);
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */

    add(volumeSlider);
    add(volumeIcon);
    add(currentTimeText);
    add(timeSeparator);
    add(totalTimeText);
#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
    add(equalizerButton);
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */
}

void AudioBottomBar::setTotalTime(int minutes, int seconds)
{
    Unicode::snprintf(totalTimeBuf, 6, "%02d:%02d", minutes, seconds);
    totalTimeText.invalidate();
}

void AudioBottomBar::setPlayState(PlayerState state)
{
    if ((playerState == STOPPED || playerState == PAUSED) && state == PLAYING)
    {
        currentTimeText.setVisible(true);
        totalTimeText.setVisible(true);
        timeSeparator.setVisible(true);
        currentTimeText.setColor(Color::getColorFrom24BitRGB(0, 0xC0, 0xFE));
        currentTimeText.invalidate();
        timeSeparator.invalidate();
        totalTimeText.invalidate();
    }
    else if (state == STOPPED)
    {
        setCurrentTime(0, 0);
        currentTimeText.setVisible(false);
        totalTimeText.setVisible(false);
        timeSeparator.setVisible(false);
        currentTimeText.invalidate();
        timeSeparator.invalidate();
        totalTimeText.invalidate();
    }
    else if (state == PAUSED)
    {
        currentTimeText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
        currentTimeText.invalidate();
    }

    playerState = state;
}

void AudioBottomBar::setCurrentTime(int minutes, int seconds)
{
    if (minutes != currMinutes || seconds != currSeconds)
    {
        Unicode::snprintf(currentTimeBuf, 6, "%02d:%02d", minutes, seconds);
        currentTimeText.invalidate();
        currMinutes = minutes;
        currSeconds = seconds;
    }
}
void AudioBottomBar::setVolumeAction(GenericCallback<int>& action)
{
    volumeAction = &action;
}

void AudioBottomBar::buttonClicked(const AbstractButton& btn)
{
#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
    if (&btn == &equalizerButton)
    {
        if (actionCallback && actionCallback->isValid())
        {
            actionCallback->execute(EQUALIZER);
        }
    }
    else
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */
    {
        BottomBar::buttonClicked(btn);
    }
}

void AudioBottomBar::handleVolumeChanged(const Slider& slider, int val)
{
    if (volumeAction && volumeAction->isValid())
    {
        volumeAction->execute(val);
    }
}
void AudioBottomBar::setVolume(int val)
{
    volumeSlider.setValue(val);
}
