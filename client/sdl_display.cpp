#include "sdl_display.h"

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

#include <SDL.h>
#include <SDL_events.h>
#include <unistd.h>

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


SDLDisplay::SDLDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue):
        Display(input_queue, output_queue),
        quit_flag(false),
        input_handler(std::make_unique<SDLInput>(input_queue, quit_flag)) {}

void SDLDisplay::run() {
    char* basePath = SDL_GetBasePath();
    if (basePath) {

        if (chdir(basePath) != 0) {
            std::cerr << "chdir failed: " << strerror(errno) << std::endl;
        }
        SDL_free(basePath);
    } else {
        std::cerr << "SDL_GetBasePath failed: " << SDL_GetError() << std::endl;
    }

    input_handler->start();

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
    /*int SCREEN_WIDTH = displayMode.w;
    int SCREEN_HEIGHT = displayMode.h - 150;*/


    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

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

        MouseHandler mouseHandler(this->input_queue);
        KeyboardHandler keyboardHandler(this->input_queue);

        bool shop = false;
        // bool list_teams = true;
        int clock = 0;  // por ahora

        while (!quit_flag) {
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
            if (clock > 20) {
                hudDisplay.update(clock);
                map.render();
                if (shop) {
                    shopDisplay.render();
                }
                // list_teams = false;
            } else {

                listTeams.update(clock);
            }
            window.render();
        }
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void SDLDisplay::stop() {
    std::cout << "Stopping SDLDisplay...\n";
    if (input_handler) {
        std::cout << "Stopping input handler...\n";
        input_handler->stop();
        input_handler->join();
    }
    std::cout << "Client::run done\n";
    Thread::stop();
}
