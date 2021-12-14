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
  

#include <gui/video_player_screen/VideoPlayerPresenter.hpp>
#include <gui/video_player_screen/VideoPlayerView.hpp>

VideoPlayerPresenter::VideoPlayerPresenter(VideoPlayerView& v) :
    DemoPresenter(v),
    view(v)
{
}

void VideoPlayerPresenter::activate()
{

}

void VideoPlayerPresenter::deactivate()
{

}

void VideoPlayerPresenter::notifyMediaDisconnected()
{
    application()->gotoVideoPlayerScreen();
}

void VideoPlayerPresenter::notifyMediaConnected()
{
    application()->gotoVideoPlayerScreen();
}
