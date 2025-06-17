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
     * @param skt Shared pointer to a connected BaseSocket.
     */
    explicit ClientProtocol(std::shared_ptr<BaseSocket> skt): BaseProtocol(std::move(skt)) {}

    payload_t serialize_message(const Message& message) const override;

    template <typename T>
    payload_t serialize_msg(const T& value) const;

    Message deserialize_message(const MessageType& type, payload_t& payload) const override;

    template <typename T>
    T deserialize_msg(payload_t& payload) const;
};
