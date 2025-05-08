#include "text_display.h"

#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"

#include "text_display.h"

void TextDisplay::draw(const Message& message) {
    // Delete later
    if (message.get_type() == MessageType::NONE)
        return;

    throw std::runtime_error("Invalid message type for TextDisplay");
    // TODO: Implement the display logic for different message types
    // if (message.get_type() == MessageType::RAW_TEXT) {
    //     std::cout << message.get_content<std::string>() << std::flush;
    // } else {
    // }
}
