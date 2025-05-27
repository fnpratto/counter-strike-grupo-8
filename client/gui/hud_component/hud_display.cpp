#include "hud_display.h"

#include <algorithm>
#include <filesystem>
#include <vector>
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
const std::string& MUTE_ICON_PATH = "../assets/gfx/hud/hud_voice.xcf";

hudDisplay::hudDisplay(SdlWindow& window):
        window(window),
        SCREEN_WIDTH(window.getWidth()),
        SCREEN_HEIGHT(window.getHeight()),
        back(BACKGROUND_PATH, window),
        pointer(POINTER_PATH, window),
        money(MONEY_PATH, window),
        money_amount(window.getRenderer(), "../assets/gfx/fonts/hud_nums.xcf"),
        life(LIFE_PATH, window),
        life_amount(window.getRenderer(), "../assets/gfx/fonts/hud_nums.xcf"),
        equipedBullets(BULLET_PATH, window),
        equipedBulletsAmount(window.getRenderer(), "../assets/gfx/fonts/hud_nums.xcf"),
        timer_amount(window.getRenderer(), "../assets/gfx/fonts/hud_nums.xcf"),
        timer_dots("../assets/gfx/fonts/hud_nums.xcf", window),
        roundText(FONT_PATH, 20, {150, 150, 150, 255}, window),
        gunNumber(FONT_PATH, 20, {150, 150, 150, 255}, window),
        scoreText(FONT_PATH, 20, {255, 255, 255, 255}, window),
        muteIcon(MUTE_ICON_PATH, window) {

    float BASE_WIDTH = 800.0f;
    float BASE_HEIGHT = 600.0f;

    widthRatio = SCREEN_WIDTH / BASE_WIDTH;
    heightRatio = SCREEN_HEIGHT / BASE_HEIGHT;
    scaleRatio = std::min(widthRatio, heightRatio);


    layout.padding = static_cast<int>(10 * scaleRatio);
    layout.iconWidth = static_cast<int>(32 * scaleRatio);
    layout.size_width = static_cast<int>(62 * scaleRatio);
    layout.size_height = static_cast<int>(64 * scaleRatio);
    layout.iconHeight = static_cast<int>(32 * scaleRatio);
    layout.digitSpacing = static_cast<int>(22 * scaleRatio);
    layout.digitHeight = static_cast<int>(32 * scaleRatio);
    layout.scale = 0.5f * scaleRatio;
}


void hudDisplay::update(/*const PlayerDTO& player_info,*/ int currentClockTick, bool isMuted) {

    renderBackground();
    renderParal();
    renderPointer();
    renderMoney();
    renderLife();
    renderTimer(currentClockTick);
    renderRoundText();
    renderBullets();
    renderGunIcons();
    renderMuteIcon(isMuted);
}


void hudDisplay::render() {
    renderBackground();
    renderParal();
    renderPointer();
    renderMoney();
    renderLife();
    renderTimer(1023);
    renderRoundText();
    renderBullets();
    renderGunIcons();
    renderMuteIcon(false);
}

void hudDisplay::renderMuteIcon(bool isMuted) {
    int iconWidth = static_cast<int>(32 * scaleRatio);
    int iconHeight = static_cast<int>(32 * scaleRatio);

    if (isMuted) {

        const Area sizeMuteIcon(0, 0, 64, 64);
        const Area destMuteIcon(SCREEN_WIDTH - iconWidth - layout.padding * 4, iconHeight,
                                iconWidth, iconHeight);
        muteIcon.render(sizeMuteIcon, destMuteIcon);

    } else {
        const Area sizeMuteIcon(256, 0, 64, 64);
        const Area destMuteIcon(SCREEN_WIDTH - iconWidth - layout.padding * 4, iconHeight,
                                iconWidth, iconHeight);
        muteIcon.render(sizeMuteIcon, destMuteIcon);
    }
}


// Render background
void hudDisplay::renderBackground() {
    // back.render(sizeBackground, destBackground);
    SDL_Renderer* renderer = window.getRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_Rect rect = {0, 0, SCREEN_WIDTH, 10};
    SDL_RenderFillRect(renderer, &rect);
    SdlTexture trapecio(TRAPECIO_PATH, window);
    Area srcTrapecio(0, 0, 500, 230);
    Area destTrapecio(SCREEN_WIDTH * 0.5 - SCREEN_WIDTH * 0.5 / 2, 0, SCREEN_WIDTH * 0.5,
                      layout.size_height * 1.75);
    trapecio.render(srcTrapecio, destTrapecio);
}

// Render trapecio
void hudDisplay::renderParal() {
    const int trapecioWidth = SCREEN_WIDTH * 0.5;
    const int trapecioX = (SCREEN_WIDTH - trapecioWidth) / 4;

    const int paralWidth = layout.size_width * 4;
    const int paralHeight = layout.size_height * 1.75;
    const int margin = layout.padding * 2;  // espacio entre trapecio y paralelogramas

    // BLUE (izquierda del trapecio)
    SdlTexture parallelogram1(PARALELO_BLUE_O_PATH, window);
    Area srcParallelogram1(0, 180, 500, 320);
    Area destParallelogram1(trapecioX - paralWidth / 4 + margin, layout.padding * 2, paralWidth,
                            paralHeight);
    parallelogram1.render(srcParallelogram1, destParallelogram1);

    scoreText.setTextString("9");
    scoreText.render(Area(trapecioX - paralWidth / 4 + margin * 6, layout.padding * 3,
                          layout.size_width / 1.75, layout.size_height / 2));

    // RED (derecha del trapecio)
    SdlTexture parallelogram(PARALELO_RED_PATH, window);
    Area srcParallelogram(0, 180, 500, 320);
    Area destParallelogram(trapecioX + trapecioWidth + margin, layout.padding * 2, paralWidth,
                           paralHeight);
    parallelogram.render(srcParallelogram, destParallelogram);

    scoreText.setTextString("1");
    scoreText.render(
            Area(trapecioX + trapecioWidth + margin + paralWidth / 2 - layout.size_width / 4,
                 layout.padding * 3, layout.size_width / 1.75, layout.size_height / 2));
}


void hudDisplay::renderPointer() {
    const Area sizePointer(0, 0, 50, 50);
    const Area destPointer(pointerX - 25, pointerY - 25, 50, 50);
    pointer.render(sizePointer, destPointer);
}


void hudDisplay::renderMoney() {
    int iconHeight = static_cast<int>(32 * scaleRatio);
    const Area sizeMoney(460, 0, 64, 62);
    const Area destMoney(SCREEN_WIDTH - layout.size_width * 2 - layout.padding,
                         SCREEN_HEIGHT - iconHeight - layout.padding, SCREEN_WIDTH / 20,
                         iconHeight);
    money.render(sizeMoney, destMoney);

    std::string moneyStr = "1000";
    int x = SCREEN_WIDTH - layout.size_width - layout.padding * 4;
    int y = SCREEN_HEIGHT - iconHeight - layout.padding;
    renderDigits(moneyStr, x, y, money_amount);
}


void hudDisplay::renderLife() {
    int iconWidth = static_cast<int>(32 * scaleRatio);
    int iconHeight = static_cast<int>(32 * scaleRatio);

    const Area sizeLife(0, 0, 62, 64);
    const Area destLife(layout.padding, SCREEN_HEIGHT - iconHeight - layout.padding, iconWidth,
                        iconHeight);
    life.render(sizeLife, destLife);

    std::string lifeStr = "100";
    int x = layout.padding + iconWidth + layout.digitSpacing / 2;
    int y = SCREEN_HEIGHT - iconHeight - layout.padding;
    renderDigits(lifeStr, x, y, life_amount);
}


void hudDisplay::renderTimer(int currentClockTick) {
    int minutesIdx = std::floor(currentClockTick / 60);
    int seconds = currentClockTick % 60;
    int secondsIdxH = std::floor(seconds / 10);
    int secondsIdxL = seconds % 10;

    int totalTimerWidth = layout.digitSpacing * 4 + 10;
    int x = SCREEN_WIDTH / 2 - totalTimerWidth / 2;
    int y = layout.padding + 30;

    timer_amount.renderDigit(minutesIdx, x, y, layout.scale);
    x += layout.digitSpacing;

    Area srcColon(475, 0, 10, layout.size_height);
    Area dstColon(x, y, 10, layout.size_height / 2);
    timer_dots.render(srcColon, dstColon);
    x += layout.digitSpacing;

    timer_amount.renderDigit(secondsIdxH, x, y, layout.scale);
    x += layout.digitSpacing;
    timer_amount.renderDigit(secondsIdxL, x, y, layout.scale);
}


void hudDisplay::renderRoundText() {
    roundText.setTextString("Round 10");
    roundText.render(Area(SCREEN_WIDTH / 2 - 50, layout.padding, 100, 20));
}


void hudDisplay::renderBullets() {
    int iconHeight = static_cast<int>(32 * scaleRatio);
    const Area sizeBullets(0, 0, 300, 302);
    const Area destBullets(SCREEN_WIDTH - SCREEN_WIDTH / 40 - layout.padding * 2,
                           SCREEN_HEIGHT - iconHeight * 3, 64 * layout.scale, 64 * layout.scale);
    equipedBullets.render(sizeBullets, destBullets);

    std::string bulletsStr = "30";
    int x = SCREEN_WIDTH - layout.size_width - SCREEN_WIDTH / 40 - layout.padding * 2;
    int y = SCREEN_HEIGHT - iconHeight * 3;
    renderDigits(bulletsStr, x, y, equipedBulletsAmount);
}


void hudDisplay::renderGunIcons() {
    int x = SCREEN_WIDTH - layout.size_width - SCREEN_WIDTH / 20;
    int y = SCREEN_HEIGHT / 2;

    int spacing = 64;

    renderGunIcon("../assets/gfx/guns/knife_k.xcf", "1", x, y);
    y += spacing;
    renderGunIcon("../assets/gfx/guns/aug_k.xcf", "2", x, y);
    y += spacing;
    renderGunIcon("../assets/gfx/guns/elite_k.xcf", "3", x, y);
}


void hudDisplay::renderGunIcon(const std::string& path, const std::string& number, int x, int y) {
    SdlTexture gunTexture(path, window);
    Area destArea(x, y, SCREEN_WIDTH / 14, 34);
    gunTexture.render(
            Area(0, 0, layout.size_width * 2 * layout.scale, layout.size_height * layout.scale),
            destArea);
    gunNumber.setTextString(number);
    gunNumber.render(Area(SCREEN_WIDTH - layout.padding * 2, y, 10, 20));
}


void hudDisplay::renderDigits(const std::string& str, int x, int y, BitmapFont& texture) {
    for (char c: str) {
        if (isdigit(c)) {
            texture.renderDigit(c - '0', x, y, layout.scale);
            x += layout.digitSpacing;
        }
    }
}


void hudDisplay::updatePointerPosition(int x, int y) {
    pointerX = x;
    pointerY = y;
}
