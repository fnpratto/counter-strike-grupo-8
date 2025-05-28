#pragma once

#include "../common/message.h"
#include "gui/controllers/keyboardhandler.h"
#include "gui/controllers/mousehandler.h"
#include "gui/hud_component/hud_display.h"
#include "gui/map_view/map_view.h"
#include "gui/pre_game_view/list_teams.h"
#include "gui/shop_view/shop.h"

class SDLInput: public Thread {
private:
    std::atomic<bool>& quit_flag;
    Queue<Message>& queue;
    MouseHandler mouseHandler{queue};
    KeyboardHandler keyboardHandler{queue};


public:
    SDLInput(Queue<Message>& queue, std::atomic<bool>& quit_flag);

    void run() override;
    void stop() override {
        quit_flag = true;
        Thread::stop();
        std::cout << "Client input ::run done\n";
    }
};
