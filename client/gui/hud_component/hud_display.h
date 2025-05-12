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
    SdlTexture equipedBullets;
    BitmapFont equipedBulletsAmount;
    BitmapFont timer_amount;
    SdlTexture timer_dots;
    SdlText roundText;
    SdlText gunNumber;
    SdlText scoreText;
    void renderBackground();
    void renderParal();
    void renderPointer();
    void renderMoney();
    void renderLife();
    void renderTimer();
    void renderRoundText();
    void renderBullets();
    void renderGunIcons();

    void show_timer();

    void updateComponents();

    void renderGunIcon(const std::string& path, const std::string& number, int x, int y);
    void renderDigits(const std::string& str, int x, int y, BitmapFont& texture);
};

#endif  // HUD_DISPLAY_H
