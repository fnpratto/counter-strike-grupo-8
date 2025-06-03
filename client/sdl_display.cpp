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
SDLDisplay::SDLDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue):
        Display(input_queue, output_queue),
        quit_flag(false),
        input_handler(std::make_unique<SDLInput>(output_queue, quit_flag)),
        state() {}

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
        /*if (state = nullptr) {
            state = receive_initial_state(msg);
            std::cout << "Initialized new GameState" << std::endl;
        }*/
        handle_msg(msg);
        std::cout << "Message handled in SDLDisplay" << std::endl;
    }
}


void SDLDisplay::handle_msg(const Message& msg /*, const std::string& player_name*/) {
    const GameUpdate& update = msg.get_content<GameUpdate>();
    apply_game_update(update);  // assuming 'state' is your GameState
    std::cout << "Applied GameUpdate" << std::endl;
}

void apply_game_update(GameState& state, const GameUpdate& update) {
    // Update number of rounds
    if (update.has_num_rounds_changed()) {
        // state.num_rounds = update.get_num_rounds();
    }

    // Update game phase
    if (update.has_phase_changed()) {
        const PhaseUpdate& phase_update = update.get_phase();

        if (phase_update.has_phase_changed()) {
            // state.phase = phase_update.get_phase();
        }
        if (phase_update.has_time_changed()) {
            // state.time = phase_update.get_time();
        }
    }

    // Update player states
    if (update.has_players_changed()) {
        for (const auto& [player_id, player_update]: update.get_players()) {
            // PlayerState& player = state.players[player_id];  // create if not exists

            if (player_update.has_team_changed()) {
                // player.team = player_update.get_team();
            }
            if (player_update.has_pos_changed()) {
                // player.pos = player_update.get_pos();
            }
            if (player_update.has_aim_direction_changed()) {
                // player.aim_direction = player_update.get_aim_direction();
            }
            if (player_update.has_velocity_changed()) {
                // player.velocity = player_update.get_velocity();
            }
            if (player_update.has_ready_changed()) {
                // player.ready = player_update.get_ready();
            }
            if (player_update.has_health_changed()) {
                // player.health = player_update.get_health();
            }
            if (player_update.has_current_weapon_changed()) {
                // player.current_weapon = player_update.get_current_weapon();
            }

            // Inventory update
            if (player_update.has_inventory_changed()) {
                const InventoryUpdate& inv = player_update.get_inventory();

                if (inv.has_money_changed()) {
                    // player.inventory.money = inv.get_money();
                }

                if (inv.has_guns_changed()) {
                    for (const auto& [slot, gun_update]: inv.get_guns()) {
                        // GunState& gun = player.inventory.guns[slot];

                        if (gun_update.has_gun_changed()) {
                            //  gun.gun = gun_update.get_gun();
                        }
                        if (gun_update.has_bullets_per_mag_changed()) {
                            // gun.bullets_per_mag = gun_update.get_bullets_per_mag();
                        }
                        if (gun_update.has_mag_ammo_changed()) {
                            // gun.mag_ammo = gun_update.get_mag_ammo();
                        }
                        if (gun_update.has_reserve_ammo_changed()) {
                            // gun.reserve_ammo = gun_update.get_reserve_ammo();
                        }
                    }
                }

                if (inv.has_utilities_changed()) {
                    for (const auto& [slot, util_update]: inv.get_utilities()) {
                        // UtilityState& util = player.inventory.utilities[slot];
                        //  Populate fields here as needed, e.g.:
                        //  if (util_update.has_X_changed()) { util.X = util_update.get_X(); }
                    }
                }
            }
        }
    }
}

#pragma GCC diagnostic pop
