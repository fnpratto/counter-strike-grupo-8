#include "mousehandler.h"

#include <iostream>

#include <SDL2/SDL.h>

#include "common/utils/rate_controller.h"

// TODO refactor this to send the event to clickable components
void MouseHandler::handleEvent(const SDL_Event& event) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button != SDL_BUTTON_LEFT)
            return;

        if (shopDisplayRef.isActive()) {
            std::optional<Message> maybe_message = shopDisplayRef.getPurchaseCommand(x, y);
            if (maybe_message.has_value()) {
                output_queue.push(maybe_message.value());
                return;
            }
        }
        if (listTeamsRef.isActive()) {
            std::optional<Team> chosen_team = listTeamsRef.updatePointerPosition(x, y);
            if (chosen_team.has_value()) {
                output_queue.push(Message(SelectTeamCommand(chosen_team.value())));
                return;
            }
            return;
        }
        if (skinSelectRef.isActive()) {
            std::optional<CharacterType> id_skin = skinSelectRef.updatePointerPosition(x, y);
            if (id_skin.has_value()) {
                output_queue.push(Message(SelectCharacterCommand(id_skin.value())));
                return;
            }
            if (skinSelectRef.updateFinishPreGame(x, y)) {
                return;
            }
        }

        if (hudDisplayRef.start_game_click(x, y)) {
            output_queue.push(Message(SetReadyCommand()));
            return;
        }
        output_queue.push(Message(AttackCommand()));
        std::cout << "MouseHandler: AttackCommand sent" << std::endl;
        return;
    } else if (event.type == SDL_MOUSEMOTION) {
        sendAimCommand(Vector2D(x, y) - Vector2D(screen_width / 2, screen_height / 2));
        hudDisplayRef.update_pointer_position(x, y);
    }
}

void MouseHandler::sendAimCommand(Vector2D direction) {
    static RateController rate_controller(60);
    if (!rate_controller.should_run())
        return;

    direction = direction.normalized(100);
    output_queue.emplace(AimCommand(direction));
}
