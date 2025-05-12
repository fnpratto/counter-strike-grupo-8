#include "hud_display.h"

#include <algorithm>
#include <filesystem>
#include <vector>
const int size_width = 62;
const int size_height = 64;
const int padding = 10;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
float scale = 0.5f;
int digitSpacingSmall = static_cast<int>(42 * scale) + 1;
int digitSpacingMedium = static_cast<int>(42) + 1;

const Area sizeMoney = Area(460, 0, size_height, size_width);
const Area sizeChronometer = Area(130, 0, size_height, size_width);
const Area sizeLife = Area(0, 0, size_height, size_width);
const Area sizeBullets = Area(0, 0, 300, 302);
const Area sizePointer = Area(0, 0, 50, 50);
const Area destPointer = Area(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 50, 50);
const Area sizeBackground = Area(0, 0, 60, 60);
const Area destBackground = Area(0, 0, 800, 600);


const Area destMoney = Area(SCREEN_WIDTH - size_width - padding * 2 - SCREEN_WIDTH / 20,
                            SCREEN_HEIGHT - size_height + 10, SCREEN_WIDTH / 20, 32);
const Area destLife = Area(10, SCREEN_HEIGHT - size_height + 10, SCREEN_WIDTH / 20, 32);
const Area destBullets = Area(SCREEN_WIDTH - padding * 5, SCREEN_HEIGHT - size_height * 2 + 10,
                              SCREEN_WIDTH / 20, 32);


const std::string& BACKGROUND_PATH = "assets/gfx/backgrounds/water1.jpg";
const std::string& POINTER_PATH = "assets/gfx/hud/pointer.xcf";
const std::string& MONEY_PATH = "assets/gfx/hud/hud_symbols.xcf";
const std::string& LIFE_PATH = "assets/gfx/hud/hud_symbols.xcf";
const std::string& CHRONOMETER_PATH = "assets/gfx/hud/hud_symbols.xcf";
const std::string& FONT_PATH = "assets/gfx/fonts/joystix_monospace.otf";
const std::string& TRAPECIO_PATH = "assets/gfx/hud/trapezoid.xcf";
const std::string& BULLET_PATH = "assets/gfx/hud/bullet-icon1.xcf";  // TODO color


hudDisplay::hudDisplay(SdlWindow& window):
        window(window),
        back(BACKGROUND_PATH, window),
        pointer(POINTER_PATH, window),
        money(MONEY_PATH, window),
        money_amount(window.getRenderer(), "assets/gfx/fonts/hud_nums.xcf"),
        life(LIFE_PATH, window),
        life_amount(window.getRenderer(), "assets/gfx/fonts/hud_nums.xcf"),
        timer_dots("assets/gfx/fonts/hud_nums.xcf", window),
        timer_amount(window.getRenderer(), "assets/gfx/fonts/hud_nums.xcf"),
        roundText(FONT_PATH, 20, {150, 150, 150, 255}, window),
        equipedBullets(BULLET_PATH, window),
        equipedBulletsAmount(window.getRenderer(), "assets/gfx/fonts/hud_nums.xcf"),
        gunNumber(FONT_PATH, 20, {150, 150, 150, 255}, window) {}

void hudDisplay::render() {
    window.fill();
    back.render(sizeBackground, destBackground);

    SDL_Renderer* renderer = window.getRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_Rect rect = {0, 0, SCREEN_WIDTH, 10};
    SDL_RenderFillRect(renderer, &rect);

    SdlTexture trapecio(TRAPECIO_PATH, window);
    Area srcTrapecio(0, 0, 500, 230);
    Area destTrapecio(SCREEN_WIDTH / 2 - 200, 0, SCREEN_WIDTH / 2, size_height * 2);
    trapecio.render(srcTrapecio, destTrapecio);


    pointer.render(sizePointer, destPointer);
    updateComponents();
    money.render(sizeMoney, destMoney);
    std::string moneyStr = "1000";  // Just the numeric part
    int x = SCREEN_WIDTH - size_width - padding * 4;
    int y = SCREEN_HEIGHT - size_height + 10;
    for (char c: moneyStr) {
        if (isdigit(c)) {
            money_amount.renderDigit(c - '0', x, y, scale);
            x += digitSpacingSmall;
        }
    }

    life.render(sizeLife, destLife);
    std::string lifeStr = "100";  // Just the numeric part
    x = 55;
    y = SCREEN_HEIGHT - size_height + 10;
    for (char c: lifeStr) {
        if (isdigit(c)) {
            life_amount.renderDigit(c - '0', x, y, scale);
            x += digitSpacingSmall;
        }
    }
    show_timer();
    roundText.setTextString("Round 10");
    roundText.render(Area(SCREEN_WIDTH / 2 - 50, padding, 100, 20));

    equipedBullets.render(sizeBullets, destBullets);

    std::string bulletsStr = "30";  // Just the numeric part
    x = SCREEN_WIDTH - size_width - padding * 6;
    y = SCREEN_HEIGHT - size_height * 2 + 10;
    for (char c: bulletsStr) {
        if (isdigit(c)) {
            equipedBulletsAmount.renderDigit(c - '0', x, y, scale);
            x += digitSpacingSmall;
        }
    }

    renderGunIcons();

    window.render();  // swap buffers
}

void hudDisplay::show_timer() {

    int currentClockTick = 123;
    int minutesIdx = std::floor(currentClockTick / 60);
    int seconds = currentClockTick % 60;
    int secondsIdxH = std::floor(seconds / 10);
    int secondsIdxL = seconds % 10;

    // Render minutes
    int x = SCREEN_WIDTH / 2 - 40;
    int y = padding + 30;
    timer_amount.renderDigit(minutesIdx, x, y, scale);
    x += digitSpacingSmall;

    // Render colon
    Area srcColon(475, 0, 10, size_height);
    Area dstColon(x, y, 10, size_height / 2);
    timer_dots.render(srcColon, dstColon);
    x += digitSpacingSmall;

    // Render tens place of seconds
    timer_amount.renderDigit(secondsIdxH, x, y, scale);
    x += digitSpacingSmall;

    // Render units place of seconds
    timer_amount.renderDigit(secondsIdxL, x, y, scale);
}


void hudDisplay::renderGunIcons() {  // TODO change to user state
    int x = SCREEN_WIDTH - size_width - padding * 6;
    int y = SCREEN_HEIGHT / 2;
    int spacing = 64;

    SdlTexture ak47("assets/gfx/guns/ak47_k.xcf", window);
    SdlTexture aug("assets/gfx/guns/aug_k.xcf", window);
    SdlTexture elite("assets/gfx/guns/elite_k.xcf", window);

    Area destArea(x, y, SCREEN_WIDTH / 10, 34);
    ak47.render(Area(0, 0, 30, 10), destArea);
    gunNumber.setTextString("1");
    gunNumber.render(Area(SCREEN_WIDTH - padding * 2, y, 10, 20));
    y += spacing;

    destArea = Area(x, y, SCREEN_WIDTH / 10, 34);
    aug.render(Area(0, 0, 28, 10), destArea);
    gunNumber.setTextString("2");
    gunNumber.render(Area(SCREEN_WIDTH - padding * 2, y, 10, 20));
    y += spacing;

    destArea = Area(x, y, SCREEN_WIDTH / 10, 34);
    elite.render(Area(0, 0, 15, 10), destArea);
    gunNumber.setTextString("3");
    gunNumber.render(Area(SCREEN_WIDTH - padding * 2, y, 10, 20));
}

void hudDisplay::updateComponents() {
    /*money_amount.setTextString("$100");
    life_amount.setTextString("Life: 100");
    timer_amount.setTextString("Chrono: 00:00");*/
}
