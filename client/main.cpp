#include <exception>
#include <iostream>

#include "client/client.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int, char**) {
    Client client;
    client.run();
    return 0;
}
