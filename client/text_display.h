#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"

#include "display.h"

class TextDisplay: public Display {
public:
    explicit TextDisplay(Queue<Message>& input_queue): Display(input_queue) {}
    void draw(const Message& message) override;
};
