#ifndef SHOP_DISPLAY_H
#define SHOP_DISPLAY_H

#include <optional>
#include <string>
#include <vector>

#include "../window_elements/area.h"
#include "../window_elements/sdl_text.h"
#include "../window_elements/sdl_texture.h"
#include "../window_elements/sdl_window.h"
#include "common/message.h"


class shopDisplay {
public:
    explicit shopDisplay(SdlWindow& window);
    void render();
    std::optional<Message> updatePointerPosition(int x, int y);

private:
    SdlWindow& window;
    int DISPLAY_WIDTH;
    int DISPLAY_HEIGHT;
    SdlTexture gun_icons;
    SdlTexture ammo_icons;
    SdlText cost_money;
    SdlTexture back;
    SdlTexture back_chosen;
    SdlText gunNumber;
    int gun_buy;
    int size_guns_w;
    int size_guns_h;
    int size_slots_w;
    int size_slots_h;
    void renderSlots();
    void renderItem();
    void renderblokedSlots();
};

#endif  // SHOP_DISPLAY_H
