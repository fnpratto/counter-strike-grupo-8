#ifndef HUD_CHAT_H
#define HUD_CHAT_H

#include <string>

#include "../window_elements/sdl_text.h"
#include "../window_elements/sdl_window.h"
#include "common/updates/game_update.h"

struct HudLayout;

class HudChat {
private:
    SdlWindow& window;
    SdlText round_text;
    const HudLayout& layout;

public:
    explicit HudChat(SdlWindow& window, const HudLayout& layout);
    void render(const GameUpdate& state);
};

#endif  // HUD_CHAT_H
