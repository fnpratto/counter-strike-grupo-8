#include "mock_socket.h"

#include <algorithm>

#include <sys/socket.h>

// BaseSocket interface implementation
int MockSocket::sendsome(const void* data, unsigned int sz) {
    if (should_throw_on_send) {
        throw std::runtime_error("Mock socket send error");
    }

    if (send_closed) {
        return 0;
    }

    const char* char_data = static_cast<const char*>(data);
    written_data.insert(written_data.end(), char_data, char_data + sz);
    return sz;
}

int MockSocket::recvsome(void* data, unsigned int sz) {
    if (should_throw_on_recv) {
        throw std::runtime_error("Mock socket recv error");
    }

    if (recv_closed || read_data_queue.empty()) {
        return 0;
    }

    std::vector<char>& front_data = read_data_queue.front();
    unsigned int bytes_to_copy = std::min(sz, static_cast<unsigned int>(front_data.size()));

    std::memcpy(data, front_data.data(), bytes_to_copy);

    if (bytes_to_copy == front_data.size()) {
        read_data_queue.pop();
    } else {
        front_data.erase(front_data.begin(), front_data.begin() + bytes_to_copy);
    }

    return bytes_to_copy;
}

int MockSocket::sendall(const void* data, unsigned int sz) { return sendsome(data, sz); }

int MockSocket::recvall(void* data, unsigned int sz) {
    if (should_throw_on_recv) {
        throw std::runtime_error("Mock socket recv error");
    }

    if (recv_closed) {
        return 0;
    }

    unsigned int total_received = 0;
    char* char_data = static_cast<char*>(data);

    while (total_received < sz && !read_data_queue.empty()) {
        unsigned int remaining = sz - total_received;
        int received = recvsome(char_data + total_received, remaining);
        if (received == 0) {
            break;
        }
        total_received += received;
    }

    if (total_received < sz && read_data_queue.empty()) {
        if (total_received == 0) {
            return 0;
        } else {
            throw std::runtime_error("Mock socket: partial data received");
        }
    }

    return total_received;
}

bool MockSocket::is_stream_send_closed() const { return send_closed; }

bool MockSocket::is_stream_recv_closed() const { return recv_closed; }

void MockSocket::shutdown(int how) {
    (void)how;  // Unused parameter
    send_closed = true;
    recv_closed = true;
}

int MockSocket::close() {
    send_closed = true;
    recv_closed = true;
    return 0;
}

// Test control methods
void MockSocket::queue_read_data(const std::vector<char>& data) { read_data_queue.push(data); }

void MockSocket::queue_read_data(const void* data, size_t size) {
    std::vector<char> vec(static_cast<const char*>(data), static_cast<const char*>(data) + size);
    read_data_queue.push(vec);
}

const std::vector<char>& MockSocket::get_written_data() const { return written_data; }

void MockSocket::clear_written_data() { written_data.clear(); }

void MockSocket::set_send_closed(bool closed) { send_closed = closed; }

void MockSocket::set_recv_closed(bool closed) { recv_closed = closed; }

void MockSocket::set_should_throw_on_send(bool should_throw) {
    should_throw_on_send = should_throw;
}

void MockSocket::set_should_throw_on_recv(bool should_throw) {
    should_throw_on_recv = should_throw;
}

void MockSocket::reset() {
    while (!read_data_queue.empty()) {
        read_data_queue.pop();
    }
    written_data.clear();
    send_closed = false;
    recv_closed = false;
    should_throw_on_send = false;
    should_throw_on_recv = false;
}
