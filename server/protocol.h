#pragma once

#include <algorithm>
#include <atomic>
#include <iostream>
#include <map>
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
#include "common/updates/state_update.h"
#include "game/game.h"

#include "lobby_monitor.h"

class ServerProtocol: public BaseProtocol {
public:
    explicit ServerProtocol(std::shared_ptr<BaseSocket> skt): BaseProtocol(std::move(skt)) {}

private:
    payload_t serialize_message(const Message& message) const override;

    template <typename T>
    payload_t serialize_msg(const T& value) const;

    template <typename T>
    payload_t serialize_update(const T& value) const;

    template <typename K, typename V>
    payload_t serialize_map(const std::map<K, V>& map) const {
        payload_t result;

        payload_t length = serialize(static_cast<uint16_t>(map.size()));
        result.insert(result.begin(), length.begin(), length.end());
        for (const auto& [key, value]: map) {
            payload_t key_payload = serialize(key);
            payload_t value_payload;

            if constexpr (std::is_base_of_v<StateUpdate, V>) {
                value_payload = serialize_update(value);
            } else {
                value_payload = serialize(value);
            }

            result.insert(result.end(), key_payload.begin(), key_payload.end());
            result.insert(result.end(), value_payload.begin(), value_payload.end());
        }

        return result;
    }

    template <typename T>
    payload_t serialize_optional(const std::optional<T>& v) const {
        payload_t payload;
        bool has_value = v.has_value();
        payload_t has_value_payload = serialize(has_value);
        payload.insert(payload.end(), has_value_payload.begin(), has_value_payload.end());

        if (has_value) {
            payload_t value_payload;

            if constexpr (std::is_base_of_v<StateUpdate, T>) {
                value_payload = serialize_update(v.value());
            } else {
                value_payload = serialize(v.value());
            }

            payload.insert(payload.end(), value_payload.begin(), value_payload.end());
        }
        return payload;
    }

    template <typename T>
    payload_t serialize_vector(const std::vector<T>& vector) const {
        payload_t payload;

        payload_t length = serialize(static_cast<uint16_t>(vector.size()));
        payload.insert(payload.end(), length.begin(), length.end());
        for (const auto& item: vector) {
            auto serialized_item = serialize(item);
            payload.insert(payload.end(), serialized_item.begin(), serialized_item.end());
        }

        return payload;
    }

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
