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
    payload_t payload = serialize_message(message);

    if (payload.size() == 0)
        throw std::runtime_error("BaseProtocol: Empty payload");

    socket.sendall(payload.data(), payload.size());
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

payload_t serialize(const uint16_t& i) {
    payload_t payload(sizeof(i));

    uint16_t data = htons(static_cast<uint16_t>(i));
    payload.insert(payload.end(), reinterpret_cast<const char*>(&data),
                   reinterpret_cast<const char*>(&data) + sizeof(data));

    return payload;
}

payload_t serialize(const std::string& str) {
    payload_t length = serialize(static_cast<uint16_t>(str.size()));

    payload_t payload;
    payload.reserve(length.size() + str.size());

    payload.insert(payload.end(), length.begin(), length.end());
    payload.insert(payload.end(), str.data(), str.data() + str.size());

    return payload;
}

payload_t serialize(const bool& b) {
    payload_t payload(1);
    payload[0] = static_cast<char>(b);
    return payload;
}

void BaseProtocol::close() {
    if (is_closed())
        return;

    socket.shutdown(SHUT_RDWR);
    socket.close();
}
