#include <exception>
#include <iostream>

#include "client/client.h"

int main() try {
    Client client;
    client.run();
    return 0;
} catch (std::exception& e) {
    // If case of error, print it and exit with error
    std::cerr << e.what() << std::endl;
    return 1;
}
