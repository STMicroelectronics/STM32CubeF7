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
  #ifndef PRODUCTPRESENTER_VIEW_HPP
#define PRODUCTPRESENTER_VIEW_HPP

#include <gui_generated/productpresenter_screen/ProductPresenterViewBase.hpp>
#include <gui/productpresenter_screen/ProductPresenterPresenter.hpp>
#include <gui/productpresenter_screen/CollapsibleMenu.hpp>
#include <texts/TextKeysAndLanguages.hpp>

class ProductPresenterView : public ProductPresenterViewBase
{
public:
    ProductPresenterView();
    virtual ~ProductPresenterView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void updateProcessorLoad(uint8_t mcuLoadValue);

    void nextbuttonPressed();
    void activateLanguageSelectorButtonClicked();

protected:
    static const uint8_t NUMBER_OF_POSTERS = 3;

    LANGUAGES languages[NUMBER_OF_LANGUAGES];
    BitmapId inactiveLanguageButtons[NUMBER_OF_LANGUAGES];

    int selectedPoster;
    int nextTextIndex;
    int fadeDuration;

    CollapsibleMenu languageSelector;

    Callback<ProductPresenterView, const CollapsibleMenu&, const bool> languageSelectorStateChanged;
    void languageSelectorStateChangedHandler(const CollapsibleMenu& menu, const bool isExpanded);

    Callback<ProductPresenterView, const FadeAnimator<TextArea>&> onFadeAnimationEnded;
    void fadeAnimationEndedHandler(const FadeAnimator<TextArea>& textArea);

    void animateToPoster(int posterIndex, int duration);

    void setLanguage();
    BitmapId getPosterBitmap(int index);
    TEXTS getHeadline(int index);
    TEXTS getBodyText(int index);
};

#endif // PRODUCTPRESENTER_VIEW_HPP
