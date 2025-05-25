#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "common/message.h"
#include "common/protocol.h"
#include "common/socket.h"

#include "errors.h"

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
    payload_t serialize(const T& value) const;

    Message deserialize_message(const MessageType& type, payload_t& payload) const override;

    template <typename T>
    T deserialize(payload_t& payload) const;
};
