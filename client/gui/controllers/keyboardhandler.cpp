#include "keyboardhandler.h"

#include <iostream>

#include <SDL2/SDL.h>

KeyboardHandler::KeyboardHandler(Map& map_ref): map_ref(map_ref) {
    x = 300;
    y = 300;
}

void KeyboardHandler::handleEvent(const SDL_Event& event, bool& shop) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_DOWN:
                y += 10;
                std::cout << "KEY_PRESS_DOWN" << std::endl;
                map_ref.update_character(x, y);
                break;
            case SDLK_UP:
                y -= 10;
                std::cout << "KEY_PRESS_UP" << std::endl;
                map_ref.update_character(x, y);
                break;
            case SDLK_LEFT:
                x -= 10;
                std::cout << "KEY_PRESS_LEFT" << std::endl;
                map_ref.update_character(x, y);
                break;
            case SDLK_RIGHT:
                x += 10;
                std::cout << "KEY_PRESS_RIGHT" << std::endl;
                map_ref.update_character(x, y);
                break;
            case SDLK_SPACE:
                std::cout << "KEY_PRESS_SPACE" << std::endl;
                shop = false;
                break;
            case SDLK_b:
                shop = true;
                std::cout << "KEY_PRESS_B" << std::endl;
                break;
        }
    }
}
