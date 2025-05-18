#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"

#include "display.h"

class QtDisplay: public Display {
public:
    explicit QtDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue);

    void run() override;
    void stop() override;
};
