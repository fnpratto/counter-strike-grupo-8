#include "keyboardhandler.h"

#include <iostream>

#include <SDL2/SDL.h>


KeyboardHandler::KeyboardHandler(Queue<Message>& output_queue, shopDisplay& shopRef,
                                 ScoreDisplay& score_displayRef, SoundManager& sound_managerRef,
                                 SdlHud& hudDisplayRef, SdlWorld& wordRef):
        output_queue(output_queue),
        shopRef(shopRef),
        score_displayRef(score_displayRef),
        sound_manager(sound_managerRef),
        hudDisplayRef(hudDisplayRef),
        worldRef(wordRef),
        active_b(false) {}

void KeyboardHandler::handleEvent(const SDL_Event& event) {
    if (event.type != SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                shopRef.updateShopState(false);
                active_b = false;
                break;
            case SDLK_b:
                output_queue.push(Message(GetShopPricesCommand()));
                active_b = false;
                break;
            case SDLK_m:
                sound_manager.toggle_mute();
                hudDisplayRef.update_mute_icon();
                active_b = false;
                break;
            case SDLK_TAB:
                if (!score_displayRef.isActive()) {
                    output_queue.push(Message(GetScoreboardCommand()));
                } else {
                    score_displayRef.updateState();
                }
                active_b = false;
                break;
            case SDLK_1:
                std::cout << "Switching to primary item" << std::endl;
                output_queue.push(Message(SwitchItemCommand(ItemSlot::Primary)));
                active_b = false;
                break;
            case SDLK_2:
                std::cout << "Switching to 2 item" << std::endl;
                output_queue.push(Message(SwitchItemCommand(ItemSlot::Secondary)));
                active_b = false;
                break;
            case SDLK_3:
                std::cout << "Switching to 3 item" << std::endl;
                output_queue.push(Message(SwitchItemCommand(ItemSlot::Melee)));
                active_b = false;
                break;
            case SDLK_4:
                std::cout << "Switching to 4 item" << std::endl;
                output_queue.push(Message(SwitchItemCommand(ItemSlot::Bomb)));
                active_b = false;
                break;
            case SDLK_r:
                output_queue.push(Message(ReloadCommand()));
                sound_manager.play("reload");
                active_b = false;
                break;
            case SDLK_g:
                sound_manager.play("item_pick");
                output_queue.push(Message(PickUpItemCommand()));
                active_b = false;
                break;
            case SDLK_e:
                std::optional<Message> maybe_message = worldRef.getStartBombMessage(sound_manager);
                if (maybe_message.has_value()) {
                    active_b = true;
                }
                break;
        }
    }
    update_direction();
    if (active_b) {
        std::optional<Message> maybe_message = worldRef.getStopBombMessage(sound_manager);
        if (maybe_message.has_value()) {
            active_b = false;
        }
    }
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
