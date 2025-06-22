#include "sdl_display.h"

#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <SDL.h>
#include <SDL_events.h>
#include <unistd.h>

#include "client/gui/map_view/sdl_world.h"
#include "common/models.h"
#include "common/utils/rate_controller.h"

#include "sdl_input.h"


SDLDisplay::SDLDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue,
                       const std::string& player_name):
        Display(input_queue, output_queue),
        player_name(player_name),
        state(get_initial_state()),
        quit_flag(false),
        input_handler(nullptr),
        score_display(nullptr),
        shop_display(nullptr),
        world(nullptr),
        end_round_display(nullptr),
        sound_manager(),
        current_phase(state.get_phase().get_phase()) {
    SCREEN_WIDTH = 1200;
    SCREEN_HEIGHT = 600;
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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError()
                  << std::endl;
        exit(1);
    }

    /*SCREEN_WIDTH = displayMode.w;
    SCREEN_HEIGHT = displayMode.h - 150;*/
    SCREEN_WIDTH = 1200;
    SCREEN_HEIGHT = 600;
}

void SDLDisplay::run() {
    setup();
    load_audio();
    SdlWindow window(SCREEN_WIDTH, SCREEN_HEIGHT);
    SdlHud hud_display(window, state, player_name);
    shop_display = std::make_unique<shopDisplay>(window, state);
    Map map = get_map();
    world = std::make_unique<SdlWorld>(window, std::move(map), state, player_name);
    listTeams list_teams(window, state, player_name);
    skinSelect list_skins(window, state, player_name);
    std::map<std::string, ScoreboardEntry> scoreboard;
    score_display = std::make_unique<ScoreDisplay>(window, scoreboard, state);
    input_handler = std::make_unique<SDLInput>(
            quit_flag,
            MouseHandler(output_queue, SCREEN_WIDTH, SCREEN_HEIGHT, list_teams, *shop_display,
                         hud_display, list_skins),
            KeyboardHandler(output_queue, *shop_display, *score_display, sound_manager,
                            hud_display));
    end_round_display = std::make_unique<EndRoundDisplay>(window, state);
    input_handler->start();

    update_state();

    RateController rate_controller(60);
    rate_controller.set_debug_mode(true);
    sound_manager.play_music("menu", -1);
    rate_controller.run_at_rate([&]() {
        update_music();
        update_state();
        window.fill();
        if (state.get_phase().get_phase() == PhaseType::WarmUp) {
            if (list_teams.isActive()) {
                list_teams.render();
            } else if (list_skins.isActive()) {
                list_skins.render();
            } else {
                world->render();
                hud_display.render();
            }
        } else if (state.get_phase().get_phase() == PhaseType::Buying) {
            world->render();
            hud_display.render();
            shop_display->render();
        } else if (state.get_phase().get_phase() == PhaseType::InRound) {
            world->render();
            hud_display.render();
        } else if (state.get_phase().get_phase() == PhaseType::RoundEnd) {
            world->render();
            hud_display.render();
            end_round_display->render();
        }

        if (score_display->isActive()) {
            score_display->render();
        }
        window.render();
        return !quit_flag;
    });
}


void SDLDisplay::load_audio() {
    sound_manager.load_music("menu", std::string(GameConfig::Paths::MENU_MUSIC_PATH).c_str());
    sound_manager.load_music("background", std::string(GameConfig::Paths::GAME_MUSIC_PATH).c_str());
    sound_manager.set_volume(0.8f);
    sound_manager.load_sound("ct_win",
                             std::string(GameConfig::Paths::CT_WIN_SOUND_PATH).c_str());  // TODO
    sound_manager.load_sound("tt_win",
                             std::string(GameConfig::Paths::TT_WIN_SOUND_PATH).c_str());  // TODO
    sound_manager.load_sound("item_pick",
                             std::string(GameConfig::Paths::ITEM_PICK_SOUND_PATH).c_str());
    sound_manager.load_sound("recharge",
                             std::string(GameConfig::Paths::RECHARGE_SOUND_PATH).c_str());
    sound_manager.load_sound("hit",
                             std::string(GameConfig::Paths::HIT_SOUND_PATH).c_str());  // TODO
    sound_manager.load_sound(
            "bomb_planted",
            std::string(GameConfig::Paths::BOMB_PLANTED_SOUND_PATH).c_str());  // TODO
    sound_manager.load_sound(
            "bomb_defused",
            std::string(GameConfig::Paths::BOMB_DEFUSED_SOUND_PATH).c_str());  // TODO

    sound_manager.load_sound("switch_teams",
                             std::string(GameConfig::Paths::SWITCH_TEAMS_SOUND_PATH).c_str());
    sound_manager.load_sound("error", std::string(GameConfig::Paths::ERROR_SOUND_PATH).c_str());

    sound_manager.load_sound(
            "bomb_exploded",
            std::string(GameConfig::Paths::BOMB_EXPLODED_SOUND_PATH).c_str());  // TODO
}

void SDLDisplay::stop() {
    std::cout << "Stopping SDLDisplay...\n";
    sound_manager.stop_music();
    Mix_CloseAudio();
    if (input_handler) {
        std::cout << "Stopping input handler...\n";
        input_handler->stop();
        input_handler->join();
    }
    std::cout << "Client::run done\n";
    Thread::stop();
}

void SDLDisplay::update_music() {
    PhaseType new_phase = state.get_phase().get_phase();
    if (new_phase != current_phase) {
        Mix_HaltMusic();
        switch (new_phase) {
            case PhaseType::WarmUp:
            case PhaseType::GameEnd:
            case PhaseType::RoundEnd:
                sound_manager.play_music("menu", -1);
                break;
            case PhaseType::Buying:
            case PhaseType::InRound:
            case PhaseType::BombPlanted:
                sound_manager.play_music("background", -1);
                break;

            default:
                std::cerr << "SDLDisplay::update_music: Unhandled phase type: "
                          << static_cast<int>(new_phase) << std::endl;
                break;
        }

        current_phase = new_phase;
    }
}

GameUpdate SDLDisplay::get_initial_state() {
    Message msg;
    while (true) {
        msg = input_queue.pop();
        if (msg.get_type() == MessageType::GAME_UPDATE) {
            return msg.get_content<GameUpdate>();
        } else {
            std::cerr << "SDLDisplay::get_initial_state: Received unexpected message type: "
                      << msg.get_type() << std::endl;
        }
    }
}

Map SDLDisplay::get_map() {
    Message msg;
    while (true) {
        msg = input_queue.pop();
        if (msg.get_type() == MessageType::MAP_RESP) {
            return msg.get_content<Map>();
        } else {
            throw std::runtime_error("SDLDisplay::get_map: Expected MAP_RESP, but received: " +
                                     std::to_string(static_cast<int>(msg.get_type())));
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
        switch (msg.get_type()) {
            case MessageType::GAME_UPDATE: {
                const GameUpdate& update = msg.get_content<GameUpdate>();
                state = state.merged(update);
                break;
            }
            case MessageType::SHOP_PRICES_RESP: {
                const ShopPricesResponse& response = msg.get_content<ShopPricesResponse>();
                shop_display->updateShopState(true);
                shop_display->updatePrices(response);
                std::cout << "Updated shop prices" << std::endl;
                break;
            }
            case MessageType::SCOREBOARD_RESP: {
                std::cout << "Received ScoreboardResponse" << std::endl;
                auto scoreboard = msg.get_content<ScoreboardResponse>().get_scoreboard();
                score_display->updateScoreboard(scoreboard);
                score_display->updateState();
                break;
            }
            case MessageType::HIT_RESP: {
                std::cout << "Received Hit response" << std::endl;
                auto hit = msg.get_content<HitResponse>();
                world->handleHit(hit.get_origin(), hit.get_hit_pos(), hit.get_hit_dir(),
                                 hit.is_hit());
                break;
            }
            case MessageType::ROUND_END_RESP: {
                std::cout << "Received RoundEndResponse" << std::endl;
                auto round_end_resp = msg.get_content<RoundEndResponse>();
                Team winner = round_end_resp.get_winning_team();
                if (winner == Team::TT) {
                    sound_manager.play("tt_win");
                } else if (winner == Team::CT) {
                    sound_manager.play("ct_win");
                }
                end_round_display->update_winner_team(winner);
                break;
            }
            case MessageType::SWAP_TEAMS_RESP: {
                std::cout << "Received SkinSelectResponse" << std::endl;
                sound_manager.play("switch_teams");

                break;
            }
            case MessageType::ERROR_RESP: {
                sound_manager.play("error");
                break;
            }
            default: {
                std::cerr << "SDLDisplay::update_state: Received unexpected message type: "
                          << msg.get_type() << std::endl;
                break;
            }
        }
    }
}
