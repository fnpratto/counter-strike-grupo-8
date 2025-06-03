#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"
#include "common/updates/game_update.h"

#include "display.h"

class TextDisplay: public Display {
    std::string line;

    GameUpdate state;

public:
    explicit TextDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue);

    void run() override;

    void draw(const Message& message);

    void read_input();
    Message parse_line(const std::string& line);
    void handle_char(char c);

    template <typename T>
    Message build_message(std::istringstream& iss);
};
