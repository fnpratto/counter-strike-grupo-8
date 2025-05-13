#include "keyboardhandler.h"

#include <iostream>

#include <SDL2/SDL.h>

KeyboardHandler::KeyboardHandler() {}

void KeyboardHandler::handleEvent(const SDL_Event& event, bool& shop) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_DOWN:
                std::cout << "KEY_PRESS_DOWN" << std::endl;
                break;
            case SDLK_UP:
                std::cout << "KEY_PRESS_UP" << std::endl;
                break;
            case SDLK_LEFT:
                std::cout << "KEY_PRESS_LEFT" << std::endl;
                break;
            case SDLK_RIGHT:
                std::cout << "KEY_PRESS_RIGHT" << std::endl;
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
