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
  

#ifndef AUDIO_PLAYER_PRESENTER_HPP
#define AUDIO_PLAYER_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>
#include <gui/common/Playlist.hpp>
#include <gui/common/CommonDefinitions.hpp>
#include <gui/video_player_screen/FileInput.hpp>
#include <gui/audio_player_screen/AudioPlayer.hpp>
#include <gui/audio_player_screen/CoverElement.hpp>
#include <gui/audio_player_screen/AudioFolderWidget.hpp>

using namespace touchgfx;

class AudioPlayerView;

class AudioPlayerPresenter : public DemoPresenter/*, AudioPlaylistManager*/
{
public:
    AudioPlayerPresenter(AudioPlayerView& v);
    virtual ~AudioPlayerPresenter() {};

    virtual void activate();
    void initializeFileSystem(); 
    virtual void deactivate();

    void playPauseClicked();
    void selectSong(int index);
    void selectPreviousSong();
    void setSongProgressPercentage(uint32_t prog);
    bool isPlaylistEmpty();
    void selectNextSong();
    void setVolume(int val);
    uint32_t getSongProgressMs();
    bool isAudioPlaying()
    {
        return player.isPlaying();
    }
    AudioPlayer* getPlayer()
    {
        return &player;
    }
    PlaylistElement* getPlayList()
    {
        return &playlist[0];
    }

    bool addFile(uint32_t index);
    bool removeFile(uint32_t index);
    bool fileAdded(uint32_t index);


    /* Events from Model/backend */
    virtual void notifyAudioEndOfFile();
    virtual void notifyMediaDisconnected();
    virtual void notifyMediaConnected();

    void backPressed();
    void upPressed();
    void donePressed();
    void folderSelected(uint32_t index);
    void resetDirectoryList();

private:

    void getFolder(char* file, char* buffer, uint32_t max);
    CoverElement covers[PLAYLIST_LENGTH];
    PlaylistElement playlist[PLAYLIST_LENGTH];
    fileinput::DirEntry filelist[DIRLIST_LENGTH];
    fileinput::DirEntry dirlist[DIRLIST_LENGTH];

    AudioPlayer player;
    char currentFolder[50];
    char currentSong[50];
    AudioPlayerPresenter();
    void readCover(CoverElement& cover);
    void refreshPlaylist();
    void updateActiveSong();
    AudioPlayerView& view;
    uint32_t currentPlayListItem;
};

#endif // AUDIO_PLAYER_PRESENTER_HPP
