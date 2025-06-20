#ifndef HUD_CONTROLS_H
#define HUD_CONTROLS_H

#include "../window_elements/sdl_text.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"

struct HudLayout;

class HudControls {
private:
    SdlWindow& window;
    SdlTexture start_game_button;
    SdlText round_text;
    const HudLayout& layout;
    float scale_ratio;

public:
    explicit HudControls(SdlWindow& window, const HudLayout& layout, float scale_ratio);
    void render_start_game_button();
    bool start_game_click(int x, int y);
};

#endif  // HUD_CONTROLS_H
