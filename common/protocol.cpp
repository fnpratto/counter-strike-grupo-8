#include "protocol.h"

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <sys/socket.h>

#include "message.h"
#include "socket.h"

BaseProtocol::BaseProtocol(std::shared_ptr<Socket>&& skt): socket(std::move(skt)) {
    if (!socket)
        throw std::runtime_error("BaseProtocol: Socket is null");
}

bool BaseProtocol::is_closed() const {
    return socket->is_stream_recv_closed() || socket->is_stream_send_closed();
}

bool BaseProtocol::is_open() const { return !is_closed(); }

void BaseProtocol::send(const Message& message) {
    payload_t payload = serialize_message(message);

    if (payload.size() == 0)
        throw std::runtime_error("BaseProtocol: Empty payload");

    this->socket->sendall(payload.data(), payload.size());
}

void BaseProtocol::close() {
    socket->shutdown(SHUT_RDWR);
    socket->close();
}
