#include "mousehandler.h"

#include <iostream>

#include <SDL2/SDL.h>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

void MouseHandler::sendNormalizedCoordinates(int x, int y) {
    float norm_x = static_cast<float>(x) / SCREEN_WIDTH;
    float norm_y = static_cast<float>(y) / SCREEN_HEIGHT;

    // Optional: Clamp values between 0 and 1 just in case
    norm_x = std::max(0.0f, std::min(1.0f, norm_x));
    norm_y = std::max(0.0f, std::min(1.0f, norm_y));
    inputQueue.push(Message(AimCommand(norm_x, norm_y)));
}

void MouseHandler::handleEvent(const SDL_Event& event, bool shop, bool list_teams) {
    int x, y;
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        switch (event.button.button) {
            case SDL_BUTTON_LEFT:
                std::cout << "MOUSE_PRESS_LEFT" << std::endl;
                if (shop) {
                    SDL_GetMouseState(&x, &y);
                    // normalizar
                    // inputQueue.push(Message(BuyWeaponCommand??));
                    // F(BuyWeaponCommand, BUY_WEAPON_CMD)
                    shopDisplayRef.updatePointerPosition(x, y);
                }
                if (list_teams) {
                    SDL_GetMouseState(&x, &y);
                    std::optional<Team> team_choosen = listTeamsRef.updatePointerPosition(x, y);
                    if (team_choosen.has_value()) {
                        inputQueue.push(Message(SelectTeamCommand(team_choosen.value())));
                        std::cout << "Selected team" << std::endl;
                    }
                }
                // inputQueue.push(Message(ShootCommand));
                break;
            case SDL_BUTTON_RIGHT:
                std::cout << "MOUSE_PRESS_RIGHT" << std::endl;
                break;
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        SDL_GetMouseState(&x, &y);
        sendNormalizedCoordinates(x, y);
        hudDisplayRef.updatePointerPosition(x, y);
    }
}
