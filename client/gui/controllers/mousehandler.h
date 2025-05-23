#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include <SDL2/SDL.h>

#include "../hud_component/hud_display.h"
#include "../pre_game_view/list_teams.h"
#include "../shop_view/shop.h"

class MouseHandler {
public:
    explicit MouseHandler(hudDisplay& hudDisplayRef, shopDisplay& shopDisplayRef,
                          listTeams& listTeamsRef):
            hudDisplayRef(hudDisplayRef),
            shopDisplayRef(shopDisplayRef),
            listTeamsRef(listTeamsRef) {}
    void handleEvent(const SDL_Event& event, bool shop, bool list_teams);


private:
    hudDisplay& hudDisplayRef;
    shopDisplay& shopDisplayRef;
    listTeams& listTeamsRef;
};

#endif  // MOUSEHANDLER_H
