#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"

#include "queue.h"
#include "thread.h"

class Display: public Thread {
    Queue<Message>& queue;

public:
    explicit Display(Queue<Message>& input_queue): queue(input_queue) {}
    virtual ~Display() = default;

    void run() override;

    virtual void draw(const Message& message) = 0;
};
