#include "keyboardhandler.h"

#include <iostream>

#include <SDL2/SDL.h>


KeyboardHandler::KeyboardHandler(Queue<Message>& inputQueue): inputQueue(inputQueue) {
    // Constructor implementation can be empty or contain initialization logic if needed
}

void KeyboardHandler::handleEvent(const SDL_Event& event, bool& shop) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_DOWN:
                std::cout << "KEY_PRESS_DOWN" << std::endl;
                inputQueue.push(Message(MoveCommand(MoveDirection::Down, true)));
                break;
            case SDLK_UP:
                std::cout << "KEY_PRESS_UP" << std::endl;
                inputQueue.push(Message(MoveCommand(MoveDirection::Up, true)));
                break;
            case SDLK_LEFT:
                std::cout << "KEY_PRESS_LEFT" << std::endl;
                inputQueue.push(Message(MoveCommand(MoveDirection::Left, true)));
                break;
            case SDLK_RIGHT:
                std::cout << "KEY_PRESS_RIGHT" << std::endl;
                inputQueue.push(Message(MoveCommand(MoveDirection::Right, true)));
                break;
            case SDLK_SPACE:
                std::cout << "KEY_PRESS_SPACE" << std::endl;
                shop = false;
                break;
            case SDLK_b:
                std::cout << "KEY_PRESS_B" << std::endl;
                shop = true;
                break;
        }
    }
}
