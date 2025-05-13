#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include <SDL2/SDL.h>

#include "../hud_component/hud_display.h"
#include "../shop_view/shop.h"

class MouseHandler {
public:
    explicit MouseHandler(hudDisplay& hudDisplayRef): hudDisplayRef(hudDisplayRef) {}
    void handleEvent(const SDL_Event& event, bool shop);

private:
    hudDisplay& hudDisplayRef;
};

#endif  // MOUSEHANDLER_H
