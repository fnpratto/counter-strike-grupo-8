#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include <algorithm>

#include <SDL2/SDL.h>

#include "client/gui/hud_component/hud_display.h"
#include "client/gui/pre_game_view/list_skins.h"
#include "client/gui/pre_game_view/list_teams.h"
#include "client/gui/shop_view/shop.h"
#include "common/message.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"

class MouseHandler {
    Queue<Message>& output_queue;
    int screen_width;
    int screen_height;
    shopDisplay& shopDisplayRef;
    listTeams& listTeamsRef;
    hudDisplay& hudDisplayRef;
    skinSelect& skinSelectRef;

public:
    explicit MouseHandler(Queue<Message>& output_queue, int screen_width, int screen_height,
                          listTeams& listRef, shopDisplay& shopRef, hudDisplay& hudRef,
                          skinSelect& skinRef):
            output_queue(output_queue),
            screen_width(screen_width),
            screen_height(screen_height),
            shopDisplayRef(shopRef),
            listTeamsRef(listRef),
            hudDisplayRef(hudRef),
            skinSelectRef(skinRef) {}
    void handleEvent(const SDL_Event& event);

private:
    void sendAimCommand(Vector2D direction);
};

#endif  // MOUSEHANDLER_H
