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
  

#ifndef AUDIO_PLAYER_PLAYLIST_HPP
#define AUDIO_PLAYER_PLAYLIST_HPP

#include <gui/common/CommonDefinitions.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/Button.hpp>

using namespace touchgfx;

class AudioPlayerPlaylistElement : public Container
{
public:
    AudioPlayerPlaylistElement();
    virtual ~AudioPlayerPlaylistElement();
    void setSelectedCallback(GenericCallback<int>& callback);
    void setRemoveCallback(GenericCallback<AudioPlayerPlaylistElement&>& callback);
    void setPlaying(bool play);
    void setLineVisibility(bool visible);
    void setCover(Bitmap& bitmapId);
    void set(int index, BitmapId cover, char* artist, char* song);

    virtual void handleClickEvent(const ClickEvent& evt);
private:
    GenericCallback<int>* selectedCallback;
    GenericCallback<AudioPlayerPlaylistElement&>* removeCallback;

    Image cover;
    Image coverShadow;
    Image coverOverlay;
    Button remove;
    Image line;
    TextAreaWithOneWildcard numberText;
    Unicode::UnicodeChar numberTextBuffer[3];
    TextAreaWithOneWildcard songText;
    Unicode::UnicodeChar songTextBuffer[50];
    TextAreaWithOneWildcard artistText;
    Unicode::UnicodeChar artistTextBuffer[50];

    int playlistIndex;
    bool pressed;
};

class AudioPlayerPlaylist : public Container
{
public:
    AudioPlayerPlaylist();

    void setSelectedCallback(GenericCallback<int>& callback);
    void clear(); //todo
    void addSong(BitmapId cover, char* artist, char* song);
    void setPlaying(int index);
private:
    Image background;
    Image shadow;
    ScrollableContainer scroll;

    int numberOfElements;
    const static int maxElements = 20;
    AudioPlayerPlaylistElement elements[maxElements];

    Callback<AudioPlayerPlaylist, AudioPlayerPlaylistElement&> onRemove;
    void removeElement(AudioPlayerPlaylistElement& element);
};

#endif
