#pragma once

#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "common/base_socket.h"
#include "common/game/world_item.h"
#include "common/models.h"
#include "common/socket.h"
#include "common/utils/container_type_traits.h"
#include "common/utils/vector_2d.h"

#include "message.h"

enum class CmdType : uint8_t {
    CMD_CREATE = 0x6E,
    CMD_JOIN = 0x6A,
    CMD_LIST = 0x6C,
    CMD_PLAY = 0x70
};

typedef std::vector<char> payload_t;

/**
 * @class BaseProtocol
 * @brief Abstract base class for message-based protocols using a BaseSocket.
 *
 * Manages a BaseSocket to send and receive Message objects over the network.
 */
class BaseProtocol {
protected:
    std::shared_ptr<BaseSocket> socket;

public:
    /**
     * @brief Constructs the protocol with a given socket.
     * @param skt Shared pointer to a BaseSocket used for communication.
     * @throws std::runtime_error if the provided socket is null.
     */
    explicit BaseProtocol(std::shared_ptr<BaseSocket> skt);

    /**
     * @brief Destructor for BaseProtocol.
     */
    virtual ~BaseProtocol() = default;

    /**
     * @brief Checks if the underlying socket streams are closed.
     * @return True if send or receive stream is closed, false otherwise.
     */
    bool is_closed() const;

    /**
     * @brief Checks if the underlying socket is open for communication.
     * @return True if both send and receive streams are open.
     */
    bool is_open() const;

    /**
     * @brief Sends a Message over the network.
     * @param message The Message to serialize and send.
     * @throws std::runtime_error if serialization yields an empty payload.
     */
    void send(const Message& message);

    /**
     * @brief Receives a Message from the network.
     * @return The deserialized Message object.
     */
    Message recv();

    /**
     * @brief Closes both send and receive streams and the socket.
     */
    void close();

private:
    /**
     * @brief Serializes the payload header to determine the message type.
     * @return The serialized MessageType.
     */
    payload_t serialize(MessageType type) const;

    /**
     * @brief Deserializes the payload header to determine the message type.
     * @return The deserialized MessageType.
     */
    MessageType deserialize_message_type();

    /**
     * @brief Deserializes the payload header to determine the message length.
     * @return The deserialized length of the message.
     */
    uint16_t deserialize_message_length();

protected:
    /**
     * @brief Serializes a Message into a payload for transmission.
     * @param message The Message to serialize.
     * @return Vector of bytes representing the serialized message.
     */
    virtual payload_t serialize_message(const Message& message) const = 0;

    /**
     * @brief Receives and deserializes the payload into a Message object.
     * @param msg_type The type of message to deserialize.
     * @return The deserialized Message object.
     */
    virtual Message deserialize_message(const MessageType& type, payload_t& payload) const = 0;

    payload_t pop(payload_t& payload, size_t size) const;

    // Must be defined in this header file because it's used in other TUs
    template <typename K, typename V>
    payload_t serialize(const std::map<K, V>& map) const {
        payload_t result;

        payload_t length = serialize(static_cast<uint16_t>(map.size()));
        result.insert(result.begin(), length.begin(), length.end());
        for (const auto& [key, value]: map) {
            payload_t key_payload = serialize(key);
            payload_t value_payload;

            value_payload = serialize(value);

            result.insert(result.end(), key_payload.begin(), key_payload.end());
            result.insert(result.end(), value_payload.begin(), value_payload.end());
        }

        return result;
    }

    template <typename T>
    payload_t serialize(const std::optional<T>& v) const;

    // Must be defined in this header file because it's used in other TUs
    template <typename T>
    payload_t serialize(const std::vector<T>& v) const {
        payload_t payload;

        payload_t length = serialize(static_cast<uint16_t>(v.size()));
        payload.insert(payload.end(), length.begin(), length.end());
        for (const auto& item: v) {
            auto serialized_item = serialize(item);
            payload.insert(payload.end(), serialized_item.begin(), serialized_item.end());
        }
        return payload;
    }

    template <typename T>
    typename std::enable_if<std::is_enum<T>::value, payload_t>::type serialize(
            const T& enum_value) const {
        return serialize(static_cast<uint8_t>(enum_value));
    }

    template <typename T>
    typename std::enable_if<!std::is_enum<T>::value && !is_vector<T>::value &&
                                    !is_optional<T>::value && !is_map<T>::value,
                            payload_t>::type
            serialize(const T& value) const;

    template <typename T>
    typename std::enable_if<is_optional<T>::value, T>::type deserialize(payload_t& payload) const {
        bool has_value = deserialize<bool>(payload);

        if (has_value) {
            return deserialize<T>(payload);
        }

        return {};
    }

    template <typename K, typename V>
    std::map<K, V> deserialize(payload_t& payload) const {
        uint16_t length = deserialize<uint16_t>(payload);
        std::map<K, V> result;

        for (size_t i = 0; i < length; i++) {
            K key = deserialize<K>(payload);
            result.emplace(key, deserialize<V>(payload));
        }

        return result;
    }

    template <typename T>
    typename std::enable_if<is_vector<T>::value, T>::type deserialize(payload_t& payload) const {
        uint16_t length = deserialize<uint16_t>(payload);

        std::vector<typename T::value_type> result;
        for (size_t i = 0; i < length; i++) {
            typename T::value_type item = deserialize<typename T::value_type>(payload);
            result.push_back(item);
        }

        return result;
    }

    template <typename T>
    typename std::enable_if<std::is_enum<T>::value, T>::type deserialize(payload_t& payload) const {
        auto underlying_value = deserialize<uint8_t>(payload);
        return static_cast<T>(underlying_value);
    }

    template <typename T>
    typename std::enable_if<!std::is_enum<T>::value && !is_vector<T>::value &&
                                    !is_optional<T>::value && !is_map<T>::value,
                            T>::type
            deserialize(payload_t& payload) const;
};
