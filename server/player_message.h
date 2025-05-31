#pragma once

#include <string>

#include "common/message.h"

class PlayerMessage {
    std::string player_name;
    Message message;

public:
    PlayerMessage(): player_name(""), message(Message()) {}
    PlayerMessage(const std::string& player_name, const Message& message):
            player_name(player_name), message(message) {}
    const std::string& get_player_name() const { return player_name; }
    const Message& get_message() const { return message; }
};
