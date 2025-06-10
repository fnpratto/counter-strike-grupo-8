#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <SDL2/SDL.h>

#include "../map_view/map_view.h"
#include "../shop_view/shop.h"
#include "common/commands.h"
#include "common/message.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"
class KeyboardHandler {
public:
    explicit KeyboardHandler(Queue<Message>& output_queue, shopDisplay& shopRef);
    void handleEvent(const SDL_Event& event);

private:
    Queue<Message>& output_queue;
    shopDisplay& shopRef;
    void update_direction();
    // bool isMuted = false;  // TODO sacar
    bool no_movement = false;  // TODO sacar
};

#endif  // KEYBOARDHANDLER_H
