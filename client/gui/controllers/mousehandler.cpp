#include "mousehandler.h"

#include <iostream>

#include <SDL2/SDL.h>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

void MouseHandler::sendNormalizedCoordinates(int x, int y) {
    int norm_x = x / SCREEN_WIDTH;
    int norm_y = y / SCREEN_HEIGHT;

    // Optional: Clamp values between 0 and 1 just in case
    int norm_x_int =
            static_cast<int>(std::max(0.0, std::min(1.0, static_cast<double>(norm_x))) * 100);
    int norm_y_int =
            static_cast<int>(std::max(0.0, std::min(1.0, static_cast<double>(norm_y))) * 100);
    output_queue.push(Message(AimCommand(Vector2D(norm_x_int, norm_y_int))));  // TODO_ADD SERVER
}

void MouseHandler::handleEvent(const SDL_Event& event) {
    int x, y;
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        SDL_GetMouseState(&x, &y);
        std::optional<Message> maybe_message;
        std::optional<Team> team_choosen;
        std::optional<CharacterType> id_skin;
        std::optional<bool> pre_game_finished;

        switch (event.button.button) {
            case SDL_BUTTON_LEFT:
                std::cout << "MOUSE_PRESS_LEFT" << std::endl;

                maybe_message = shopDisplayRef.getPurchaseCommand(x, y);
                if (maybe_message.has_value()) {
                    output_queue.push(maybe_message.value());
                    std::cout << "Sent shop-related command." << std::endl;
                    return;
                }
                if (listTeamsRef.isActive()) {
                    team_choosen = listTeamsRef.updatePointerPosition(x, y);
                    if (team_choosen.has_value()) {
                        output_queue.push(Message(SelectTeamCommand(team_choosen.value())));
                        std::cout << "Selected team" << std::endl;
                        return;
                    }
                    return;
                }
                if (skinSelectRef.isActive()) {
                    id_skin = skinSelectRef.updatePointerPosition(x, y);
                    if (id_skin.has_value()) {
                        output_queue.push(Message(SelectCharacterCommand(id_skin.value())));
                        return;
                    }
                    pre_game_finished = skinSelectRef.updateFinishPreGame(x, y);
                    if (pre_game_finished) {
                        output_queue.push(Message(SetReadyCommand()));
                        return;
                    }
                    return;
                }
                output_queue.push(Message(AttackCommand()));
                break;
            case SDL_BUTTON_RIGHT:
                std::cout << "MOUSE_PRESS_RIGHT" << std::endl;
                break;
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        SDL_GetMouseState(&x, &y);
        sendNormalizedCoordinates(x, y);
        hudDisplayRef.updatePointerPosition(x, y);  // TODO_ADD SERVER
    }
}
