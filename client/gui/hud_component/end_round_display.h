#ifndef WINNER_DISPLAY_H
#define WINNER_DISPLAY_H

#include <map>
#include <string>

#include "../../game_config.h"
#include "../window_elements/area.h"
#include "../window_elements/sdl_text.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/game/scoreboard_entry.h"
#include "common/message.h"

class EndRoundDisplay {
private:
    SdlWindow& window;
    const GameUpdate& state;
    SdlTexture background;
    SdlText wining_team;
    SdlText losing_team;
    int DISPLAY_WIDTH;
    int DISPLAY_HEIGHT;
    int slotWidth;
    int slotHeight;

public:
    explicit EndRoundDisplay(SdlWindow& window, const GameUpdate& state);
    void render();
};

#endif  // SCORE_DISPLAY_H
