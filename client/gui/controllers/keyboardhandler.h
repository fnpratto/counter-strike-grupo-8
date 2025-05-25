#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <SDL2/SDL.h>

#include "../map_view/map_view.h"

class KeyboardHandler {
public:
    explicit KeyboardHandler(Map& map_ref);
    void handleEvent(const SDL_Event& event, bool& shop);

private:
    Map& map_ref;
    int x;  // TODO sacar
    int y;  // TODO sacar
};

#endif  // KEYBOARDHANDLER_H
