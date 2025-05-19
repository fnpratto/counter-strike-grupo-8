#ifndef SHOP_DISPLAY_H
#define SHOP_DISPLAY_H

#include <string>
#include <vector>

#include "../window_elements/Area.h"
#include "../window_elements/SdlText.h"
#include "../window_elements/SdlTexture.h"
#include "../window_elements/SdlWindow.h"


class shopDisplay {
public:
    explicit shopDisplay(SdlWindow& window);
    void render();
    void updatePointerPosition(int x, int y);

private:
    SdlWindow& window;
    int DISPLAY_WIDTH;
    int DISPLAY_HEIGHT;
    std::vector<SdlTexture> gun_icons;
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
