#ifndef SCORE_DISPLAY_H
#define SCORE_DISPLAY_H

#include <map>
#include <string>

#include "../window_elements/area.h"
#include "../window_elements/sdl_text.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/message.h"
#include "common/scoreboard/scoreboard_entry.h"

class ScoreDisplay {
private:
    SdlWindow& window;
    const GameUpdate& state;
    SdlText playerNameText;
    SdlText killsText;
    SdlText deathsText;
    SdlText scoreText;
    SdlText moneyText;
    std::map<std::string, ScoreboardEntry> scoreboard;
    int DISPLAY_WIDTH;
    int DISPLAY_HEIGHT;
    SdlTexture black_rectangle;
    SdlTexture red_rectangle;
    SdlTexture blue_rectangle;
    int baseX;
    int baseY;
    int slotWidth;
    int slotHeight;

public:
    explicit ScoreDisplay(SdlWindow& window, std::map<std::string, ScoreboardEntry> scoreboard,
                          const GameUpdate& state);

    void render();
    void updateScoreboard(const std::map<std::string, ScoreboardEntry>& scoreboard);  // Update data
};

#endif  // SCORE_DISPLAY_H
