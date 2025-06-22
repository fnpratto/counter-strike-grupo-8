#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <SDL2/SDL.h>

#include "../hud_component/hud_display.h"
#include "../map_view/sdl_world.h"
#include "../score_view/score_display.h"
#include "../shop_view/shop.h"
#include "client/sound_manager.h"
#include "common/commands.h"
#include "common/message.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"

class KeyboardHandler {
public:
    explicit KeyboardHandler(Queue<Message>& output_queue, shopDisplay& shopRef,
                             ScoreDisplay& score_displayRef, SoundManager& sound_manager,
                             SdlHud& hudDisplayRef);
    void handleEvent(const SDL_Event& event);

private:
    Queue<Message>& output_queue;
    shopDisplay& shopRef;
    ScoreDisplay& score_displayRef;
    SoundManager& sound_manager;
    SdlHud& hudDisplayRef;
    void update_direction();
};

#endif  // KEYBOARDHANDLER_H
