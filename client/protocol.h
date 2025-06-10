#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "common/errors.h"
#include "common/message.h"
#include "common/protocol.h"
#include "common/socket.h"
#include "common/updates/state_update.h"

/**
 * @class ClientProtocol
 * @brief Protocol for client-side message transmission and reception.
 */
class ClientProtocol: public BaseProtocol {
public:
    /**
     * @brief Constructs a ClientProtocol with a connected socket.
     * @param skt Shared pointer to a connected Socket.
     */
    explicit ClientProtocol(Socket&& skt): BaseProtocol(std::move(skt)) {}

    payload_t serialize_message(const Message& message) const override;

    template <typename T>
    payload_t serialize_msg(const T& value) const;

    Message deserialize_message(const MessageType& type, payload_t& payload) const override;

    template <typename T>
    T deserialize_msg(payload_t& payload) const;

    template <typename T>
    T deserialize_update(payload_t& payload) const;

    template <typename K, typename V>
    std::map<K, V> deserialize_map(payload_t& payload) const {
        uint16_t length = deserialize<uint16_t>(payload);
        std::map<K, V> result;

        for (size_t i = 0; i < length; i++) {
            K key = deserialize<K>(payload);
            V value;

            if constexpr (std::is_base_of_v<StateUpdate, V>) {
                value = deserialize_update<V>(payload);
            } else {
                value = deserialize<V>(payload);
            }

            result[key] = value;
        }

        return result;
    }

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
