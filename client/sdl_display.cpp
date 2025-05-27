#include "sdl_display.h"

#include <iostream>
#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#include "../client/requests.h"
#include "../common/message.h"
#include "common/message.h"
#include "gui/controllers/keyboardhandler.h"
#include "gui/controllers/mousehandler.h"
#include "gui/hud_component/hud_display.h"
#include "gui/map_view/map_view.h"
#include "gui/pre_game_view/list_teams.h"
#include "gui/shop_view/shop.h"
#include "gui/window_elements/sdl_window.h"

#include "display.h"

void update_components() {}

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

SDLDisplay::SDLDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue):
        Display(input_queue, output_queue) {}

void SDLDisplay::run() {

    // FOR FULL SIZE
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
        std::cerr << "SDL_GetCurrentDisplayMode failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
    int SCREEN_WIDTH = displayMode.w;
    int SCREEN_HEIGHT = displayMode.h - 150;

    /*const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;*/

    /*TODO maybe will change*/
    Uint32 RATE = 16;  // Define RATE as the frame duration in milliseconds (e.g., 16ms for ~60 FPS)
    Uint32 frame_start = SDL_GetTicks();
    Uint32 frame_end;
    Uint32 behind;
    Uint32 lost;

    try {
        SdlWindow window(SCREEN_WIDTH, SCREEN_HEIGHT);
        hudDisplay hudDisplay(window);
        shopDisplay shopDisplay(window);
        Map map(window);
        listTeams listTeams(window);

        MouseHandler mouseHandler(hudDisplay, shopDisplay, listTeams);
        KeyboardHandler keyboardHandler(map);
        SDL_Event e;

        bool quit = false;
        bool shop = false;
        bool list_teams = true;
        int clock = 0;  // por ahora
        bool isMuted = false;

        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                keyboardHandler.handleEvent(e, shop, isMuted);
                mouseHandler.handleEvent(e, shop, list_teams);
            }
            /* update clock */
            frame_end = SDL_GetTicks();
            int elapsed_time = frame_end - frame_start;
            int rest_time = RATE - elapsed_time;

            if (rest_time < 0) {
                behind = -rest_time;
                rest_time = RATE - (behind % RATE);
                lost = behind / RATE;
                frame_start += lost * RATE;
                // clock += lost;  // Increment clock for lost frames
            }

            SDL_Delay(rest_time);
            frame_start += RATE;

            // Increment clock if a second has passed
            static Uint32 accumulated_time = 0;
            accumulated_time += RATE;
            if (accumulated_time >= 1000) {  // 1000 ms = 1 second
                clock++;
                accumulated_time -= 1000;
            }

            /*update --> pull event from the queue*/
            window.fill();
            if (clock > 1) {
                map.render();
                hudDisplay.update(clock - 10, isMuted);
                if (shop) {
                    shopDisplay.render();
                }
            } else {
                listTeams.update(clock);
            }

            window.render();
        }
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void SDLDisplay::stop() { Thread::stop(); }
