#ifndef HUD_WEAPONS_H
#define HUD_WEAPONS_H

#include <string>

#include "../window_elements/sdl_text.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/updates/game_update.h"

#include "bit_map_font_number.h"

struct HudLayout;

class HudWeapons {
private:
    SdlWindow& window;
    SdlTexture equipped_bullets;
    SdlTexture white_line;
    BitmapFont equipped_bullets_amount;
    SdlText gun_number;
    SdlTexture guns_inventory_texture;
    const HudLayout& layout;

public:
    explicit HudWeapons(SdlWindow& window, const HudLayout& layout);
    void render(const GameUpdate& state, const std::string& player_name);

private:
    void render_bullets(const GameUpdate& state, const std::string& player_name);
    void render_gun_icons(const GameUpdate& state, const std::string& player_name);
    void render_digits(const std::string& str, int x, int y);
};

#endif  // HUD_WEAPONS_H
