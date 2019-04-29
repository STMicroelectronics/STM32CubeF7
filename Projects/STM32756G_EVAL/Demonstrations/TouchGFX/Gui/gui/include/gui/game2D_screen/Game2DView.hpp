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
  


#ifndef GAME_2D_VIEW_HPP
#define GAME_2D_VIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/game2D_screen/Game2DPresenter.hpp>
#include <gui/game2D_screen/CollectibleItem.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/ToggleButton.hpp>
#include <gui/game2D_screen/ParallaxBackground.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>

class Game2DView : public DemoView<Game2DPresenter>
{
public:
    Game2DView();
    virtual ~Game2DView();

    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

private:
    static const int NUMBER_OF_COLLECTIBLE_ITEMS = 5;

    // With framerate compensation enabled, we need to slow down animation a bit on both
    // of the supported boards( STM32F429-EVAL, STM32F756-EVAL)
    static const int INITIAL_ANIMATION_MODIFIER = 2;

    int animationModifier;

    Image background;

    Container gameArea;
    ParallaxBackground parallaxBackground;

    MoveAnimator<AnimatedImage> playerCharacter;
    int playerCharacterStartX;
    int playerCharacterStartY;
    int playerCharacterMaxY;


    CollectibleItem collectibleItem[NUMBER_OF_COLLECTIBLE_ITEMS];
    CollectibleItem bullet;

    bool chromArtOff;

    Button flyButton;
    Button chromArtOnOffButton;

    TextArea flyText;
    TextArea chromArtText;

    TextAreaWithOneWildcard scoreTxt;
    Unicode::UnicodeChar scoreTxtbuf[16];

    int velocity;
    int collectibleItemCatched;
    int collectibleItemMissed;
    int playerDeadCounter;

    Callback<Game2DView, const AbstractButton&> onButtonPressed;
    void buttonPressedhandler(const AbstractButton& button);

    void restartGame();
    void updateScoreText();
    void updateChromArtButtonBitmaps();
};

#endif // GAME_2D_VIEW_HPP
