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
        baseX(DISPLAY_WIDTH / 2 - DISPLAY_WIDTH * 0.25),
        baseY(DISPLAY_HEIGHT * 0.15),
        slotWidth(DISPLAY_WIDTH * 0.5),
        slotHeight(DISPLAY_WIDTH * 0.03) {}

void ScoreDisplay::updateScoreboard(const std::map<std::string, ScoreboardEntry>& score_board) {
    scoreboard = score_board;
}

void ScoreDisplay::render() {

    int y = baseY;
    int spacing = 10;
    int base_cc = baseY;
    int base_ct = baseY + slotHeight * 7 + 10;

    Area src1(0, 0, 600, 300);
    Area dest1(baseX, baseY, DISPLAY_WIDTH * 0.5, DISPLAY_WIDTH * 0.50);
    black_rectangle.render(src1, dest1);

    for (const auto& [playerName, entry]: scoreboard) {
        Team team = state.get_players().at(playerName).get_team();
        if (team == Team::TT) {
            red_rectangle.render(Area(0, 0, 600, 300),
                                 Area(baseX, y, DISPLAY_WIDTH * 0.5, DISPLAY_WIDTH * 0.40));
        } else if (team == Team::CT) {
            blue_rectangle.render(Area(0, 0, 600, 300),
                                  Area(baseX, y, DISPLAY_WIDTH * 0.5, DISPLAY_WIDTH * 0.40));
        }

        playerNameText.setTextString(playerName);
        playerNameText.render(Area(baseX, y, 150, 30));


        killsText.setTextString("Kills: " + std::to_string(entry.kills));
        killsText.render(Area(baseX + 160, y, 100, 30));


        deathsText.setTextString("Deaths: " + std::to_string(entry.deaths));
        deathsText.render(Area(baseX + 270, y, 100, 30));


        scoreText.setTextString("Score: " + std::to_string(entry.score));
        scoreText.render(Area(baseX + 380, y, 100, 30));


        moneyText.setTextString("Money: $" + std::to_string(entry.money));
        moneyText.render(Area(baseX + 490, y, 100, 30));

        y += spacing;
    }
    // header


    black_rectangle.render(Area(0, 0, 600, 300), Area(baseX, base_cc, slotWidth, slotHeight));
    base_cc += slotHeight + spacing;

    // Red team slots
    for (int i = 0; i < 5; ++i) {
        red_rectangle.render(Area(0, 0, 600, 300), Area(baseX, base_cc + i * (slotHeight + spacing),
                                                        slotWidth, slotHeight));
    }

    // First black rectangle before blue team
    black_rectangle.render(Area(0, 0, 600, 300), Area(baseX, base_ct, slotWidth, slotHeight));
    base_ct += slotHeight + spacing;

    // Blue team slots
    for (int i = 0; i < 5; ++i) {
        blue_rectangle.render(
                Area(0, 0, 600, 300),
                Area(baseX, base_ct + i * (slotHeight + spacing), slotWidth, slotHeight));
    }
}
