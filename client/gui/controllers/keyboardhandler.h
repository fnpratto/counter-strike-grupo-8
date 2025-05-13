#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <SDL2/SDL.h>

class KeyboardHandler {
public:
    KeyboardHandler();
    void handleEvent(const SDL_Event& event, bool& shop);
};

#endif  // KEYBOARDHANDLER_H
