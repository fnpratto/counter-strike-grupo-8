#pragma once

#include <utility>

#include "../common/message.h"
#include "gui/controllers/keyboardhandler.h"
#include "gui/controllers/mousehandler.h"
#include "gui/hud_component/hud_display.h"
#include "gui/pre_game_view/list_skins.h"
#include "gui/pre_game_view/list_teams.h"
#include "gui/shop_view/shop.h"

class SDLInput: public Thread {
private:
    std::atomic<bool>& quit_flag;
    MouseHandler mouseHandler;
    KeyboardHandler keyboardHandler;

public:
    SDLInput(std::atomic<bool>& quit_flag, MouseHandler mouse_handler,
             KeyboardHandler keyboard_handler):
            quit_flag(quit_flag),
            mouseHandler(std::move(mouse_handler)),
            keyboardHandler(std::move(keyboard_handler)) {}

    void run() override;
    void stop() override {
        quit_flag = true;
        Thread::stop();
        std::cout << "Client input ::run done\n";
    }
};
