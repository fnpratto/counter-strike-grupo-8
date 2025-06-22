#pragma once

#include <memory>
#include <string>

#include "../window_elements/sdl_window.h"
#include "common/updates/game_update.h"

#include "hud_chat.h"
#include "hud_controls.h"
#include "hud_health.h"
#include "hud_money.h"
#include "hud_pointer.h"
#include "hud_scores.h"
#include "hud_timer.h"
#include "hud_weapons.h"

struct HudLayout {
    int iconWidth;
    int iconHeight;
    int size_width;
    int size_height;
    int digitHeight;
    int digitSpacing;
    int padding;
    float scale;
};


class SdlHud {
    const GameUpdate& state;
    const std::string& player_name;
    SdlWindow& window;

    HudLayout layout;
    float scale_ratio;

    std::unique_ptr<HudPointer> pointer_component;
    std::unique_ptr<HudMoney> money_component;
    std::unique_ptr<HudHealth> health_component;
    std::unique_ptr<HudTimer> timer_component;
    std::unique_ptr<HudWeapons> weapons_component;
    std::unique_ptr<HudScores> scores_component;
    std::unique_ptr<HudChat> chat_component;
    std::unique_ptr<HudControls> controls_component;

public:
    explicit SdlHud(SdlWindow& window, const GameUpdate& state, const std::string& player_name);
    void render();
    void update_pointer_position(int x, int y);
    void update_mute_icon();
    bool start_game_click(int x, int y);

private:
    void initialize_layout();
};
