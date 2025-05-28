#include "protocol.h"

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "common/socket.h"

#include "message.h"

BaseProtocol::BaseProtocol(Socket&& skt): socket(std::move(skt)) {}

bool BaseProtocol::is_closed() const {
    return socket.is_stream_recv_closed() || socket.is_stream_send_closed();
}

bool BaseProtocol::is_open() const { return !is_closed(); }

void BaseProtocol::send(const Message& message) {
    payload_t payload;
    payload_t header = serialize(message.get_type());
    payload_t content = serialize_message(message);  // TODO replace with a function pointer
    payload_t length = serialize(static_cast<uint16_t>(content.size()));

    payload.reserve(header.size() + length.size() + content.size());
    payload.insert(payload.end(), header.begin(), header.end());
    payload.insert(payload.end(), length.begin(), length.end());
    payload.insert(payload.end(), content.begin(), content.end());

    if (payload.size() == 0)
        throw std::runtime_error("BaseProtocol: Empty payload");

    socket.sendall(payload.data(), payload.size());
}

payload_t BaseProtocol::serialize(MessageType type) const {
    payload_t header(1);
    header[0] = static_cast<uint8_t>(type);
    return header;
}

Message BaseProtocol::recv() {
    MessageType msg_type = deserialize_message_type();
    uint16_t length = deserialize_message_length();

    payload_t content(length);
    socket.recvall(content.data(), length);

    return deserialize_message(msg_type, content);
}

MessageType BaseProtocol::deserialize_message_type() {
    payload_t header(1);
    socket.recvall(header.data(), 1);

    return static_cast<MessageType>(header[0]);
}

uint16_t BaseProtocol::deserialize_message_length() {
    payload_t payload(2);
    socket.recvall(payload.data(), 2);

    return ntohs(*reinterpret_cast<const uint16_t*>(payload.data()));
}

payload_t BaseProtocol::pop(payload_t& payload, size_t size) const {
    payload_t result(payload.begin(), payload.begin() + size);
    payload.erase(payload.begin(), payload.begin() + size);
    return result;
}

void BaseProtocol::close() {
    if (is_closed())
        return;

    socket.shutdown(SHUT_RDWR);
    socket.close();
}
