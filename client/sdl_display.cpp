#include "sdl_display.h"

#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include <SDL.h>
#include <SDL_events.h>
#include <unistd.h>

#include "sdl_input.h"


SDLDisplay::SDLDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue,
                       const std::string& player_name):
        Display(input_queue, output_queue),
        player_name(player_name),
        state(get_initial_state()),
        quit_flag(false),
        input_handler(nullptr) {
    std::cout << "SDLDisplay initialized with player: " << player_name << std::endl;
}


void SDLDisplay::setup() {
    char* basePath = SDL_GetBasePath();
    if (basePath) {
        if (chdir(basePath) != 0) {
            std::cerr << "chdir failed: " << strerror(errno) << std::endl;
        }
        SDL_free(basePath);
    } else {
        std::cerr << "SDL_GetBasePath failed: " << SDL_GetError() << std::endl;
    }

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
}

void SDLDisplay::run() {
    setup();
    SdlWindow window(SCREEN_WIDTH, SCREEN_HEIGHT);
    hudDisplay hud_display(window, state, player_name);
    shopDisplay shop_display(window);
    Map map(window, player_name, state);
    listTeams list_teams(window);

    input_handler = std::make_unique<SDLInput>(output_queue, quit_flag, list_teams, shop_display,
                                               hud_display);

    input_handler->start();

    update_state();

    framerated([&]() {
        // Update game state and display
        update_state();
        window.fill();
        map.render();
        hud_display.render();
        shop_display.render();
        window.render();
        return !quit_flag;
    });
}


void SDLDisplay::framerated(std::function<bool()> draw) {
    const int target_fps = 60;
    const std::chrono::milliseconds frame_duration(1000 / target_fps);

    auto next_frame_time = std::chrono::steady_clock::now();


    while (true) {
        auto now = std::chrono::steady_clock::now();

        if (now >= next_frame_time) {
            // We're at or past the time for the next frame
            // Call draw(), if it returns false, exit loop
            if (!draw())
                break;

            // Schedule next frame (even if we’re late)
            next_frame_time += frame_duration;

            // If we are significantly behind (e.g. > 5 frames), skip ahead
            auto max_lag = 5 * frame_duration;
            if (now - next_frame_time > max_lag) {
                next_frame_time = now;
            }
        } else {
            // We're ahead of schedule: sleep to limit framerate
            std::this_thread::sleep_until(next_frame_time);
        }
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

GameUpdate SDLDisplay::get_initial_state() {
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

void SDLDisplay::update_state() {
    Message msg;
    if (input_queue.try_pop(msg)) {
        const GameUpdate& update = msg.get_content<GameUpdate>();
        state = state.merged(update);
        std::cout << "Applied GameUpdate" << std::endl;
    }
}
