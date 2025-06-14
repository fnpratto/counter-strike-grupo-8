#include "keyboardhandler.h"

#include <iostream>

#include <SDL2/SDL.h>


KeyboardHandler::KeyboardHandler(Queue<Message>& output_queue, shopDisplay& shopRef,
                                 ScoreDisplay& score_displayRef):
        output_queue(output_queue), shopRef(shopRef), score_displayRef(score_displayRef) {
    // Constructor implementation can be empty or contain initialization logic if needed
}

void KeyboardHandler::handleEvent(const SDL_Event& event) {
    if (event.type != SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                shopRef.updateShopState(false);
                break;
            case SDLK_b:
                output_queue.push(Message(GetShopPricesCommand()));
                break;
            case SDLK_m:
                // Toggle mute functionality //TODO_ADD SERVER
                break;
            case SDLK_TAB:
                if (!score_displayRef.isActive()) {
                    output_queue.push(Message(GetScoreboardCommand()));
                } else {
                    score_displayRef.updateState();
                }
                break;
            case SDLK_r:
                output_queue.push(Message(ReloadCommand()));
                break;
        }
    }
    update_direction();
}

void KeyboardHandler::update_direction() {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    int dx = 0;
    int dy = 0;

    if (keystate[SDL_SCANCODE_W]) {
        dy = -1;
    }
    if (keystate[SDL_SCANCODE_S]) {
        dy = 1;
    }
    if (keystate[SDL_SCANCODE_A]) {
        dx = -1;
    }
    if (keystate[SDL_SCANCODE_D]) {
        dx = 1;
    }

    if (dx != 0 || dy != 0) {
        output_queue.push(Message(MoveCommand(Vector2D(dx, dy))));
        return;
    } else {
        output_queue.push(Message(StopPlayerCommand()));
    }
}
