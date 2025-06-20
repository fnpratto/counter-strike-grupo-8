#ifndef HUD_HEALTH_H
#define HUD_HEALTH_H

#include <string>

#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/updates/game_update.h"

#include "bit_map_font_number.h"

struct HudLayout;

class HudHealth {
private:
    SdlWindow& window;
    SdlTexture life;
    BitmapFont life_amount;
    const HudLayout& layout;

public:
    explicit HudHealth(SdlWindow& window, const HudLayout& layout);
    void render(const GameUpdate& state, const std::string& player_name);

private:
    void render_digits(const std::string& str, int x, int y);
};

#endif  // HUD_HEALTH_H
