#ifndef HUD_DISPLAY_H
#define HUD_DISPLAY_H

#include <string>
#include <vector>

#include "../window_elements/Area.h"  // Assuming Area is in this header
#include "../window_elements/SdlText.h"
#include "../window_elements/SdlTexture.h"
#include "../window_elements/SdlWindow.h"

#include "BitmapFontNumber.h"

class hudDisplay {
public:
    explicit hudDisplay(SdlWindow& window);
    void render();

private:
    SdlWindow& window;
    SdlTexture back;
    SdlTexture pointer;
    SdlTexture money;
    BitmapFont money_amount;
    SdlTexture life;
    BitmapFont life_amount;
    /*SdlTexture equipedGuns;
    BitmapFont equipedGuns_amount;*/
    SdlTexture equipedBullets;
    BitmapFont equipedBulletsAmount;
    BitmapFont timer_amount;
    SdlTexture timer_dots;
    SdlText roundText;
    SdlText gunNumber;
    void renderGunIcons();

    void show_timer();

    void updateComponents();
};

#endif  // HUD_DISPLAY_H
