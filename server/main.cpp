#include <arpa/inet.h>

#include "common/socket.h"
#include "common/thread.h"

#include "acceptor.h"
#include "lobby_monitor.h"

int main(int argc, const char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
            return 1;
        }

        Acceptor acceptor(argv[1]);
        acceptor.start();

        while (std::cin.get() != 'q') {}

        acceptor.stop();
        acceptor.join();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
