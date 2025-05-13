#include <exception>
#include <iostream>

#include <SDL2/SDL.h>

#include "gui/controllers/keyboardhandler.h"
#include "gui/controllers/mousehandler.h"
#include "gui/hud_component/hud_display.h"
#include "gui/shop_view/shop.h"
#include "gui/window_elements/SdlWindow.h"


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void keyboard_update(const SDL_Event& event, bool& shop) {
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


void mouse_update(const SDL_Event& event, hudDisplay& hudDisplay, shopDisplay& shopDisplay,
                  bool shop) {
    int x, y;
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        switch (event.button.button) {
            case SDL_BUTTON_LEFT:
                std::cout << "MOUSE_PRESS_LEFT" << std::endl;
                if (shop) {
                    shopDisplay.updatePointerPosition(x, y);
                }
                break;
            case SDL_BUTTON_RIGHT:
                std::cout << "MOUSE_PRESS_RIGHT" << std::endl;
                break;
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        SDL_GetMouseState(&x, &y);
        hudDisplay.updatePointerPosition(x, y);
    }
}

int main(int, char**) {
    try {
        SdlWindow window(SCREEN_WIDTH, SCREEN_HEIGHT);
        hudDisplay hudDisplay(window);
        shopDisplay shopDisplay(window);
        MouseHandler mouseHandler(hudDisplay, shopDisplay);
        KeyboardHandler keyboardHandler;
        bool shop = false;

        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                keyboardHandler.handleEvent(e, shop);
                mouseHandler.handleEvent(e, shop);
            }
            window.fill();
            hudDisplay.render();
            if (shop) {
                shopDisplay.render();
            }
            window.render();
        }

    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
