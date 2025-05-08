#ifndef TEXT_INPUT_H
#define TEXT_INPUT_H

#include <string>

#include "input.h"

class TextInput: public Input {
public:
    explicit TextInput(Queue<Message>& q): Input(q) {}

private:
    std::string line;

    void setup() override;
    void work() override;
    void handle_char(char c);
    Message parse_line(const std::string& line);
};

#endif  // TEXT_INPUT_H
