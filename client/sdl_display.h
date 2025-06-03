#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "../client/requests.h"
#include "../common/message.h"
#include "../common/updates/game_update.h"
#include "../common/updates/player_update.h"
#include "common/message.h"
#include "gui/controllers/keyboardhandler.h"
#include "gui/controllers/mousehandler.h"
#include "gui/hud_component/hud_display.h"
#include "gui/map_view/map_view.h"
#include "gui/pre_game_view/list_teams.h"
#include "gui/shop_view/shop.h"
#include "gui/window_elements/sdl_window.h"
#include "states/game_state.h"
#include "states/gun_state.h"
#include "states/inventory_state.h"
#include "states/phase_state.h"
#include "states/player_state.h"
#include "states/state.h"
#include "states/utility_state.h"

#include "display.h"
#include "sdl_input.h"

class SDLDisplay: public Display {
public:
    explicit SDLDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue);

    void run() override;
    void stop() override;

private:
    std::atomic<bool> quit_flag;
    std::unique_ptr<SDLInput> input_handler;
    void update_game();
    void handle_msg(const Message& msg /*, const std::string& player_name*/);
    void apply_game_update(const GameUpdate& update);
    GameState state;
};
