#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <SDL2/SDL.h>

#include "../map_view/map_view.h"
#include "common/commands.h"
#include "common/message.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"
class KeyboardHandler {
public:
    explicit KeyboardHandler(Queue<Message>& inputQueue, Map& map_ref);
    void handleEvent(const SDL_Event& event, bool& shop, bool& isMuted);

private:
    Queue<Message>& inputQueue;
    void update_direction();
    Map& map_ref;
    int x;  // TODO sacar
    int y;  // TODO sacar
};

#endif  // KEYBOARDHANDLER_H
