#include "sdl_display.h"

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

#include <SDL.h>
#include <SDL_events.h>
#include <unistd.h>

#include "display.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

// display = std::make_unique<SDLDisplay>(display_queue, ingame_queue);

/*    sender = std::make_unique<ClientSender>(protocol, ingame_queue);
sender->start();

// Feed the receiver queue to the display
receiver = std::make_unique<ClientReceiver>(protocol, display_queue);

//*/

GameUpdate SDLDisplay::receive_initial_state() {
    Message msg;
    while (true) {
        msg = input_queue.pop();
        if (msg.get_type() == MessageType::GAME_UPDATE) {
            return msg.get_content<GameUpdate>();
        } else {
            std::cerr << "Received unexpected message type: " << static_cast<int>(msg.get_type())
                      << std::endl;
        }
    }
}

SDLDisplay::SDLDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue):
        Display(input_queue, output_queue),
        quit_flag(false),
        input_handler(std::make_unique<SDLInput>(output_queue, quit_flag)) {
    state = receive_initial_state();
}

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

    std::cout << "Initialized new GameState" << std::endl;


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
        // Map map(window);
        listTeams listTeams(window);

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

            /*update --> pull event from the queue*/  // TODO
            update_game();
            window.fill();
            if (clock > 20) {
                hudDisplay.update(clock);
                // map.render();
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


void SDLDisplay::update_game() {
    Message msg;
    if (input_queue.try_pop(msg)) {
        handle_msg(msg);
        std::cout << "Message handled in SDLDisplay" << std::endl;
    }
}


void SDLDisplay::handle_msg(const Message& msg) {
    const GameUpdate& update = msg.get_content<GameUpdate>();
    state = state.merged(update);
    apply_game_update();
    std::cout << "Applied GameUpdate" << std::endl;
}

void SDLDisplay::apply_game_update() {
    // avisar a quien sea
}

#pragma GCC diagnostic pop
