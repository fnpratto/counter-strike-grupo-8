#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"
#include "common/queue.h"
#include "common/thread.h"

// Synchronous display class for the lobby phase of the game.
class LobbyDisplay {
protected:
    Queue<Message>& input_queue;
    Queue<Message>& output_queue;

public:
    explicit LobbyDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue):
            input_queue(input_queue), output_queue(output_queue) {}

    virtual void run() = 0;
    virtual void stop() = 0;

    virtual ~LobbyDisplay() = default;
};
