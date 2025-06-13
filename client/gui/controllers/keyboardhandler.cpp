#include "keyboardhandler.h"

#include <iostream>

#include <SDL2/SDL.h>

KeyboardHandler::KeyboardHandler(Queue<Message>& output_queue, shopDisplay& shopRef):
        output_queue(output_queue), shopRef(shopRef) {}


void KeyboardHandler::handleEvent(const SDL_Event& event /*, bool& shop*/) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                std::cout << "KEY_PRESS_SPACE" << std::endl;
                shopRef.updateShopState(false);
                break;
            case SDLK_b:
                std::cout << "KEY_PRESS_B" << std::endl;
                output_queue.push(Message(GetShopPricesCommand()));
                shopRef.updateShopState(true);
                break;
            case SDLK_m:
                // Toggle mute functionality //TODO_ADD SERVER
                break;
        }
    }
    update_direction();
}

void KeyboardHandler::update_direction() {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    Vector2D direction(0, 0);

    if (keystate[SDL_SCANCODE_W]) {
        direction.set_y(-1);
    } else if (keystate[SDL_SCANCODE_S]) {
        direction.set_y(1);
    }

    if (keystate[SDL_SCANCODE_A]) {
        direction.set_x(-1);
    } else if (keystate[SDL_SCANCODE_D]) {
        direction.set_x(1);
    }

    static Vector2D last_direction(0, 0);

    if (direction != last_direction) {
        if (direction != Vector2D(0, 0)) {
            output_queue.push(Message(MoveCommand(direction)));
        } else {
            output_queue.push(Message(StopPlayerCommand()));
        }
        last_direction = direction;
    }
}
