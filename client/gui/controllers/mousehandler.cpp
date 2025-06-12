#include "mousehandler.h"

#include <iostream>

#include <SDL2/SDL.h>

#include "common/utils/rate_controller.h"

// TODO refactor this to send the event to clickable components
void MouseHandler::handleEvent(const SDL_Event& event) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        std::optional<Message> maybe_message;
        std::optional<Team> team_choosen;
        std::optional<CharacterType> id_skin;
        std::optional<bool> pre_game_finished;

        if (event.button.button != SDL_BUTTON_LEFT)
            return;

        maybe_message = shopDisplayRef.updatePointerPosition(x, y);
        if (maybe_message.has_value()) {
            output_queue.push(maybe_message.value());
            return;
        }
        if (listTeamsRef.isActive()) {
            team_choosen = listTeamsRef.updatePointerPosition(x, y);
            if (team_choosen.has_value()) {
                output_queue.push(Message(SelectTeamCommand(team_choosen.value())));
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
                output_queue.push(Message(StartGameCommand()));
                return;
            }
            return;
        }

        return;
        // output_queue.push(Message(ShootCommand()));  // TODO_ADD SERVER
    } else if (event.type == SDL_MOUSEMOTION) {
        sendAimCommand(Vector2D(x, y) - Vector2D(screen_width / 2, screen_height / 2));
        hudDisplayRef.updatePointerPosition(x, y);  // TODO_ADD SERVER
    }
}

void MouseHandler::sendAimCommand(Vector2D direction) {
    static RateController rate_controller(5);
    if (!rate_controller.should_run())
        return;

    direction = direction.normalized(100);
    output_queue.emplace(AimCommand(direction));  // TODO_ADD SERVER
}
