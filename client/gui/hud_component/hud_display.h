#ifndef HUD_DISPLAY_H
#define HUD_DISPLAY_H

#include <string>
#include <vector>

#include "../window_elements/area.h"  // Assuming Area is in this header
#include "../window_elements/sdl_text.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/updates/game_update.h"

#include "bit_map_font_number.h"


struct HudLayout {
    int iconWidth;
    int iconHeight;
    int size_width;
    int size_height;
    int digitHeight;
    int digitSpacing;
    int padding;
    float scale;
};

struct HudData {
    int money;
    int life;
    int bullets;
    TimePoint timer;  // in seconds or ticks (depending on your timer system)
    int roundNumber;
    // std::vector<std::string> equippedGuns;  // paths to gun icons (e.g., "ak47_k.xcf", etc.)
    // std::optional<WeaponSlot> selectedGunIndex;
    int scoreTT;
    int scoreCT;  // index of the currently selected gun
};
class hudDisplay {
public:
    explicit hudDisplay(SdlWindow& window);
    void render();
    void updatePointerPosition(int x, int y);
    void update(GameUpdate state);
    HudData hudData;

private:
    SdlWindow& window;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
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

    int pointerX = SCREEN_WIDTH / 2;
    int pointerY = SCREEN_HEIGHT / 2;
    float widthRatio;
    float heightRatio;
    float scaleRatio;
    HudLayout layout;


private:
    void renderBackground();
    void renderParal();
    void renderPointer();
    void renderMoney();
    void renderLife();
    void renderTimer();
    void renderRoundText();
    void renderBullets();
    void renderGunIcons();

    void renderGunIcon(const std::string& path, const std::string& number, int x, int y);
    void renderDigits(const std::string& str, int x, int y, BitmapFont& texture);
};

#endif  // HUD_DISPLAY_H
