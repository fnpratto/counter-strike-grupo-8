#include "sdl_display.h"

#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_events.h>
#include <unistd.h>

#include "client/gui/map_view/sdl_world.h"
#include "common/utils/rate_controller.h"

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
    SdlWorld world(window, state, player_name);
    listTeams list_teams(window);

    input_handler = std::make_unique<SDLInput>(output_queue, quit_flag, list_teams, shop_display,
                                               hud_display);

    input_handler->start();

    update_state();

    RateController rate_controller(60);  // 60 FPS
    rate_controller.run_at_rate([&]() {
        // Update game state and display
        update_state();
        window.fill();
        world.render();
        hud_display.render();
        window.render();
        return !quit_flag;
    });
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
    std::vector<Message> msgs;
    for (int i = 0; i < 10; ++i) {
        Message msg;
        if (!input_queue.try_pop(msg))
            break;  // No more messages to process
        msgs.push_back(msg);
    }

    for (const auto& msg: msgs) {
        if (msg.get_type() == MessageType::GAME_UPDATE) {
            const GameUpdate& update = msg.get_content<GameUpdate>();
            state = state.merged(update);
            std::cout << "Applied GameUpdate" << std::endl;
        }
    }
}
