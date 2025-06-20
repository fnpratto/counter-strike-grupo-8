#ifndef HUD_MONEY_H
#define HUD_MONEY_H

#include <string>

#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/updates/game_update.h"

#include "bit_map_font_number.h"

struct HudLayout;

class HudMoney {
private:
    SdlWindow& window;
    SdlTexture money;
    BitmapFont money_amount;
    const HudLayout& layout;

public:
    explicit HudMoney(SdlWindow& window, const HudLayout& layout);
    void render(const GameUpdate& state, const std::string& player_name);

private:
    void render_digits(const std::string& str, int x, int y);
};

#endif  // HUD_MONEY_H
