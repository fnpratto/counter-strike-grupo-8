#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "../client/requests.h"
#include "../common/message.h"
#include "../common/updates/game_update.h"
#include "../common/updates/player_update.h"
#include "common/message.h"
#include "gui/controllers/mousehandler.h"
#include "gui/hud_component/end_round_display.h"
#include "gui/hud_component/hud_display.h"
#include "gui/pre_game_view/list_skins.h"
#include "gui/pre_game_view/list_teams.h"
#include "gui/score_view/score_display.h"
#include "gui/shop_view/shop.h"
#include "gui/window_elements/sdl_music.h"
#include "gui/window_elements/sdl_window.h"

#include "display.h"
#include "game_config.h"
#include "sdl_input.h"
#include "sound_manager.h"

class SDLDisplay: public Display {
    std::string player_name;
    GameUpdate state;

    std::atomic<bool> quit_flag;


    std::unique_ptr<SDLInput> input_handler;

public:
    explicit SDLDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue,
                        const std::string& player_name);
    void run() override;
    void stop() override;

private:
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    std::unique_ptr<ScoreDisplay> score_display;
    std::unique_ptr<shopDisplay> shop_display;
    std::unique_ptr<SdlWorld> world;
    std::unique_ptr<EndRoundDisplay> end_round_display;
    std::unique_ptr<SdlHud> hud_display;
    SoundManager sound_manager;
    PhaseType current_phase;

    void setup();

    Map get_map();
    GameUpdate get_initial_state();
    void update_state();
    void update_music();
    void load_audio();
};
