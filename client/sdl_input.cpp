#include "sdl_input.h"

#include <SDL.h>

#include "gui/controllers/keyboardhandler.h"
#include "gui/controllers/mousehandler.h"

SDLInput::SDLInput(std::atomic<bool>& quit_flag, MouseHandler mouse_handler,
                   KeyboardHandler keyboard_handler):
        quit_flag(quit_flag),
        mouseHandler(std::move(mouse_handler)),
        keyboardHandler(std::move(keyboard_handler)) {}

void SDLInput::run() {
    while (!quit_flag) {
        // bool shop = false;
        // bool list_teams = false;
        //  bool isMuted = false;
        SDL_Event e;
        if (SDL_WaitEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit_flag = true;
            }
            keyboardHandler.handleEvent(e);
            mouseHandler.handleEvent(e);  // TODO: shop and list_teams flags
        }
    }
    std::cout << "SDLInput thread stopped\n";
}
