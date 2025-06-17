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
class hudDisplay {

    const GameUpdate& state;
    const std::string& player_name;
    SdlWindow& window;

    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    SdlTexture pointer;
    SdlTexture money;
    BitmapFont money_amount;
    SdlTexture life;
    BitmapFont life_amount;
    SdlTexture equipedBullets;
    SdlTexture white_line;
    BitmapFont equipedBulletsAmount;
    BitmapFont timer_amount;
    SdlTexture timer_dots;
    SdlText roundText;
    SdlText gunNumber;
    SdlText scoreText;

    SdlTexture muteIcon;
    SdlTexture gunsInventoryTexture;
    int pointerX = SCREEN_WIDTH / 2;
    int pointerY = SCREEN_HEIGHT / 2;
    float widthRatio;
    float heightRatio;
    float scaleRatio;
    HudLayout layout;
    SdlTexture start_game_buttom;


public:
    explicit hudDisplay(SdlWindow& window, const GameUpdate& state, const std::string& player_name);
    void render();
    void updatePointerPosition(int x, int y);
    bool start_game_click(int x, int y);

private:
    void renderBackground();
    void renderTeamScores();
    void renderPointer();
    void renderMoney();
    void renderLife();
    void renderTimer();
    void renderRoundText();
    void renderBullets();
    void renderGunIcons();
    void renderMuteIcon(bool isMuted);
    void renderStartGameButton();
    void renderDigits(const std::string& str, int x, int y, BitmapFont& texture);
    void renderChat();
};

#endif  // HUD_DISPLAY_H
