#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include <algorithm>

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
    explicit MouseHandler(Queue<Message>& output_queue, listTeams& listRef, shopDisplay& shopRef,
                          hudDisplay& hudRef):

            output_queue(output_queue),
            shopDisplayRef(shopRef),
            listTeamsRef(listRef),
            hudDisplayRef(hudRef) {}
    void handleEvent(const SDL_Event& event, bool shop, bool list_teams);


private:
    Queue<Message>& output_queue;

    void sendNormalizedCoordinates(int x, int y);
    shopDisplay shopDisplayRef;
    listTeams listTeamsRef;
    hudDisplay hudDisplayRef;
};

#endif  // MOUSEHANDLER_H
