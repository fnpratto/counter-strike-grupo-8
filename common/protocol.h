#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

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
    payload_t serialize_message_type(MessageType type) const;

    /**
     * @brief Serializes the payload header to determine the message type.
     * @return The serialized MessageType.
     */
    payload_t serialize_message_length(const payload_t& msg) const;

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

    template <typename T>
    payload_t serialize(const T& value) const;

    template <typename T>
    T deserialize(payload_t& payload) const;
};
