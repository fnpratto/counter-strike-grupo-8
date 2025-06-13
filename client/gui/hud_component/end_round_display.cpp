#include "end_round_display.h"

#include <iostream>
#include <map>
#include <unordered_map>
const std::string& FONT1_PAT = "../assets/gfx/fonts/joystix_monospace.otf";
const std::string& BACKGROUND_WINNER_PATH = "../assets/gfx/hud/end_round.xcf";

const int pading = 10;

EndRoundDisplay::EndRoundDisplay(SdlWindow& window,

                                 const GameUpdate& ref_state):
        window(window),
        state(ref_state),
        background(BACKGROUND_WINNER_PATH, window),
        wining_team(FONT1_PAT, 20, {0, 255, 0, 255}, window),  // Green color for winning team
        losing_team(FONT1_PAT, 20, {255, 0, 0, 255}, window),  // Red color for losing team
        DISPLAY_WIDTH(window.getWidth()),
        DISPLAY_HEIGHT(window.getHeight()),
        slotWidth(DISPLAY_WIDTH * 0.5),
        slotHeight(DISPLAY_WIDTH * 0.1) {}

void EndRoundDisplay::render() {
    Area srcArea(0, 0, 680, 130);
    Area destArea(DISPLAY_WIDTH / 2 - slotWidth / 2, DISPLAY_HEIGHT / 2 - slotHeight, slotWidth,
                  slotHeight);
    background.render(srcArea, destArea);

    // TODO
    std::string name_winning_team = "COUNTER - TERRORISTS";
    wining_team.setTextString(name_winning_team + " WIN");
    wining_team.render(Area(destArea.getX() + slotWidth / 4, destArea.getY() + pading,
                            slotWidth / 2, slotHeight / 2));
    // TODO
    std::string name_losing_team = "TERRORIST";
    losing_team.setTextString(name_losing_team + " ELIMINATED");
    losing_team.render(Area(destArea.getX() + slotWidth / 3, destArea.getY() + slotHeight / 2,
                            slotWidth / 3, slotHeight / 3));
}
