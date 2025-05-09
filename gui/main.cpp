#include <exception>
#include <iostream>

#include <SDL2/SDL.h>

#include "window_elements/SdlTexture.h"
#include "window_elements/SdlWindow.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char** argv) {
    try {
        SdlWindow window(SCREEN_WIDTH, SCREEN_HEIGHT);
        SdlTexture back("assets/gfx/background.bmp", window);    // load background
        SdlTexture pointer("assets/gfx/pointer.xcf", window);    // load  pointer
        SdlTexture money("assets/gfx/hud_symbols.xcf", window);  // load  pointer
        SDL_Event e;
        Area SizePointer(1, 1, 44, 44);      // hud
        Area SizeBackground(35, 0, 30, 30);  // background
        Area SizeMoney(460, 0, 60, 60);      // money
        bool quit = false;
        while (!quit) {
            // Handle events on queue
            while (SDL_PollEvent(&e) != 0) {
                // User requests quit
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            Area destArea(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 28,
                          SCREEN_HEIGHT / 28);
            Area destAreaBackground(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            Area destAreaMoney(50, SCREEN_HEIGHT - 50, SCREEN_WIDTH / 28, SCREEN_HEIGHT / 28);
            window.fill();  // Repinto el fondo gris

            back.render(SizeBackground, destAreaBackground);
            pointer.render(SizePointer, destArea);
            money.render(SizeMoney, destAreaMoney);

            // poll event
            window.render();
        }


    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
