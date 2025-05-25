#pragma once

#include <string>

#define SERIALIZERS(C)                                                                 \
    template <>                                                                        \
    payload_t C::serialize(const uint8_t& i) const {                                   \
        payload_t payload(sizeof(i));                                                  \
                                                                                       \
        uint8_t data = htons(static_cast<uint8_t>(i));                                 \
        payload.insert(payload.end(), reinterpret_cast<const char*>(&data),            \
                       reinterpret_cast<const char*>(&data) + sizeof(data));           \
                                                                                       \
        return payload;                                                                \
    }                                                                                  \
                                                                                       \
    template <>                                                                        \
    payload_t C::serialize(const uint16_t& i) const {                                  \
        payload_t payload(sizeof(i));                                                  \
                                                                                       \
        uint16_t data = htons(static_cast<uint16_t>(i));                               \
        payload.insert(payload.end(), reinterpret_cast<const char*>(&data),            \
                       reinterpret_cast<const char*>(&data) + sizeof(data));           \
                                                                                       \
        return payload;                                                                \
    }                                                                                  \
                                                                                       \
    template <>                                                                        \
    payload_t C::serialize(const std::string& str) const {                             \
        payload_t length = serialize(static_cast<uint16_t>(str.size()));               \
                                                                                       \
        payload_t payload;                                                             \
        payload.reserve(length.size() + str.size());                                   \
                                                                                       \
        payload.insert(payload.end(), length.begin(), length.end());                   \
        payload.insert(payload.end(), str.data(), str.data() + str.size());            \
                                                                                       \
        return payload;                                                                \
    }                                                                                  \
                                                                                       \
    template <>                                                                        \
    payload_t C::serialize(const float& f) const {                                     \
        payload_t payload(sizeof(f));                                                  \
                                                                                       \
        float network_f = htonl(f);                                                    \
        payload.insert(payload.end(), reinterpret_cast<const char*>(&network_f),       \
                       reinterpret_cast<const char*>(&network_f) + sizeof(network_f)); \
                                                                                       \
        return payload;                                                                \
    }                                                                                  \
                                                                                       \
    template <>                                                                        \
    payload_t C::serialize(const bool& b) const {                                      \
        payload_t payload(1);                                                          \
        payload[0] = static_cast<char>(b);                                             \
        return payload;                                                                \
    }
