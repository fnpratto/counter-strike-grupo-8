#include "keyboardhandler.h"

#include <iostream>

#include <SDL2/SDL.h>

KeyboardHandler::KeyboardHandler(Queue<Message>& output_queue): output_queue(output_queue) {
    x = 300;
    y = 300;
    // Constructor implementation can be empty or contain initialization logic if needed
}


void KeyboardHandler::handleEvent(const SDL_Event& event /*, bool& shop*/) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                std::cout << "KEY_PRESS_SPACE" << std::endl;
                // shop = false;
                break;
            case SDLK_b:
                std::cout << "KEY_PRESS_B" << std::endl;
                // inputQueue.push(Message(Request Store);
                // shop = true;
                break;
            case SDLK_m:
                // Toggle mute functionality (implementation pending)
                break;
        }
    }
    update_direction();
}

void KeyboardHandler::update_direction() {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    int dx = 0;
    int dy = 0;

    if (keystate[SDL_SCANCODE_UP]) {
        y -= 5;
        // map_ref.update_character(x, y, DIR_UP);
        dy = -1;
    }
    if (keystate[SDL_SCANCODE_DOWN]) {
        y += 5;
        // map_ref.update_character(x, y, DIR_DOWN);
        dy = 1;
    }
    if (keystate[SDL_SCANCODE_LEFT]) {
        x -= 5;
        // map_ref.update_character(x, y, DIR_LEFT);
        dx = -1;
    }
    if (keystate[SDL_SCANCODE_RIGHT]) {
        x += 5;
        // map_ref.update_character(x, y, DIR_RIGHT);
        dx = 1;
    }

    if (dx != 0 || dy != 0) {
        output_queue.push(Message(MoveCommand(Vector2D(dx, dy))));
        std::cout << "KEY_PRESS_MOVE: dx=" << dx << ", dy=" << dy << std::endl;
    }
}
