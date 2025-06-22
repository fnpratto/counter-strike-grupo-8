#include "end_round_display.h"

#include <iostream>
#include <map>
#include <unordered_map>

const int pading = 10;

EndRoundDisplay::EndRoundDisplay(SdlWindow& window,

                                 const GameUpdate& ref_state):
        window(window),
        state(ref_state),
        background(std::string(GameConfig::Paths::BACKGROUND_WINNER_PATH), window),
        wining_team(std::string(GameConfig::Paths::FONT_PATH), 20, {0, 255, 0, 255},
                    window),  // Green color for winning team
        losing_team(std::string(GameConfig::Paths::FONT_PATH), 20, {255, 0, 0, 255},
                    window),  // Red color for losing team
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()),
        slotWidth(DISPLAY_WIDTH * 0.5),
        slotHeight(DISPLAY_WIDTH * 0.1) {}

void EndRoundDisplay::render() {
    Area srcArea(0, 0, 680, 130);
    Area destArea(DISPLAY_WIDTH / 2 - slotWidth / 2, DISPLAY_HEIGHT / 2 - slotHeight, slotWidth,
                  slotHeight);
    background.render(srcArea, destArea);

    wining_team.render(Area(destArea.getX() + slotWidth / 4, destArea.getY() + pading,
                            slotWidth / 2, slotHeight / 2));
    losing_team.render(Area(destArea.getX() + slotWidth / 3, destArea.getY() + slotHeight / 2,
                            slotWidth / 3, slotHeight / 3));
}

void EndRoundDisplay::update_winner_team(Team winner) {
    if (winner == Team::CT) {
        wining_team.setTextString("COUNTER - TERRORISTS WIN");
        losing_team.setTextString("TERRORISTS ELIMINATED");
    } else {
        wining_team.setTextString("TERRORISTS WIN");
        losing_team.setTextString("COUNTER - TERRORISTS ELIMINATED");
    }
}
