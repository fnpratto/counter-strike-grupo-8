#include "score_display.h"

#include <iostream>
#include <map>
#include <unordered_map>
const std::string& FONT1_PAT = "../assets/gfx/fonts/joystix_monospace.otf";
const int pading = 10;

ScoreDisplay::ScoreDisplay(SdlWindow& window, std::map<std::string, ScoreboardEntry> score_board,
                           const GameUpdate& ref_state):
        window(window),
        state(ref_state),
        playerNameText("../assets/gfx/fonts/joystix_monospace.otf", 20,
                       SDL_Color{255, 255, 255, 255}, window),
        killsText(FONT1_PAT, 20, {255, 255, 255, 255}, window),
        deathsText(FONT1_PAT, 20, {255, 255, 255, 255}, window),
        scoreText(FONT1_PAT, 20, {255, 255, 255, 255}, window),
        moneyText(FONT1_PAT, 20, {255, 255, 255, 255}, window),
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
        slotHeight(DISPLAY_WIDTH * 0.03) {}

void ScoreDisplay::updateScoreboard(const std::map<std::string, ScoreboardEntry>& score_board) {
    scoreboard = score_board;
}

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
                                     int y, Team team) {
    if (team == Team::TT) {
        renderRectangle(red_rectangle, baseX, y, DISPLAY_WIDTH * 0.5, DISPLAY_WIDTH * 0.40);
    } else if (team == Team::CT) {
        renderRectangle(blue_rectangle, baseX, y, DISPLAY_WIDTH * 0.5, DISPLAY_WIDTH * 0.40);
    }

    int col1 = slotWidth * 0.30;
    int col2 = slotWidth * 0.15;  // Reuse for col3
    int col4 = slotWidth * 0.20;  // Reuse for col5

    int x1 = baseX;
    int x2 = x1 + col1;
    int x3 = x2 + col2;
    int x4 = x3 + col2;  // Reuse col2 for col3
    int x5 = x4 + col4;  // Reuse col4 for col5

    renderText(playerNameText, playerName, x1 + 10, y, col1 - 10, 30);
    renderText(killsText, "Kills: " + std::to_string(entry.kills), x2 + 10, y, col2 - 10, 30);
    renderText(deathsText, "Deaths: " + std::to_string(entry.deaths), x3 + 10, y, col2 - 10,
               30);  // Reuse col2 for col3
    renderText(scoreText, "Score: " + std::to_string(entry.score), x4 + 10, y, col4 - 10, 30);
    renderText(moneyText, "Money: " + std::to_string(entry.money), x5 + 10, y, col4 - 10,
               30);  // Reuse col4 for col5
}

void ScoreDisplay::renderRectangle(const SdlTexture& rectangle, int x, int y, int width,
                                   int height) {
    rectangle.render(Area(0, 0, 600, 300), Area(x, y, width, height));
}

void ScoreDisplay::renderTeamSlots(const SdlTexture& rectangle, int baseY, int slotCount) {
    for (int i = 0; i < slotCount; ++i) {
        renderRectangle(rectangle, baseX, baseY + i * (slotHeight + pading), slotWidth, slotHeight);
    }
}


void ScoreDisplay::renderLines(int baseY, int slotCount) {
    int col1 = slotWidth * 0.30;
    int col2 = slotWidth * 0.15;  // Reuse for col3
    int col4 = slotWidth * 0.20;

    int lineHeight = slotHeight * slotCount;

    renderLine(baseX + col1, baseY, lineHeight);                       // after Player Name
    renderLine(baseX + col1 + col2, baseY, lineHeight);                // after Kills
    renderLine(baseX + col1 + col2 + col4, baseY, lineHeight);         // after Deaths
    renderLine(baseX + col1 + col2 + col4 + col4, baseY, lineHeight);  // after Score
}

void ScoreDisplay::renderLine(int x, int baseY, int height) {
    line.render(Area(0, 0, 10, 600), Area(x, baseY, 10, height));
}

void ScoreDisplay::render() {
    int base_cc = baseY;
    int base_ct = baseY + slotHeight * 7 + pading;

    renderRectangle(black_rectangle, baseX, baseY, DISPLAY_WIDTH * 0.5, DISPLAY_WIDTH * 0.50);

    for (const auto& [playerName, entry]: scoreboard) {
        Team team = state.get_players().at(playerName).get_team();
        renderPlayerEntry(playerName, entry, base_cc, team);
        base_cc += slotHeight + pading;
    }

    renderRectangle(black_rectangle, baseX, base_cc, slotWidth, slotHeight);
    renderHeaderTexts(base_cc + 20);
    base_cc += slotHeight + pading;

    renderTeamSlots(red_rectangle, base_cc, 5);
    renderLines(base_cc, 7);

    renderRectangle(black_rectangle, baseX, base_ct, slotWidth, slotHeight);
    renderHeaderTexts(base_ct + 20);
    base_ct += slotHeight + pading;

    renderTeamSlots(blue_rectangle, base_ct, 5);
    renderLines(base_ct, 7);
}
