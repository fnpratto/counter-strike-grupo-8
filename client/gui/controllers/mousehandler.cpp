#include "mousehandler.h"

#include <iostream>

#include <SDL2/SDL.h>

void MouseHandler::handleEvent(const SDL_Event& event, bool shop) {
    int x, y;
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        switch (event.button.button) {
            case SDL_BUTTON_LEFT:
                std::cout << "MOUSE_PRESS_LEFT" << std::endl;
                if (shop) {
                    SDL_GetMouseState(&x, &y);
                    // shopDisplay.updatePointerPosition(x, y);
                }
                break;
            case SDL_BUTTON_RIGHT:
                std::cout << "MOUSE_PRESS_RIGHT" << std::endl;
                break;
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        SDL_GetMouseState(&x, &y);
        hudDisplayRef.updatePointerPosition(x, y);
        if (shop) {
            // shopDisplay.updatePointerPosition(x, y);
        }
    }
}
