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

#include "game.h"
#include "lobby_monitor.h"

class ServerProtocol: public BaseProtocol {
public:
    explicit ServerProtocol(Socket&& skt): BaseProtocol(std::move(skt)) {}

private:
    payload_t serialize_message(const Message& message) override;

    template <typename T>
    payload_t serialize(const T& value);

    Message deserialize_message(const MessageType& type, const payload_t& payload) override;

    /**
     * @brief Deserializes the payload into the type T.
     * @tparam T The type to deserialize into.
     * @return The deserialized object of type T.
     */
    template <typename T>
    T deserialize(const payload_t& payload);
};
