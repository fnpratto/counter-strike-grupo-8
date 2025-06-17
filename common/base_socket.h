#ifndef BASE_SOCKET_H
#define BASE_SOCKET_H

/**
 * @class BaseSocket
 * @brief Abstract base class for socket implementations.
 *
 * Defines the interface for socket operations that can be implemented
 * by concrete socket classes or mocked for testing.
 */
class BaseSocket {
public:
    virtual ~BaseSocket() = default;

    // Send/receive operations
    virtual int sendsome(const void* data, unsigned int sz) = 0;
    virtual int recvsome(void* data, unsigned int sz) = 0;
    virtual int sendall(const void* data, unsigned int sz) = 0;
    virtual int recvall(void* data, unsigned int sz) = 0;

    // Stream status
    virtual bool is_stream_send_closed() const = 0;
    virtual bool is_stream_recv_closed() const = 0;

    // Connection management
    virtual void shutdown(int how) = 0;
    virtual int close() = 0;
};

#endif  // BASE_SOCKET_H
