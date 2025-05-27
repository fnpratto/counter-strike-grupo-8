#include "mousehandler.h"

#include <iostream>

#include <SDL2/SDL.h>

// void getNormalizedCoordinates(/*int& x, int& y*/) {
//  Normalize the coordinates to the range [0, 1]
//  x = static_cast<int>(x / static_cast<float>(SCREEN_WIDTH));
//  y = static_cast<int>(y / static_cast<float>(SCREEN_HEIGHT));
//}

void MouseHandler::handleEvent(const SDL_Event& event, bool shop, bool list_teams) {
    int x, y;
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        switch (event.button.button) {
            case SDL_BUTTON_LEFT:
                std::cout << "MOUSE_PRESS_LEFT" << std::endl;
                if (shop) {
                    SDL_GetMouseState(&x, &y);
                    // normalizar
                    // inputQueue.push(Message(BuyWeaponCommand??));  F(BuyWeaponCommand,
                    // BUY_WEAPON_CMD)
                    shopDisplayRef.updatePointerPosition(x, y);
                }
                if (list_teams) {
                    SDL_GetMouseState(&x, &y);
                    listTeamsRef.updatePointerPosition(x, y);
                    // inputQueue.push(Message(SelectTeamCommand);
                }
                // inputQueue.push(Message(ShootCommand));
                break;
            case SDL_BUTTON_RIGHT:
                std::cout << "MOUSE_PRESS_RIGHT" << std::endl;
                break;
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        SDL_GetMouseState(&x, &y);
        // F(AimCommand, AIM_CMD)
        hudDisplayRef.updatePointerPosition(x, y);
    }
}
