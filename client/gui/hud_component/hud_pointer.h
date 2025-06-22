#ifndef HUD_POINTER_H
#define HUD_POINTER_H

#include "../window_elements/area.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"

struct HudLayout;

class HudPointer {
private:
    SdlWindow& window;
    SdlTexture pointer;
    int pointer_x;
    int pointer_y;
    const HudLayout& layout;

public:
    explicit HudPointer(SdlWindow& window, const HudLayout& layout);
    void render();
    void update_position(int x, int y);
};

#endif  // HUD_POINTER_H
