#pragma once

#include <string>

#define DESERIALIZERS(C)                                                      \
                                                                              \
    template <>                                                               \
    uint8_t C::deserialize(payload_t& payload) const {                        \
        payload_t data = pop(payload, sizeof(uint8_t));                       \
        return static_cast<uint8_t>(data[0]);                                 \
    }                                                                         \
                                                                              \
    template <>                                                               \
    uint16_t C::deserialize(payload_t& payload) const {                       \
        payload_t data = pop(payload, sizeof(uint16_t));                      \
        return ntohs(*reinterpret_cast<const uint16_t*>(data.data()));        \
    }                                                                         \
                                                                              \
    template <>                                                               \
    float C::deserialize(payload_t& payload) const {                          \
        payload_t data = pop(payload, sizeof(float));                         \
        uint32_t network_f = *reinterpret_cast<const uint32_t*>(data.data()); \
        return ntohl(network_f);                                              \
    }                                                                         \
                                                                              \
    template <>                                                               \
    std::string C::deserialize(payload_t& payload) const {                    \
        uint16_t length = deserialize<uint16_t>(payload);                     \
        length = ntohs(length);                                               \
                                                                              \
        payload_t data = pop(payload, length);                                \
                                                                              \
        return std::string(data.data(), data.size());                         \
    }
