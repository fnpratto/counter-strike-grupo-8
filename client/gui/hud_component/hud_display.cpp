#include "hud_display.h"

#include <algorithm>
#include <filesystem>
#include <vector>

// Constants
const int size_width = 62;
const int size_height = 64;
const int padding = 10;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float scale = 0.5f;
const int digitSpacingSmall = static_cast<int>(42 * scale) + 1;

// Paths
const std::string& BACKGROUND_PATH = "../assets/gfx/backgrounds/water1.jpg";
const std::string& POINTER_PATH = "../assets/gfx/hud/pointer.xcf";
const std::string& MONEY_PATH = "../assets/gfx/hud/hud_symbols.xcf";
const std::string& LIFE_PATH = "../assets/gfx/hud/hud_symbols.xcf";
const std::string& FONT_PATH = "../assets/gfx/fonts/joystix_monospace.otf";
const std::string& BULLET_PATH = "../assets/gfx/hud/bullet-icon1.xcf";
const std::string& TRAPECIO_PATH = "../assets/gfx/hud/trapezoid.xcf";
const std::string& HUD_NUMS_PATH = "../assets/gfx/fonts/hud_nums.xcf";
const std::string& PARALELO_RED_PATH = "../assets/gfx/hud/parallelogram.xcf";
const std::string& PARALELO_BLUE_PATH = "../assets/gfx/hud/parallelogram_blue.xcf";
const std::string& PARALELO_RED_O_PATH = "../assets/gfx/hud/parallelogram_red_op.xcf";
const std::string& PARALELO_BLUE_O_PATH = "../assets/gfx/hud/parallelogram_blue_op.xcf";


// Constructor
hudDisplay::hudDisplay(SdlWindow& window):
        window(window),
        back(BACKGROUND_PATH, window),
        pointer(POINTER_PATH, window),
        money(MONEY_PATH, window),
        money_amount(window.getRenderer(), "../assets/gfx/fonts/hud_nums.xcf"),
        life(LIFE_PATH, window),
        life_amount(window.getRenderer(), "../assets/gfx/fonts/hud_nums.xcf"),
        timer_dots("../assets/gfx/fonts/hud_nums.xcf", window),
        timer_amount(window.getRenderer(), "../assets/gfx/fonts/hud_nums.xcf"),
        roundText(FONT_PATH, 20, {150, 150, 150, 255}, window),
        equipedBullets(BULLET_PATH, window),
        equipedBulletsAmount(window.getRenderer(), "../assets/gfx/fonts/hud_nums.xcf"),
        gunNumber(FONT_PATH, 20, {150, 150, 150, 255}, window),
        scoreText(FONT_PATH, 20, {255, 255, 255, 255}, window) {}

// Main render function
void hudDisplay::render() {
    window.fill();
    renderBackground();
    renderParal();
    renderPointer();
    renderMoney();
    renderLife();
    renderTimer();
    renderRoundText();
    renderBullets();
    renderGunIcons();
    window.render();
}

// Render background
void hudDisplay::renderBackground() {
    const Area sizeBackground(0, 0, 60, 60);
    const Area destBackground(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    back.render(sizeBackground, destBackground);
    SDL_Renderer* renderer = window.getRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_Rect rect = {0, 0, SCREEN_WIDTH, 10};
    SDL_RenderFillRect(renderer, &rect);
    SdlTexture trapecio(TRAPECIO_PATH, window);
    Area srcTrapecio(0, 0, 500, 230);
    Area destTrapecio(SCREEN_WIDTH / 2 - 200, 0, SCREEN_WIDTH / 2, size_height * 2);
    trapecio.render(srcTrapecio, destTrapecio);
}

// Render trapecio
void hudDisplay::renderParal() {

    // int randomNum = getTeam()  // TODO logica depende el usuario si es terrorista o counter
    // if (randomNum == 1) {
    SdlTexture parallelogram1(PARALELO_BLUE_O_PATH, window);
    Area srcParallelogram1(0, 180, 500, 320);
    Area destParallelogram1(SCREEN_WIDTH / 2 - 290, padding * 2, size_width * 3, size_height * 2);
    parallelogram1.render(srcParallelogram1, destParallelogram1);


    scoreText.setTextString("9");
    scoreText.render(Area(SCREEN_WIDTH / 2 - 200, padding * 3, size_width / 1.75, size_height / 2));

    SdlTexture parallelogram(PARALELO_RED_PATH, window);
    Area srcParallelogram(0, 180, 500, 320);
    Area destParallelogram(SCREEN_WIDTH / 2 + 110, padding * 2, size_width * 3, size_height * 2);
    parallelogram.render(srcParallelogram, destParallelogram);

    scoreText.setTextString("1");
    scoreText.render(Area(SCREEN_WIDTH / 2 + 190, padding * 3, size_width / 1.75, size_height / 2));
    /*}
    else {
        SdlTexture parallelogram(PARALELO_BLUE_PATH, window);
        Area srcParallelogram(0, 0, 500, 230);
        Area destParallelogram(0, 0, 100, size_height * 2);
        parallelogram.render(srcParallelogram, destParallelogram);

        SdlTexture parallelogram1(PARALELO_RED_O_PATH, window);
        Area srcParallelogram1(0, 0, 500, 230);
        Area destParallelogram1(SCREEN_WIDTH / 2 - 200, 0, 100, size_height * 2);
        parallelogram1.render(srcParallelogram1, destParallelogram1);
        SDL_Renderer* renderer = window.getRenderer();
    }*/
}


void hudDisplay::renderPointer() {
    const Area sizePointer(0, 0, 50, 50);
    const Area destPointer(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 50, 50);
    pointer.render(sizePointer, destPointer);
}


void hudDisplay::renderMoney() {
    const Area sizeMoney(460, 0, size_height, size_width);
    const Area destMoney(SCREEN_WIDTH - size_width - padding * 2 - SCREEN_WIDTH / 20,
                         SCREEN_HEIGHT - size_height + 10, SCREEN_WIDTH / 20, 32);
    money.render(sizeMoney, destMoney);

    std::string moneyStr = "1000";
    int x = SCREEN_WIDTH - size_width - padding * 4;
    int y = SCREEN_HEIGHT - size_height + 10;
    renderDigits(moneyStr, x, y, money_amount);
}


void hudDisplay::renderLife() {
    const Area sizeLife(0, 0, size_height, size_width);
    const Area destLife(10, SCREEN_HEIGHT - size_height + 10, SCREEN_WIDTH / 20, 32);
    life.render(sizeLife, destLife);

    std::string lifeStr = "100";
    int x = 55;
    int y = SCREEN_HEIGHT - size_height + 10;
    renderDigits(lifeStr, x, y, life_amount);
}


void hudDisplay::renderTimer() {
    int currentClockTick = 123;
    int minutesIdx = std::floor(currentClockTick / 60);
    int seconds = currentClockTick % 60;
    int secondsIdxH = std::floor(seconds / 10);
    int secondsIdxL = seconds % 10;

    int x = SCREEN_WIDTH / 2 - 40;
    int y = padding + 30;

    timer_amount.renderDigit(minutesIdx, x, y, scale);
    x += digitSpacingSmall;

    Area srcColon(475, 0, 10, size_height);
    Area dstColon(x, y, 10, size_height / 2);
    timer_dots.render(srcColon, dstColon);
    x += digitSpacingSmall;

    timer_amount.renderDigit(secondsIdxH, x, y, scale);
    x += digitSpacingSmall;
    timer_amount.renderDigit(secondsIdxL, x, y, scale);
}


void hudDisplay::renderRoundText() {
    roundText.setTextString("Round 10");
    roundText.render(Area(SCREEN_WIDTH / 2 - 50, padding, 100, 20));
}


void hudDisplay::renderBullets() {
    const Area sizeBullets(0, 0, 300, 302);
    const Area destBullets(SCREEN_WIDTH - padding * 5, SCREEN_HEIGHT - size_height * 2 + 10,
                           SCREEN_WIDTH / 20, 32);
    equipedBullets.render(sizeBullets, destBullets);

    std::string bulletsStr = "30";
    int x = SCREEN_WIDTH - size_width - padding * 6;
    int y = SCREEN_HEIGHT - size_height * 2 + 10;
    renderDigits(bulletsStr, x, y, equipedBulletsAmount);
}


void hudDisplay::renderGunIcons() {
    int x = SCREEN_WIDTH - size_width - padding * 6;
    int y = SCREEN_HEIGHT / 2;
    int spacing = 64;

    renderGunIcon("../assets/gfx/guns/ak47_k.xcf", "1", x, y);
    y += spacing;
    renderGunIcon("../assets/gfx/guns/aug_k.xcf", "2", x, y);
    y += spacing;
    renderGunIcon("../assets/gfx/guns/elite_k.xcf", "3", x, y);
}


void hudDisplay::renderGunIcon(const std::string& path, const std::string& number, int x, int y) {
    SdlTexture gunTexture(path, window);
    Area destArea(x, y, SCREEN_WIDTH / 10, 34);
    gunTexture.render(Area(0, 0, 30, 10), destArea);
    gunNumber.setTextString(number);
    gunNumber.render(Area(SCREEN_WIDTH - padding * 2, y, 10, 20));
}


void hudDisplay::renderDigits(const std::string& str, int x, int y, BitmapFont& texture) {
    for (char c: str) {
        if (isdigit(c)) {
            texture.renderDigit(c - '0', x, y, scale);
            x += digitSpacingSmall;
        }
    }
}
