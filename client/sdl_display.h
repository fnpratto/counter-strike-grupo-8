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

#include "display.h"
#include "sdl_input.h"

class SDLDisplay: public Display {
    GameUpdate state;
    std::string player_name = "Player1";

    std::atomic<bool> quit_flag;

    static constexpr int SCREEN_WIDTH = 800;
    static constexpr int SCREEN_HEIGHT = 600;

    std::unique_ptr<SDLInput> input_handler;

public:
    explicit SDLDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue);
    void run() override;
    void stop() override;

private:
    void setup();

    void update_display(hudDisplay& hud_display);
    void framerated(std::function<bool()> draw);

    GameUpdate get_initial_state();
    void update_state();
};
