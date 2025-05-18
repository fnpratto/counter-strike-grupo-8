#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"
#include "common/queue.h"
#include "common/thread.h"

class Display: public Thread {
protected:
    Queue<Message>& input_queue;
    Queue<Message>& output_queue;

public:
    explicit Display(Queue<Message>& input_queue, Queue<Message>& output_queue):
            input_queue(input_queue), output_queue(output_queue) {}
};
