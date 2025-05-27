#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <SDL2/SDL.h>

#include "common/commands.h"
#include "common/message.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"


class KeyboardHandler {
public:
    explicit KeyboardHandler(Queue<Message>& inputQueue);
    void handleEvent(const SDL_Event& event, bool& shop);

private:
    Queue<Message>& inputQueue;
    void update_direction();
};

#endif  // KEYBOARDHANDLER_H
