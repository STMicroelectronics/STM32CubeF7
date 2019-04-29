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
  

#ifndef VIDEO_PLAYER_PLAYLIST_HPP
#define VIDEO_PLAYER_PLAYLIST_HPP

#include <touchgfx/mixins/MoveAnimator.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>

#include <gui/common/Playlist.hpp>
#include <gui/common/CommonDefinitions.hpp>

using namespace touchgfx;

class ThumbNail : public Container
{
public:
    ThumbNail();
    void setThumbNail(BitmapId bmpId);
    void setIndex(uint32_t index);
    static void setPressedAction(GenericCallback< const uint32_t >& callback);

private:
    Button thumbNail;
    Image overlay;
    uint32_t index;

    Callback<ThumbNail, const AbstractButton&> thumbCallback;
    void onThumbNailPressed(const AbstractButton&);

    //callback to container
    static GenericCallback< const uint32_t >* pressedAction;
};

class PlaylistWidget : public Container
{
public:
    PlaylistWidget(PlaylistElement* list,
                   GenericCallback< const uint32_t >& movieClickCallback,
                   GenericCallback< const AbstractButton& >& backCallback);

    virtual void setPosition(int16_t x, int16_t y, int16_t width, int16_t height);

    void refreshPlaylist();
    void updateUI();

private:
    PlaylistElement* playlist;
    ScrollableContainer scroller;
    Container holder;
    MoveAnimator<Container> backContainer;
    Image backBackground;
    Image shadow;
    Button backButton;
    ThumbNail thumbNails[PLAYLIST_LENGTH];
    Image background;
    bool backOut;
};

#endif
