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
    payload_t serialize_message(const Message& message) const override;

    template <typename T>
    payload_t serialize_msg(const T& value) const;

    Message deserialize_message(const MessageType& type, payload_t& payload) const override;

    /**
     * @brief Deserializes the payload into the type T.
     * @tparam T The type to deserialize into.
     * @return The deserialized object of type T.
     */
    template <typename T>
    T deserialize_msg(payload_t& payload) const;

    template <typename T>
    std::vector<T> deserialize_vector(payload_t& payload) const {
        uint16_t length = deserialize<uint16_t>(payload);

        std::vector<T> result;
        for (size_t i = 0; i < length; i++) {
            T item = deserialize<T>(payload);
            result.push_back(item);
        }

        return result;
    }
};
