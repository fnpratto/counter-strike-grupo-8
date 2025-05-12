#include <exception>
#include <iostream>

#include <SDL2/SDL.h>

#include "gui/hud_component/hud_display.h"
#include "gui/window_elements/SdlWindow.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int, char**) {
    try {
        SdlWindow window(SCREEN_WIDTH, SCREEN_HEIGHT);
        hudDisplay hudDisplay(window);

        SDL_Event e;
        bool quit = false;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            hudDisplay.render();
        }

    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
