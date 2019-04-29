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
