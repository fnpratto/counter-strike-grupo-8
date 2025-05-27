#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include <SDL2/SDL.h>

#include "client/gui/hud_component/hud_display.h"
#include "client/gui/pre_game_view/list_teams.h"
#include "client/gui/shop_view/shop.h"
#include "common/message.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"


class MouseHandler {
public:
    explicit MouseHandler(hudDisplay& hudDisplayRef, shopDisplay& shopDisplayRef,
                          listTeams& listTeamsRef, Queue<Message>& inputQueue):
            hudDisplayRef(hudDisplayRef),
            shopDisplayRef(shopDisplayRef),
            listTeamsRef(listTeamsRef),
            inputQueue(inputQueue) {}
    void handleEvent(const SDL_Event& event, bool shop, bool list_teams);


private:
    hudDisplay& hudDisplayRef;
    shopDisplay& shopDisplayRef;
    listTeams& listTeamsRef;
    Queue<Message>& inputQueue;
};

#endif  // MOUSEHANDLER_H
