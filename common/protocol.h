#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "message.h"
#include "socket.h"

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
    std::shared_ptr<Socket> socket;

public:
    /**
     * @brief Constructs the protocol with a given socket.
     * @param skt Shared pointer to a Socket used for communication.
     * @throws std::runtime_error if the provided socket is null.
     */
    explicit BaseProtocol(std::shared_ptr<Socket>&& skt);

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
    virtual Message recv() = 0;

    /**
     * @brief Closes both send and receive streams and the socket.
     */
    void close();

protected:
    /**
     * @brief Serializes a Message into a payload for transmission.
     * @param message The Message to serialize.
     * @return Vector of bytes representing the serialized message.
     */
    virtual payload_t serialize_message(const Message& message) = 0;
};
