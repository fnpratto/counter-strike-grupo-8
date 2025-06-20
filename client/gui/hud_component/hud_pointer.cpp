#include "hud_pointer.h"

#include <string>

#include "../../game_config.h"

#include "hud_display.h"

HudPointer::HudPointer(SdlWindow& window, const HudLayout& layout):
        window(window),
        pointer(std::string(GameConfig::Paths::POINTER_PATH), window),
        pointer_x(window.getWidth() / 2),
        pointer_y(window.getHeight() / 2),
        layout(layout) {}

void HudPointer::render() {
    const Area sizePointer(0, 0, 50, 50);
    const Area destPointer(pointer_x - 25, pointer_y - 25, 50, 50);
    pointer.render(sizePointer, destPointer);
}

void HudPointer::update_position(int x, int y) {
    pointer_x = x;
    pointer_y = y;
}
