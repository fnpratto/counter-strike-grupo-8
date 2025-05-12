#pragma once

#include <algorithm>
#include <atomic>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "common/message.h"
#include "common/protocol.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"

#include "lobby_monitor.h"

class ServerProtocol: public BaseProtocol {
public:
    explicit ServerProtocol(Socket&& skt): BaseProtocol(std::move(skt)) {}

    Message recv() override;
    payload_t serialize_message(const Message& message) override;

private:
    Message deserialize_create_game_cmd();
    Message deserialize_join_game_cmd();
    Message deserialize_list_games_cmd();
};
