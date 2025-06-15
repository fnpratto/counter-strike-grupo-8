#ifndef SCORE_DISPLAY_H
#define SCORE_DISPLAY_H

#include <map>
#include <string>

#include "../../game_config.h"
#include "../window_elements/area.h"
#include "../window_elements/sdl_text.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/game/scoreboard_entry.h"
#include "common/message.h"

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
    SdlTexture line;
    int baseX;
    int baseY;
    int slotWidth;
    int slotHeight;
    bool active;

    void renderHeaderTexts(int y);
    void renderText(SdlText& text, const std::string& content, int x, int y, int width, int height);
    void renderPlayerEntry(const std::string& playerName, const ScoreboardEntry& entry, int y);
    void renderRectangle(const SdlTexture& rectangle, int x, int y, int width, int height);
    void renderTeamSlot(const SdlTexture& rectangle, int baseY);
    void renderLine(int x, int baseY, int height);
    void renderLines(int baseY, int slotCount);

public:
    explicit ScoreDisplay(SdlWindow& window, std::map<std::string, ScoreboardEntry> scoreboard,
                          const GameUpdate& state);

    void render();
    void updateScoreboard(const std::map<std::string, ScoreboardEntry>& scoreboard);  // Update data
    void updateState() { active = !active; }
    bool isActive();
};

#endif  // SCORE_DISPLAY_H
