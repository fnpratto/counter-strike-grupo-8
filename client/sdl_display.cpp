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
        input_handler(std::make_unique<SDLInput>(output_queue, quit_flag)) {}

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


void SDLDisplay::update_game() {
    // state.clear_updates(); why this?
    Message msg;
    if (input_queue.try_pop(msg)) {
        handle_msg(msg);
        std::cout << "Message handled in SDLDisplay" << std::endl;
    }
}

void SDLDisplay::handle_msg(const Message& msg /*, const std::string& player_name*/) {
    MessageType msg_type = msg.get_type();
    if (msg_type == MessageType::SELECT_TEAM_CMD) {
        // Team team = msg.get_content<SelectTeamCommand>().get_team();
        std::cout << "Received SELECT_TEAM_CMD message with team: ";
        // handle_select_team_msg(player_name, team);
    } else if (msg_type == MessageType::START_GAME_CMD) {
        std::cout << "Received START_GAME_CMD message" << std::endl;
        // handle_start_game_msg(player_name);
    } else if (msg_type == MessageType::BUY_GUN_CMD) {
        // GunType gun = msg.get_content<BuyGunCommand>().get_gun();
        std::cout << "Received BUY_GUN_CMD message with gun: ";
        // handle_buy_gun_msg(player_name, gun);
    } else if (msg_type == MessageType::BUY_AMMO_CMD) {
        // GunType gun = msg.get_content<BuyAmmoCommand>().get_gun();
        std::cout << "Received BUY_AMMO_CMD message with gun: ";
        // handle_buy_ammo_msg(player_name, gun);
    } else if (msg_type == MessageType::MOVE_CMD) {
        // Vector2D direction = msg.get_content<MoveCommand>().get_direction();
        std::cout << "Received MOVE_CMD message with direction: ("
                     ")"
                  << std::endl;
        // handle_move_msg(player_name, direction);
    } else if (msg_type == MessageType::STOP_PLAYER_CMD) {
        std::cout << "Received STOP_PLAYER_CMD message" << std::endl;
        // handle_stop_player_msg(player_name);
    } else if (msg_type == MessageType::AIM_CMD) {
        float x = msg.get_content<AimCommand>().get_x();
        float y = msg.get_content<AimCommand>().get_y();
        std::cout << "Received AIM_CMD message with coordinates: (" << x << ", " << y << ")"
                  << std::endl;
        // handle_aim_msg(player_name, x, y);
        // } else if (msg_type == MessageType::SHOOT_CMD) {
        //     int x = msg.get_content<ShootCommand>().get_x();
        //     int y = msg.get_content<ShootCommand>().get_y();
        //     handle_shoot_msg(player_name, x, y);
    } else if (msg_type == MessageType::SWITCH_WEAPON_CMD) {
        // WeaponSlot slot = msg.get_content<SwitchWeaponCommand>().get_slot();
        std::cout << "Received SWITCH_WEAPON_CMD message with slot: " << std::endl;
        // handle_switch_weapon_msg(player_name, slot);
    } else if (msg_type == MessageType::RELOAD_CMD) {
        std::cout << "Received RELOAD_CMD message" << std::endl;
        // handle_reload_msg(player_name);
    }

    std::cout << "Nada" << std::endl;
}


#pragma GCC diagnostic pop
