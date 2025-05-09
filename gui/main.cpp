#include <exception>
#include <iostream>

#include <SDL2/SDL.h>

#include "window_elements/SdlTexture.h"
#include "window_elements/SdlWindow.h"

int main(int argc, char** argv) {
    try {
        SdlWindow window(800, 600);
        window.fill();
        SdlTexture im("assets/gfx/aztec.bmp", window);  // load background
        SDL_Event e;
        Area srcArea(0, 0, 300, 300);
        bool quit = false;
        int x = 100;
        int y = 150;
        while (!quit) {
            // Handle events on queue
            while (SDL_PollEvent(&e) != 0) {
                // User requests quit
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            Area destArea(x, y, 300, 300);
            window.fill();  // Repinto el fondo gris
            im.render(srcArea, destArea);
            // poll event
            window.render();
        }


    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
