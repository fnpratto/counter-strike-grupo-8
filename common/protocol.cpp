#include "protocol.h"

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "common/socket.h"
#include "server/clock/clock.h"

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

payload_t BaseProtocol::pop(payload_t& payload, size_t size) const {
    if (size > payload.size()) {
        throw std::runtime_error("BaseProtocol::pop: Requested size (" + std::to_string(size) +
                                 ") exceeds payload size (" + std::to_string(payload.size()) + ")");
    }
    payload_t result(payload.begin(), payload.begin() + size);
    payload.erase(payload.begin(), payload.begin() + size);
    return result;
}

template <>
uint8_t BaseProtocol::deserialize<uint8_t>(payload_t& payload) const {
    payload_t data = pop(payload, sizeof(uint8_t));
    return static_cast<uint8_t>(data[0]);
}

template <>
uint16_t BaseProtocol::deserialize<uint16_t>(payload_t& payload) const {
    payload_t data = pop(payload, sizeof(uint16_t));
    return ntohs(*reinterpret_cast<const uint16_t*>(data.data()));
}

template <>
float BaseProtocol::deserialize<float>(payload_t& payload) const {
    payload_t data = pop(payload, sizeof(float));
    uint32_t network_f = *reinterpret_cast<const uint32_t*>(data.data());
    return ntohl(network_f);
}

template <>
std::string BaseProtocol::deserialize<std::string>(payload_t& payload) const {
    uint16_t length = deserialize<uint16_t>(payload);

    payload_t data = pop(payload, length);

    return std::string(data.data(), data.size());
}

template <>
Vector2D BaseProtocol::deserialize<Vector2D>(payload_t& payload) const {
    float x = deserialize<float>(payload);
    float y = deserialize<float>(payload);
    return Vector2D(x, y);
}

template <>
int BaseProtocol::deserialize<int>(payload_t& payload) const {
    return static_cast<int>(deserialize<uint16_t>(payload));
}

template <>
bool BaseProtocol::deserialize<bool>(payload_t& payload) const {
    payload_t data = pop(payload, sizeof(uint8_t));
    return static_cast<bool>(data[0]);
}

template <>
TimePoint BaseProtocol::deserialize<TimePoint>(payload_t& payload) const {
    payload_t data = pop(payload, sizeof(uint64_t));
    uint64_t time_ns = ntohl(*reinterpret_cast<const uint64_t*>(data.data()));
    return TimePoint(std::chrono::nanoseconds(time_ns));
}

// template <>
// MessageType BaseProtocol::deserialize<MessageType>(payload_t& payload) const {
//     payload_t data = pop(payload, sizeof(uint8_t));
//     return static_cast<MessageType>(data[0]);
// }

Message BaseProtocol::recv() {
    payload_t header(sizeof(uint8_t) + sizeof(uint16_t));
    socket.recvall(header.data(), sizeof(uint8_t) + sizeof(uint16_t));

    MessageType msg_type = deserialize<MessageType>(header);
    uint16_t length = deserialize<uint16_t>(header);

    payload_t content(length);
    socket.recvall(content.data(), length);

    return deserialize_message(msg_type, content);
}

void BaseProtocol::close() {
    if (is_closed())
        return;

    socket.shutdown(SHUT_RDWR);
    socket.close();
}
