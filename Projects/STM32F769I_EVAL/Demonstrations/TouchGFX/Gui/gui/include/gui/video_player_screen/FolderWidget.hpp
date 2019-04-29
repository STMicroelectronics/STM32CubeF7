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
  

#ifndef VIDEO_PLAYER_FOLDERWIDGET_HPP
#define VIDEO_PLAYER_FOLDERWIDGET_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/ListLayout.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

#include <gui/common/Playlist.hpp>
#include <gui/common/CommonDefinitions.hpp>
#include <gui/video_player_screen/FileInput.hpp>

using namespace touchgfx;

class PlaylistManager
{
public:
    virtual bool addFile(uint32_t index) = 0;
    virtual void removeFile(uint32_t index) = 0;
    virtual void folderSelected(uint32_t index) = 0;
    virtual void upPressed() = 0;
    virtual void donePressed() = 0;
    virtual ~PlaylistManager() {};
};

class FolderListElement : public Container
{
public:
    FolderListElement();
    void configure(bool isDirectory);
    void setSelected(bool selected);
    virtual void handleClickEvent(const ClickEvent& event);

    TextAreaWithOneWildcard name;
    Unicode::UnicodeChar nameBuf[FILENAME_LENGTH];
    Image background; // visible when selected
    Image icon;
    Image button;
    uint32_t index;
    bool isDirectory;
    bool isIncluded;
    bool isSelected;
    bool clickCancelled;

    PlaylistManager* playlistManager;
    static GenericCallback< const uint32_t >* itemPressedAction;
};

class FolderWidget : public Container
{
public:
    FolderWidget(PlaylistManager&);
    void setPlaylist(PlaylistElement* playlist, uint32_t length);
    void setupDirlist(const char* const foldername, fileinput::DirEntry* dirlist, uint32_t length);
private:
    Image background;
    Image shadow;
    Image titlebar;
    TextAreaWithOneWildcard titleText;
    Unicode::UnicodeChar folderNameBuf[FOLDERNAME_LENGTH];    

    ScrollableContainer scrollCnt;
    ListLayout list;
    FolderListElement listElements[DIRLIST_LENGTH];

    Image bottombar;
    Button backButton;
    Button okButton;
    uint32_t selected;
    
    PlaylistManager& playlistManager;

    PlaylistElement* playlist;
    uint32_t playlistLength;
    bool isOnPlaylist(const char* const foldername, fileinput::DirEntry& direntry);

    Callback<FolderWidget, const AbstractButton&> buttonCallback;
    void onButtonPressed(const AbstractButton& button);
    Callback<FolderWidget, const uint32_t> itemCallback;
    void onItemPressed(const uint32_t index);
};

#endif

