#ifndef HUD_TIMER_H
#define HUD_TIMER_H

#include "../window_elements/sdl_text.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/updates/game_update.h"

#include "bit_map_font_number.h"

struct HudLayout;

class HudTimer {
private:
    SdlWindow& window;
    BitmapFont timer_amount;
    SdlTexture timer_dots;
    SdlText round_text;
    const HudLayout& layout;

public:
    explicit HudTimer(SdlWindow& window, const HudLayout& layout);
    void render(const GameUpdate& state);
    void render_round_text(const GameUpdate& state);
};

#endif  // HUD_TIMER_H
