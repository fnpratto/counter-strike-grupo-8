#include "hud_display.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <map>
#include <vector>

#include "../../game_config.h"

hudDisplay::hudDisplay(SdlWindow& window, const GameUpdate& state, const std::string& player_name):
        state(state),
        player_name(player_name),
        window(window),
        SCREEN_WIDTH(window.getWidth()),
        SCREEN_HEIGHT(window.getHeight()),
        pointer(std::string(GameConfig::Paths::POINTER_PATH), window),
        money(std::string(GameConfig::Paths::MONEY_PATH), window),
        money_amount(window.getRenderer(), std::string(GameConfig::Paths::HUD_NUMS_XCF)),
        life(std::string(GameConfig::Paths::LIFE_PATH), window),
        life_amount(window.getRenderer(), std::string(GameConfig::Paths::HUD_NUMS_XCF)),
        equipedBullets(std::string(GameConfig::Paths::BULLET_PATH), window),
        white_line(std::string(GameConfig::Paths::WHITE_LINE_PATH), window),
        equipedBulletsAmount(window.getRenderer(), std::string(GameConfig::Paths::HUD_NUMS_XCF)),
        timer_amount(window.getRenderer(), std::string(GameConfig::Paths::HUD_NUMS_XCF)),
        timer_dots(std::string(GameConfig::Paths::HUD_NUMS_XCF), window),
        roundText(std::string(GameConfig::Paths::FONT_PATH), 20, {150, 150, 150, 255}, window),
        gunNumber(std::string(GameConfig::Paths::FONT_PATH), 20, {150, 150, 150, 255}, window),
        scoreText(std::string(GameConfig::Paths::FONT_PATH), 20, {255, 255, 255, 255}, window),
        muteIcon(std::string(GameConfig::Paths::MUTE_ICON_PATH), window),
        gunsInventoryTexture(std::string(GameConfig::Paths::GUNS_INVENTORY_PATH), window),
        start_game_buttom(std::string(GameConfig::Paths::BACKGROUND_WINNER_PATH), window) {
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

void hudDisplay::render() {
    renderBackground();
    renderTeamScores();
    renderPointer();
    renderMoney();
    renderLife();
    renderTimer();
    renderRoundText();
    renderBullets();
    renderGunIcons();
    renderMuteIcon(false);
    if (state.get_phase().get_phase() == PhaseType::WarmUp) {
        renderStartGameButton();
        renderChat();
    }
}

void hudDisplay::renderChat() {
    int totalPlayers = state.get_players().size();
    int readyPlayers = 0;

    for (const auto& [name, player_state]: state.get_players()) {
        if (player_state.get_ready()) {
            readyPlayers++;
            roundText.setTextString(name + " join game");

            Area chatArea(
                    layout.padding,
                    SCREEN_HEIGHT - layout.size_height * 2 - readyPlayers * layout.size_height / 2,
                    layout.size_width * 1.2, layout.size_height / 2);
            roundText.render(chatArea);
        }
    }

    std::string readyStatus = std::to_string(readyPlayers) + "/" + std::to_string(totalPlayers);
    roundText.setTextString("Ready: " + readyStatus);

    Area statusArea(50, layout.size_height, layout.size_width * 2, layout.size_height * 1.2);
    roundText.render(statusArea);
}

void hudDisplay::renderStartGameButton() {
    int buttonWidth = static_cast<int>(280 * scaleRatio);
    int buttonHeight = static_cast<int>(50 * scaleRatio);
    const Area sizeButton(0, 0, 680, 130);
    const Area destButton(SCREEN_WIDTH / 2 - buttonWidth / 2,
                          SCREEN_HEIGHT - buttonHeight - layout.padding * 2, buttonWidth,
                          buttonHeight);
    start_game_buttom.render(sizeButton, destButton);

    roundText.setTextString("START GAME");
    roundText.render(Area(SCREEN_WIDTH / 2 - buttonWidth * 0.5 / 2 + layout.padding,
                          SCREEN_HEIGHT - buttonHeight - layout.padding * 2 + layout.padding,
                          buttonWidth * 0.5 - layout.padding * 2,
                          buttonHeight - layout.padding * 2));
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
    SDL_Renderer* renderer = window.getRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_Rect rect = {0, 0, SCREEN_WIDTH, 10};
    SDL_RenderFillRect(renderer, &rect);
    SdlTexture trapecio(std::string(GameConfig::Paths::TRAPECIO_PATH), window);
    Area srcTrapecio(0, 0, 500, 230);
    Area destTrapecio(SCREEN_WIDTH * 0.5 - SCREEN_WIDTH * 0.5 / 2, 0, SCREEN_WIDTH * 0.5,
                      layout.size_height * 1.75);
    trapecio.render(srcTrapecio, destTrapecio);
}

// Render trapecio
void hudDisplay::renderTeamScores() {
    const int trapecioWidth = SCREEN_WIDTH * 0.5;
    const int trapecioX = (SCREEN_WIDTH - trapecioWidth) / 4;

    const int paralWidth = layout.size_width * 4;
    const int paralHeight = layout.size_height * 1.75;
    const int margin = layout.padding * 2;  // espacio entre trapecio y paralelogramas

    // BLUE (izquierda del trapecio)
    SdlTexture parallelogram1(std::string(GameConfig::Paths::PARALELO_BLUE_O_PATH), window);
    Area srcParallelogram1(0, 180, 500, 320);
    Area destParallelogram1(trapecioX - paralWidth / 4 + margin, layout.padding * 2, paralWidth,
                            paralHeight);
    parallelogram1.render(srcParallelogram1, destParallelogram1);

    scoreText.setTextString(std::to_string(0));  // TODO : Replace with actual score
    scoreText.render(Area(trapecioX - paralWidth / 4 + margin * 6, layout.padding * 3,
                          layout.size_width / 1.75, layout.size_height / 2));

    // RED (derecha del trapecio)
    SdlTexture parallelogram(std::string(GameConfig::Paths::PARALELO_RED_PATH), window);
    Area srcParallelogram(0, 180, 500, 320);
    Area destParallelogram(trapecioX + trapecioWidth + margin, layout.padding * 2, paralWidth,
                           paralHeight);
    parallelogram.render(srcParallelogram, destParallelogram);

    scoreText.setTextString(std::to_string(0));  // TODO : Replace with actual score
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

    std::string moneyStr =
            std::to_string(state.get_players().at(player_name).get_inventory().get_money());
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

    std::string lifeStr = std::to_string(state.get_players().at(player_name).get_health());
    int x = layout.padding + iconWidth + layout.digitSpacing / 2;
    int y = SCREEN_HEIGHT - iconHeight - layout.padding;
    renderDigits(lifeStr, x, y, life_amount);
}


void hudDisplay::renderTimer() {
    int secsRemaining = state.get_phase().get_secs_remaining();
    int minutesIdx = secsRemaining / 60;
    int secondsIdx = secsRemaining % 60;
    int secondsIdxH = secondsIdx / 10;
    int secondsIdxL = secondsIdx % 10;

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
    roundText.setTextString("Round " + std::to_string(state.get_num_rounds()));
    roundText.render(Area(SCREEN_WIDTH / 2 - 50, layout.padding, 100, 20));
}


void hudDisplay::renderBullets() {
    int iconHeight = static_cast<int>(32 * scaleRatio);
    const Area sizeBullets(0, 0, 300, 302);
    const Area destBullets(SCREEN_WIDTH - SCREEN_WIDTH / 40 - layout.padding * 2,
                           SCREEN_HEIGHT - iconHeight * 3, 64 * layout.scale, 64 * layout.scale);
    equipedBullets.render(sizeBullets, destBullets);


    const auto& player = state.get_players().at(player_name);
    const auto& inventory = player.get_inventory();
    const auto& guns = inventory.get_guns();
    ItemSlot equipped = player.get_equipped_item();

    auto it = guns.find(equipped);
    if (it == guns.end())
        return;


    std::string bulletsStr = std::to_string(
            guns.at(state.get_players().at(player_name).get_equipped_item()).get_mag_ammo());

    std::string bulletsReserve = std::to_string(
            guns.at(state.get_players().at(player_name).get_equipped_item()).get_reserve_ammo());

    int x = SCREEN_WIDTH - layout.size_width - SCREEN_WIDTH / 40 - layout.padding * 8;
    int y = SCREEN_HEIGHT - iconHeight * 3;

    renderDigits(bulletsStr, x, y, equipedBulletsAmount);

    x += layout.digitSpacing * bulletsStr.size();

    const Area sizeWhiteLine(0, 0, 30, 300);
    const Area destWhiteLine(x + 5, y, 5 * scaleRatio, 32 * scaleRatio);
    white_line.render(sizeWhiteLine, destWhiteLine);

    x += 5 * scaleRatio + 5;

    renderDigits(bulletsReserve, x, y, equipedBulletsAmount);
}


void hudDisplay::renderGunIcons() {
    const auto& inventory = state.get_players().at(player_name).get_inventory();
    const auto& guns = inventory.get_guns();

    int x = SCREEN_WIDTH - layout.size_width - SCREEN_WIDTH / 20;
    int y = SCREEN_HEIGHT / 2;
    static constexpr int iconWidth = 40;
    static constexpr int iconHeight = 12;
    static constexpr int spacing = 64;

    std::map<ItemSlot, std::string> slotToKey = {{ItemSlot::Primary, "1"},
                                                 {ItemSlot::Secondary, "2"},
                                                 {ItemSlot::Melee, "3"},
                                                 {ItemSlot::Bomb, "4"}};

    for (const auto& [slot, gun]: guns) {
        GunType type = gun.get_gun();

        int iconIndex = -1;
        if (slot == ItemSlot::Primary || slot == ItemSlot::Secondary) {
            std::string key = std::string(GameConfig::GunTypeToStr(type));
            if (GameConfig::offsetInventory.count(key)) {
                iconIndex = GameConfig::offsetInventory[key];
            }
        }

        if (iconIndex != -1) {
            Area src(iconIndex * iconWidth, 0, iconWidth, iconHeight);
            Area dest(x, y, SCREEN_WIDTH / 14, 34);

            gunsInventoryTexture.render(src, dest);
            gunNumber.setTextString(slotToKey[slot]);
            gunNumber.render(Area(SCREEN_WIDTH - layout.padding * 2, y, 10, 20));

            y += spacing;
        }
    }

    // Render knife (always present)
    int knifeIndex = GameConfig::offsetInventory["knife"];
    Area knifeSrc(knifeIndex * iconWidth, 0, iconWidth, iconHeight);
    Area knifeDest(x, y, SCREEN_WIDTH / 14, 34);
    gunsInventoryTexture.render(knifeSrc, knifeDest);
    gunNumber.setTextString(slotToKey[ItemSlot::Melee]);
    gunNumber.render(Area(SCREEN_WIDTH - layout.padding * 2, y, 10, 20));

    y += spacing;

    // Only render bomb if it exists
    try {
        if (!inventory.get_bomb().has_value()) {
            return;
        }
    } catch (const std::exception& e) {
        return;
    }
    int bombIndex = GameConfig::offsetInventory["bomb"];
    Area bombSrc(bombIndex * iconWidth, 0, iconWidth, iconHeight);
    Area bombDest(x, y, SCREEN_WIDTH / 14, 34);

    gunsInventoryTexture.render(bombSrc, bombDest);
    gunNumber.setTextString(slotToKey[ItemSlot::Bomb]);
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

bool hudDisplay::start_game_click(int x, int y) {
    int buttonWidth = static_cast<int>(200 * scaleRatio);
    int buttonHeight = static_cast<int>(50 * scaleRatio);
    int buttonX = SCREEN_WIDTH / 2 - buttonWidth / 2;
    int buttonY = SCREEN_HEIGHT - buttonHeight - layout.padding * 2;

    if (x >= buttonX && x <= buttonX + buttonWidth && y >= buttonY && y <= buttonY + buttonHeight) {
        if (!in_game) {
            in_game = true;
            return true;
        }
    }
    return false;
}
