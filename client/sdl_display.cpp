#include "sdl_display.h"

#include <iostream>
#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#include "common/message.h"
#include "gui/hud_component/hud_display.h"
#include "gui/window_elements/sdl_window.h"

#include "display.h"
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


SDLDisplay::SDLDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue):
        Display(input_queue, output_queue) {}
void SDLDisplay::run() {
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
    }
}

void SDLDisplay::stop() { Thread::stop(); }
