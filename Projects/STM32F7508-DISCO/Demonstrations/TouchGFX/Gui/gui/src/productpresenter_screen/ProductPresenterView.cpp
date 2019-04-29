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
  #include <gui/productpresenter_screen/ProductPresenterView.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <texts/TextKeysAndLanguages.hpp>

ProductPresenterView::ProductPresenterView() :
    selectedPoster(0),
    languageSelectorStateChanged(this, &ProductPresenterView::languageSelectorStateChangedHandler),
    onFadeAnimationEnded(this, &ProductPresenterView::fadeAnimationEndedHandler)
{

}

void ProductPresenterView::setupScreen()
{
    bodyText.setFadeAnimationEndedAction(onFadeAnimationEnded);

    languages[0] = GBR;
    inactiveLanguageButtons[0] = BITMAP_FLAG_GBR_INACTIVE_ID;
    languages[1] = DNK;
    inactiveLanguageButtons[1] = BITMAP_FLAG_DNK_INACTIVE_ID;
    languages[2] = JPN;
    inactiveLanguageButtons[2] = BITMAP_FLAG_JPN_INACTIVE_ID;
    languages[3] = UKR;
    inactiveLanguageButtons[3] = BITMAP_FLAG_UKR_INACTIVE_ID;
    languages[4] = NLD;
    inactiveLanguageButtons[4] = BITMAP_FLAG_NLD_INACTIVE_ID;

    languageSelector.addMenuElement(Bitmap(BITMAP_FLAG_GBR_ACTIVE_ID), Bitmap(BITMAP_FLAG_GBR_ACTIVE_ID));
    languageSelector.addMenuElement(Bitmap(BITMAP_FLAG_DNK_ACTIVE_ID), Bitmap(BITMAP_FLAG_DNK_ACTIVE_ID));
    languageSelector.addMenuElement(Bitmap(BITMAP_FLAG_JPN_ACTIVE_ID), Bitmap(BITMAP_FLAG_JPN_ACTIVE_ID));
    languageSelector.addMenuElement(Bitmap(BITMAP_FLAG_UKR_ACTIVE_ID), Bitmap(BITMAP_FLAG_UKR_ACTIVE_ID));
    languageSelector.addMenuElement(Bitmap(BITMAP_FLAG_NLD_ACTIVE_ID), Bitmap(BITMAP_FLAG_NLD_ACTIVE_ID));
    languageSelector.setElementSpace(20);
    languageSelector.setExpandDirection(CollapsibleMenu::LEFT);
    languageSelector.setStateChangedCallback(languageSelectorStateChanged);
    languageSelector.setTimeout(300);
    languageSelector.finilizeInitialization();
    languageSelector.setXY(469 - languageSelector.getWidth(), 227);
    languageSelector.fadeOut(10);
    add(languageSelector);

    setLanguage();
    animateToPoster(0, 1);
}

void ProductPresenterView::tearDownScreen()
{

}

void ProductPresenterView::updateProcessorLoad(uint8_t mcuLoadValue)
{
    mcuLoad.updateProcessorLoad(mcuLoadValue);
}

void ProductPresenterView::nextbuttonPressed()
{
    animateToPoster((selectedPoster + 1) % NUMBER_OF_POSTERS, 10);
}

void ProductPresenterView::activateLanguageSelectorButtonClicked()
{
    if (languageSelector.isFadedOut())
    {
        languageSelector.fadeInAndExpand();
    }
}

void ProductPresenterView::languageSelectorStateChangedHandler(const CollapsibleMenu& menu, const bool isExpanded)
{
    if (!isExpanded)
    {
        languageSelector.fadeOut();
        setLanguage();
    }
}

void ProductPresenterView::animateToPoster(int posterIndex, int duration)
{
    if (duration < 2)
    {
        headlineText.setTypedText(TypedText(getHeadline(posterIndex)));
        headlineText.invalidate();

        bodyText.setTypedText(TypedText(getBodyText(posterIndex)));
        bodyText.invalidate();

        poster.setBitmap(Bitmap(getPosterBitmap(posterIndex)));
    }
    else
    {
        nextButton.setTouchable(false);

        // Replace headlineText with swap text
        headlineTextSwap.setTypedText(headlineText.getTypedText());
        headlineTextSwap.setXY(headlineText.getX(), headlineText.getY());
        headlineTextSwap.invalidate();

        // Make headlineText ready to be animated in again
        headlineText.setTypedText(TypedText(getHeadline(posterIndex)));
        headlineText.moveTo(headlineTextSwap.getRect().right(), headlineTextSwap.getY());
        headlineText.invalidate();

        headlineTextSwap.startMoveAnimation(headlineTextSwap.getX() - headlineTextSwap.getWidth(), headlineTextSwap.getY(), duration, EasingEquations::cubicEaseInOut);
        headlineText.startMoveAnimation(headlineTextSwap.getX(), headlineTextSwap.getY(), duration, EasingEquations::cubicEaseInOut);

        // Start text fade
        bodyText.startFadeAnimation(0, duration);
        fadeDuration = duration;
        nextTextIndex = posterIndex;

        // Replace poster with swap poster
        posterSwap.setBitmap(Bitmap(poster.getBitmap()));
        posterSwap.setXY(poster.getX(), poster.getY());
        posterSwap.invalidate();

        // Make poster ready to be animated in again
        poster.setBitmap(Bitmap(getPosterBitmap(posterIndex)));
        poster.moveTo(posterSwap.getRect().right(), posterSwap.getY());
        poster.invalidate();

        posterSwap.startMoveAnimation(posterSwap.getX() - posterSwap.getWidth(), posterSwap.getY(), duration, EasingEquations::cubicEaseInOut);
        poster.startMoveAnimation(posterSwap.getX(), posterSwap.getY(), duration, EasingEquations::cubicEaseInOut);
    }

    selectedPoster = posterIndex;
}


void ProductPresenterView::setLanguage()
{
    int languageIndex = languageSelector.getSelectedElementIndex();
    activateLanguageSelectorButton.setBitmaps(Bitmap(inactiveLanguageButtons[languageIndex]), Bitmap(inactiveLanguageButtons[languageIndex]));
    activateLanguageSelectorButton.setWidth(activateLanguageSelectorButton.getWidth() + 20); // Increase the button active area to better react to click
    activateLanguageSelectorButton.setHeight(activateLanguageSelectorButton.getHeight() + 20); // Increase the button active area to better react to click
    activateLanguageSelectorButton.invalidate();

    Texts::setLanguage(languages[languageIndex]);
    bodyText.resizeToCurrentText();
    textAreaContainer.reset();
    textAreaContainer.invalidate();
    mcuLoad.invalidate();
    headlineText.invalidate();
    bodyText.invalidate();
}


touchgfx::BitmapId ProductPresenterView::getPosterBitmap(int index)
{
    switch (index)
    {
    default:
    case 0:
        return BITMAP_POSTER_IMAGE_00_ID;
    case 1:
        return BITMAP_POSTER_IMAGE_01_ID;
    case 2:
        return BITMAP_POSTER_IMAGE_02_ID;
    }
}

TEXTS ProductPresenterView::getHeadline(int index)
{
    switch (index)
    {
    default:
    case 0:
        return T_POSTER_HEADLINE_00;
    case 1:
        return T_POSTER_HEADLINE_01;
    case 2:
        return T_POSTER_HEADLINE_02;
    }
}

TEXTS ProductPresenterView::getBodyText(int index)
{
    switch (index)
    {
    default:
    case 0:
        return T_POSTER_TEXT_00;
    case 1:
        return T_POSTER_TEXT_01;
    case 2:
        return T_POSTER_TEXT_02;
    }
}

void ProductPresenterView::fadeAnimationEndedHandler(const FadeAnimator<TextArea>& textArea)
{
    if (&textArea == &bodyText)
    {
        if (bodyText.getAlpha() == 0)
        {
            bodyText.setTypedText(TypedText(getBodyText(nextTextIndex)));
            bodyText.resizeToCurrentText();
            bodyText.startFadeAnimation(255, fadeDuration);

            textAreaContainer.reset();
            textAreaContainer.invalidate();

            nextButton.setTouchable(true);
        }
    }
}
