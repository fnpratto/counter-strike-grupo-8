#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"

#include "display.h"

class TextDisplay: public Display {
    std::string line;

public:
    explicit TextDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue);

    void run() override;

    void draw(const Message& message);

    void read_input();
    Message parse_line(const std::string& line);
    void handle_char(char c);
};
