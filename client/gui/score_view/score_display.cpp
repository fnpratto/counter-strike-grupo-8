#include "score_display.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
const int pading = 10;

ScoreDisplay::ScoreDisplay(SdlWindow& window, std::map<std::string, ScoreboardEntry> score_board,
                           const GameUpdate& ref_state):
        window(window),
        state(ref_state),
        playerNameText(std::string(GameConfig::Paths::FONT_PATH), 20, SDL_Color{255, 255, 255, 255},
                       window),
        killsText(std::string(GameConfig::Paths::FONT_PATH), 20, {255, 255, 255, 255}, window),
        deathsText(std::string(GameConfig::Paths::FONT_PATH), 20, {255, 255, 255, 255}, window),
        scoreText(std::string(GameConfig::Paths::FONT_PATH), 20, {255, 255, 255, 255}, window),
        moneyText(std::string(GameConfig::Paths::FONT_PATH), 20, {255, 255, 255, 255}, window),
        scoreboard(score_board),
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()),
        black_rectangle("../assets/gfx/listTeams/rectangulo.xcf", window),
        red_rectangle("../assets/gfx/listScores/rectangulo_red.xcf", window),
        blue_rectangle("../assets/gfx/listScores/rectangulo_blue.xcf", window),
        line("../assets/gfx/listScores/line.xcf", window),
        baseX(DISPLAY_WIDTH / 2 - DISPLAY_WIDTH * 0.25),
        baseY(DISPLAY_HEIGHT * 0.15),
        slotWidth(DISPLAY_WIDTH * 0.5),
        slotHeight(DISPLAY_WIDTH * 0.03),
        active(false) {}

void ScoreDisplay::updateScoreboard(const std::map<std::string, ScoreboardEntry>& score_board) {
    std::cout << "Updating scoreboard with " << score_board.size() << " entries." << std::endl;
    if (score_board.empty()) {
        std::cout << "Scoreboard is empty, no updates made." << std::endl;
        return;
    }
    scoreboard = score_board;
}

bool ScoreDisplay::isActive() { return active; }

void ScoreDisplay::renderHeaderTexts(int y) {
    int col1 = slotWidth * 0.30;
    int col2 = slotWidth * 0.15;
    int col4 = slotWidth * 0.20;

    int x1 = baseX;
    int x2 = x1 + col1;
    int x3 = x2 + col2;
    int x4 = x3 + col2;  // Reuse col2 for col3
    int x5 = x4 + col4;

    renderText(playerNameText, "Player Name", x1 + 10, y, col1 - 20, 30);
    renderText(killsText, "Kills", x2 + 20, y, col2 - 30, 30);
    renderText(deathsText, "Deaths", x3 + 20, y, col2 - 30, 30);  // Reuse col2 for col3
    renderText(scoreText, "Score", x4 + 20, y, col4 - 30, 30);
    renderText(moneyText, "Money", x5 + 20, y, col4 - 30, 30);
}

void ScoreDisplay::renderText(SdlText& text, const std::string& content, int x, int y, int width,
                              int height) {
    text.setTextString(content);
    text.render(Area(x, y, width, height));
}

void ScoreDisplay::renderPlayerEntry(const std::string& playerName, const ScoreboardEntry& entry,
                                     int y) {

    int col1 = slotWidth * 0.30;
    int col2 = slotWidth * 0.15;  // Reuse for col3
    int col4 = slotWidth * 0.20;  // Reuse for col5

    int x1 = baseX;
    int x2 = x1 + col1;
    int x3 = x2 + col2;
    int x4 = x3 + col2;  // Reuse col2 for col3
    int x5 = x4 + col4;  // Reuse col4 for col5

    int nameWidth = playerName.length() * 30;
    int killsWidth = std::to_string(entry.kills).length() * 30;
    int deathsWidth = std::to_string(entry.deaths).length() * 30;
    int scoreWidth = std::to_string(entry.score).length() * 30;
    int moneyWidth = std::to_string(entry.money).length() * 30;

    renderText(playerNameText, playerName, x1 + 10, y, nameWidth, 30);
    renderText(killsText, std::to_string(entry.kills), x2 + 20, y, killsWidth, 30);
    renderText(deathsText, std::to_string(entry.deaths), x3 + 20, y, deathsWidth, 30);
    renderText(scoreText, std::to_string(entry.score), x4 + 20, y, scoreWidth, 30);
    renderText(moneyText, std::to_string(entry.money), x5 + 20, y, moneyWidth, 30);
}

void ScoreDisplay::renderRectangle(const SdlTexture& rectangle, int x, int y, int width,
                                   int height) {
    rectangle.render(Area(0, 0, 600, 300), Area(x, y, width, height));
}

void ScoreDisplay::renderTeamSlot(const SdlTexture& rectangle, int baseY) {
    renderRectangle(rectangle, baseX, baseY, slotWidth, slotHeight);
}


void ScoreDisplay::renderLines(int baseY, int slotCount) {
    int col1 = slotWidth * 0.30;
    int col2 = slotWidth * 0.15;  // Reuse for col3
    int col4 = slotWidth * 0.20;

    int lineHeight = slotHeight * slotCount;

    renderLine(baseX + col1, baseY, lineHeight);                       // after Player Name
    renderLine(baseX + col1 + col2, baseY, lineHeight);                // after Kills
    renderLine(baseX + col1 + col2 + col2, baseY, lineHeight);         // after Deaths
    renderLine(baseX + col1 + col2 + col2 + col4, baseY, lineHeight);  // after Score
}

void ScoreDisplay::renderLine(int x, int baseY, int height) {
    line.render(Area(0, 0, 10, 600), Area(x, baseY, 10, height));
}

void ScoreDisplay::render() {
    int base_cc = baseY;
    int base_ct = baseY + slotHeight * 7 + pading;

    renderRectangle(black_rectangle, baseX, baseY, DISPLAY_WIDTH * 0.5, DISPLAY_WIDTH * 0.50);

    renderRectangle(black_rectangle, baseX, base_cc, slotWidth, slotHeight);
    renderHeaderTexts(base_cc + 20);
    base_cc += slotHeight + pading;

    renderRectangle(black_rectangle, baseX, base_ct, slotWidth, slotHeight);
    renderHeaderTexts(base_ct + 20);
    base_ct += slotHeight + pading;

    for (const auto& [playerName, entry]: scoreboard) {
        std::cout << "Rendering player: " << playerName << std::endl;
        Team team = state.get_players().at(playerName).get_team();
        if (team == Team::TT) {
            renderTeamSlot(red_rectangle, base_cc);
            renderPlayerEntry(playerName, entry, base_cc);
            base_cc += slotHeight + pading;
        } else if (team == Team::CT) {
            renderTeamSlot(blue_rectangle, base_ct);
            renderPlayerEntry(playerName, entry, base_ct);
            base_ct += slotHeight + pading;
        }
    }
    renderLines(baseY, 7);
    renderLines(baseY + slotHeight * 7 + pading, 7);
}
