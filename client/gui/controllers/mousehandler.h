#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include <SDL2/SDL.h>

#include "../common/message.h"
#include "../common/queue.h"
#include "../common/socket.h"
#include "../common/thread.h"
#include "../hud_component/hud_display.h"
#include "../pre_game_view/list_teams.h"
#include "../shop_view/shop.h"


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
