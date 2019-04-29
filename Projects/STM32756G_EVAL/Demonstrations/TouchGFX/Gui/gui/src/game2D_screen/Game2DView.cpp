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
  


#include <gui/game2D_screen/Game2DView.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <gui/common/Utils.hpp>

#if defined(ST) && !defined(SIMULATOR)
extern volatile bool disableChromArt;
#endif

Game2DView::Game2DView() :
    DemoView(),
    animationModifier(INITIAL_ANIMATION_MODIFIER),
    parallaxBackground(500, 290),
    chromArtOff(false),
    velocity(-11),
    collectibleItemCatched(0),
    collectibleItemMissed(0),
    playerDeadCounter(0),
    onButtonPressed(this, &Game2DView::buttonPressedhandler)
{
    playerCharacter.setBitmaps(BITMAP_SPRITE_00_ID, BITMAP_SPRITE_09_ID);
    playerCharacter.setUpdateTicksInterval(3 * animationModifier);
}

Game2DView::~Game2DView()
{
}

void Game2DView::setupScreen()
{
    background.setPosition(0, 0, getActiveDisplayWidth(), getActiveDisplayHeight());
    background.setBitmap(Bitmap(BITMAP_GAME_2D_FRAME_BACKGROUND_ID));
    add(background);

    flyButton.setBitmaps(Bitmap(BITMAP_GAME2D_FLY_BUTTON_ID), Bitmap(BITMAP_GAME2D_FLY_BUTTON_PRESSED_ID));
    flyButton.setXY(65, 409);
    flyButton.setAction(onButtonPressed);
    add(flyButton);

    flyText.setTypedText(TypedText(T_GAME_2D_FLY_BUTTON));
    flyText.setXY(flyButton.getRect().right() + 6, flyButton.getY() + 16);
    flyText.setColor(Color::getColorFrom24BitRGB(0x7B, 0xAF, 0xB6));
    add(flyText);

#if defined(ST) && !defined(SIMULATOR)
    updateChromArtButtonBitmaps();
    chromArtOnOffButton.setXY(252, 409);
    chromArtOnOffButton.setAction(onButtonPressed);
    add(chromArtOnOffButton);

    chromArtText.setTypedText(TypedText(T_GAME_2D_CHROM_ART));
    chromArtText.setXY(chromArtOnOffButton.getRect().right() + 6, chromArtOnOffButton.getY() + 8);
    chromArtText.setColor(Color::getColorFrom24BitRGB(0x7B, 0xAF, 0xB6));
    add(chromArtText);
#endif

    gotoMenuButton.setBitmaps(Bitmap(BITMAP_GAME2D_MENU_BUTTON_ID), Bitmap(BITMAP_GAME2D_MENU_BUTTON_PRESSED_ID));
    gotoMenuButton.setXY(570, 98);
    add(gotoMenuButton);

    mcuLoadTxt.setTypedText(TypedText(T_DEMO_VIEW_MCU_LOAD_TEXT_TINY));
    mcuLoadTxt.setXY(10, 124 - mcuLoadTxt.getTypedText().getFont()->getFontHeight());
    mcuLoadTxt.setColor(Color::getColorFrom24BitRGB(0x7B, 0xAF, 0xB6));
    mcuLoadValueTxt.setTypedText(TypedText(T_DEMO_VIEW_MCU_LOAD_VALUE_CENTER));
    mcuLoadValueTxt.setPosition(12, mcuLoadTxt.getY() + 18, 52, 14);
    mcuLoadValueTxt.setColor(mcuLoadTxt.getColor());
    add(mcuLoadTxt);
    add(mcuLoadValueTxt);

    bullet.setBitmaps(BITMAP_SPHERE1_ID, BITMAP_SPHERE1_ID);
    bullet.stopAnimation();
    parallaxBackground.addCollectibleItem(bullet);

    playerCharacterStartX = parallaxBackground.getX() + 36;
    playerCharacterStartY = parallaxBackground.getRect().bottom() - playerCharacter.getHeight() - 14;
    playerCharacterMaxY = parallaxBackground.getY() + 30;

    parallaxBackground.setXY(0, 0);
    parallaxBackground.addPlayerCharacter(playerCharacter);
    parallaxBackground.setActiveTickInterval(animationModifier);

    for (int i = 0; i < NUMBER_OF_COLLECTIBLE_ITEMS; i++)
    {
        collectibleItem[i].setX(parallaxBackground.getRect().right());
        collectibleItem[i].stopAnimation();
        parallaxBackground.addCollectibleItem(collectibleItem[i]);
    }

    gameArea.add(parallaxBackground);
    gameArea.setPosition(70, 115, parallaxBackground.getWidth(), parallaxBackground.getHeight());

    add(gameArea);


    scoreTxt.setWildcard(scoreTxtbuf);
    scoreTxt.setTypedText(TypedText(T_GAME_2D_SCORE));
    scoreTxt.setPosition(gameArea.getRect().right() - 80, 440, 80, 24);
    scoreTxt.setColor(Color::getColorFrom24BitRGB(0x7B, 0xAF, 0xB6));
    add(scoreTxt);
    updateScoreText();

    parallaxBackground.startAnimation();

#if defined(ST) && !defined(SIMULATOR)
    disableChromArt = chromArtOff;
#endif

    restartGame();
}


void Game2DView::tearDownScreen()
{
#if defined(ST) && !defined(SIMULATOR)
    disableChromArt = false;
#endif
}

void Game2DView::buttonPressedhandler(const AbstractButton& button)
{
    if (&button == &chromArtOnOffButton)
    {
        chromArtOff = !chromArtOff;
#if defined(ST) && !defined(SIMULATOR)
        disableChromArt = chromArtOff;
#endif
        animationModifier = INITIAL_ANIMATION_MODIFIER;
        playerCharacter.setUpdateTicksInterval(3 * animationModifier);
        parallaxBackground.setActiveTickInterval(animationModifier);

        //When ChromArt is enabled - reduce sample rate specifically for F756 touch controller
#if STM32F756xx
        if (!disableChromArt)
        {
            HAL::getInstance()->setTouchSampleRate(2);
        }
        else
        {
            HAL::getInstance()->setTouchSampleRate(1);
        }
#endif

        updateChromArtButtonBitmaps();
    }
}

void Game2DView::handleTickEvent()
{
    // Stop collectible items if they have passed the left border
    for (int i = 0; i < NUMBER_OF_COLLECTIBLE_ITEMS; i++)
    {
        if (collectibleItem[i].getRect().right() < 0)
        {
            collectibleItem[i].setXY(parallaxBackground.getRect().right(), parallaxBackground.getY() + 50 + Utils::nextRand(0, 180));
            collectibleItem[i].stopAnimation();

            if (playerDeadCounter == 0)
            {
                collectibleItemMissed++;
                updateScoreText();
            }
        }
    }
    if (bullet.getRect().right() < 0)
    {
        bullet.setXY(parallaxBackground.getRect().right(), parallaxBackground.getY() + 50 + Utils::nextRand(0, 180));
        bullet.stopAnimation();
    }

    // Start new collectible items
    if (Utils::nextRand(0, 100) < 8)
    {
        for (int i = 0; i < NUMBER_OF_COLLECTIBLE_ITEMS; i++)
        {
            if (!collectibleItem[i].isRunning())
            {
                collectibleItem[i].setXY(parallaxBackground.getRect().right(), parallaxBackground.getY() + 50 + Utils::nextRand(0, 180));
                collectibleItem[i].startAnimation(Utils::nextRand(4, 14) / animationModifier);
                break;
            }
        }
    }

    // Dead time period timeout
    if (playerDeadCounter > 0)
    {
        playerDeadCounter++;

        if (playerDeadCounter > 100)
        {
            restartGame();
        }
        return;
    }

    // Start new bullet
    if (Utils::nextRand(0, 100) < 3)
    {
        if (!bullet.isRunning())
        {
            bullet.setXY(parallaxBackground.getRect().right(), parallaxBackground.getY() + 50 + Utils::nextRand(0, 180));
            bullet.startAnimation(Utils::nextRand(14, 20) / animationModifier);
        }
    }

    // Collect collectible items
    Rect collisionArea(playerCharacter.getRect().x, playerCharacter.getRect().y + 10, playerCharacter.getRect().width - 20, playerCharacter.getRect().height - 10);
    for (int i = 0; i < NUMBER_OF_COLLECTIBLE_ITEMS; i++)
    {
        if (collectibleItem[i].getRect().intersect(collisionArea))
        {
            collectibleItem[i].setX(parallaxBackground.getRect().right());
            collectibleItem[i].stopAnimation();
            collectibleItemCatched++;
            updateScoreText();
        }
    }



    if (bullet.getRect().intersect(collisionArea))
    {
        // Collision with bullet
        playerCharacter.stopAnimation();
        playerCharacter.startMoveAnimation(playerCharacter.getX() - 160, playerCharacter.getY() + 100, 40 * animationModifier, EasingEquations::quadEaseOut, EasingEquations::quadEaseIn);
        playerDeadCounter = 1;
        bullet.setX(-bullet.getWidth());
        bullet.invalidate();
    }
    else
    {
        // The PlayerCharacter can only be controlled when it has arrived at
        // its start position
        if (playerCharacter.getX() == playerCharacterStartX)
        {
            // Control player
            if (flyButton.getPressedState())
            {
                velocity += 3 / animationModifier;
                velocity = (velocity > 7) ? 7 : velocity;
            }
            else if (velocity > -10)
            {
                velocity -= 2 / animationModifier;
            }

            if (velocity != 0)
            {
                int newY = playerCharacter.getY() - velocity;

                if (newY < playerCharacterMaxY)
                {
                    newY = playerCharacterMaxY;
                }
                else if (newY > playerCharacterStartY)
                {
                    newY = playerCharacterStartY;
                }

                playerCharacter.moveTo(playerCharacter.getX(), newY);
            }
        }
    }

}

void Game2DView::updateScoreText()
{
    Unicode::snprintf(scoreTxtbuf, 16, "%i\0", (collectibleItemCatched * 2) - collectibleItemMissed);
    scoreTxt.invalidate();
}

void Game2DView::updateChromArtButtonBitmaps()
{
    if (chromArtOff)
    {
        chromArtOnOffButton.setBitmaps(Bitmap(BITMAP_GAME2D_ST_BUTTON_OFF_ID), Bitmap(BITMAP_GAME2D_ST_BUTTON_OFF_PRESSED_ID));
    }
    else
    {
        chromArtOnOffButton.setBitmaps(Bitmap(BITMAP_GAME2D_ST_BUTTON_ON_ID), Bitmap(BITMAP_GAME2D_ST_BUTTON_PRESSED_ID));
    }
    chromArtOnOffButton.invalidate();
}

void Game2DView::restartGame()
{
    playerDeadCounter = 0;
    collectibleItemCatched = 0;
    collectibleItemMissed = 0;

    playerCharacter.setXY(-playerCharacter.getWidth(), playerCharacterStartY);
    playerCharacter.startMoveAnimation(playerCharacterStartX, playerCharacterStartY, 32 * animationModifier, EasingEquations::cubicEaseOut);
    playerCharacter.startAnimation(false, true, true);
    playerCharacter.invalidate();
    updateScoreText();
}
