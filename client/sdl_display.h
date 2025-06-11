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
#include "gui/hud_component/hud_display.h"
#include "gui/pre_game_view/list_skins.h"
#include "gui/pre_game_view/list_teams.h"
#include "gui/shop_view/shop.h"
#include "gui/window_elements/sdl_window.h"

#include "display.h"
#include "sdl_input.h"

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
    void setup();

    GameUpdate get_initial_state();
    void update_state();
};
