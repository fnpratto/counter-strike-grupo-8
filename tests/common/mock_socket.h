#pragma once

#include <cstring>
#include <memory>
#include <queue>
#include <stdexcept>
#include <vector>

#include "common/base_socket.h"

/**
 * @class MockSocket
 * @brief Mock implementation of BaseSocket for testing purposes.
 *
 * Allows tests to control read/write operations, simulate network conditions,
 * and verify protocol behavior without actual network communication.
 */
class MockSocket: public BaseSocket {
private:
    std::queue<std::vector<char>> read_data_queue;
    std::vector<char> written_data;
    bool send_closed = false;
    bool recv_closed = false;
    bool should_throw_on_send = false;
    bool should_throw_on_recv = false;

public:
    MockSocket() = default;
    ~MockSocket() override = default;

    // BaseSocket interface implementation
    int sendsome(const void* data, unsigned int sz) override;
    int recvsome(void* data, unsigned int sz) override;
    int sendall(const void* data, unsigned int sz) override;
    int recvall(void* data, unsigned int sz) override;
    bool is_stream_send_closed() const override;
    bool is_stream_recv_closed() const override;
    void shutdown(int how) override;
    int close() override;

    // Test control methods
    void queue_read_data(const std::vector<char>& data);
    void queue_read_data(const void* data, size_t size);
    const std::vector<char>& get_written_data() const;
    void clear_written_data();
    void set_send_closed(bool closed);
    void set_recv_closed(bool closed);
    void set_should_throw_on_send(bool should_throw);
    void set_should_throw_on_recv(bool should_throw);
    void reset();
};
