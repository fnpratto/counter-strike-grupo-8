#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "../hud_component/bit_map_font_number.h"
#include "../window_elements/area.h"
#include "../window_elements/sdl_text.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/commands.h"
#include "common/message.h"
#include "common/models.h"

class skinSelect {
public:
    explicit skinSelect(SdlWindow& window, const GameUpdate& state, const std::string& player_name);
    void render();
    bool isActive();
    std::optional<CharacterType> updatePointerPosition(int x, int y);
    std::optional<bool> updateFinishPreGame(int x, int y);
    void activate() { active = true; }

private:
    void renderSkins();
    void renderButton();

    SdlWindow& window;
    const GameUpdate& game_state;
    const std::string& player_name;
    int DISPLAY_WIDTH;
    int DISPLAY_HEIGHT;

    SdlText text;
    SdlTexture background;
    SdlTexture skins_tt;
    SdlTexture skins_ct;

    float widthRatio, heightRatio, scaleRatio;
    float slot_width, slot_height;
    float base_x, base_y;
    float size_image;
    int padding = 10;
    int selected_skin;
    bool active;

    int select_skin_x, select_skin_y;
    int select_skin_width, select_skin_height;
};
