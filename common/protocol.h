#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "common/socket.h"

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
 * @brief Abstract base class for message-based protocols using a Socket.
 *
 * Manages a Socket to send and receive Message objects over the network.
 */
class BaseProtocol {
protected:
    Socket socket;

public:
    /**
     * @brief Constructs the protocol with a given socket.
     * @param skt Shared pointer to a Socket used for communication.
     * @throws std::runtime_error if the provided socket is null.
     */
    explicit BaseProtocol(Socket&& skt);

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

    payload_t serialize(const uint8_t& i) const {
        payload_t payload(1);
        payload[0] = static_cast<char>(i);
        return payload;
    }

    payload_t serialize(const std::string& str) const {
        payload_t length = serialize(static_cast<uint16_t>(str.size()));

        payload_t payload;
        payload.reserve(length.size() + str.size());

        payload.insert(payload.end(), length.begin(), length.end());
        payload.insert(payload.end(), str.data(), str.data() + str.size());

        return payload;
    }

    payload_t serialize(const float& f) const {
        payload_t payload;

        float network_f = htonl(f);
        payload.insert(payload.end(), reinterpret_cast<const char*>(&network_f),
                       reinterpret_cast<const char*>(&network_f) + sizeof(network_f));

        return payload;
    }

    payload_t serialize(const bool& b) const {
        payload_t payload(1);
        payload[0] = static_cast<char>(b);
        return payload;
    }

    payload_t serialize(const uint16_t& i) const {
        payload_t payload;

        uint16_t data = htons(static_cast<uint16_t>(i));
        payload.insert(payload.begin(), reinterpret_cast<const char*>(&data),
                       reinterpret_cast<const char*>(&data) + sizeof(data));

        return payload;
    }

    payload_t serialize(const int& i) const { return serialize(static_cast<uint16_t>(i)); }

    payload_t serialize(const Vector2D& vec) const {
        payload_t payload;
        payload.reserve(2 * sizeof(uint16_t));
        payload_t x_payload = serialize(vec.get_x());
        payload_t y_payload = serialize(vec.get_y());
        payload.insert(payload.end(), x_payload.begin(), x_payload.end());
        payload.insert(payload.end(), y_payload.begin(), y_payload.end());
        return payload;
    }

    payload_t serialize(const TimePoint& time_point) const {
        payload_t payload;
        payload.reserve(sizeof(uint64_t));
        uint64_t time_ns = htonl(time_point.time_since_epoch().count());
        payload.insert(payload.end(), reinterpret_cast<const char*>(&time_ns),
                       reinterpret_cast<const char*>(&time_ns) + sizeof(time_ns));
        return payload;
    }

    payload_t serialize(const GameInfo& game_info) const {
        payload_t payload;

        payload_t name_payload = serialize(game_info.name);
        payload_t count_payload = serialize(game_info.players_count);
        payload_t phase_payload = serialize(game_info.phase);
        payload.reserve(name_payload.size() + count_payload.size() + phase_payload.size());
        payload.insert(payload.end(), name_payload.begin(), name_payload.end());
        payload.insert(payload.end(), count_payload.begin(), count_payload.end());
        payload.insert(payload.end(), phase_payload.begin(), phase_payload.end());

        return payload;
    }

    payload_t serialize(const CharacterType& character_type) const {
        return serialize(static_cast<uint8_t>(character_type));
    }

    template <typename T>
    typename std::enable_if<std::is_enum<T>::value, payload_t>::type serialize(
            const T& enum_value) const {
        return serialize(static_cast<uint8_t>(enum_value));
    }

    template <typename T>
    payload_t serialize(const std::vector<T>& v) const {
        payload_t payload;

        payload_t length = serialize(static_cast<uint16_t>(v.size()));
        payload.reserve(length.size() + sizeof(T) * v.size());
        payload.insert(payload.end(), length.begin(), length.end());
        for (const auto& item: v) {
            auto serialized_item = serialize(item);
            payload.insert(payload.end(), serialized_item.begin(), serialized_item.end());
        }
        return payload;
    }

    template <typename T>
    typename std::enable_if<!std::is_enum<T>::value, T>::type deserialize(payload_t& payload) const;

    template <typename T>
    typename std::enable_if<std::is_enum<T>::value, T>::type deserialize(payload_t& payload) const {
        auto underlying_value = deserialize<uint8_t>(payload);
        return static_cast<T>(underlying_value);
    }
};
