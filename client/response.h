#pragma once

#include <string>
#include <utility>

// @brief Represents a response that indicates if connected to a server.
class ConnectionResponse {
    bool status;

public:
    inline explicit ConnectionResponse(bool status = false): status(status) {}
    bool has_failed() const { return !status; }
};
