#include "client.h"

#include <atomic>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <arpa/inet.h>

#include "common/queue.h"
#include "common/socket.h"
#include "common/thread.h"

#if CLIENT_TYPE == text
#include "text_display.h"
#include "text_input.h"
#endif

#include "receiver.h"
#include "sender.h"

Client::Client(const char* hostname, const char* port):
        client_socket(std::make_shared<Socket>(hostname, port)),
#if CLIENT_TYPE == text
        input(std::make_unique<TextInput>(input_queue)),
        display(std::make_unique<TextDisplay>(display_queue)),
#endif
        sender(client_socket, input_queue),       // Feed the input queue to the sender
        receiver(client_socket, display_queue) {  // Feed the receiver queue to the display
    input->start();
    display->start();
    sender.start();
    receiver.start();
}

void Client::run() {
    while (input->is_alive() && display->is_alive() && sender.is_alive() && receiver.is_alive())
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    input->stop();
    input->join();
    display->stop();
    display->join();
    sender.stop();
    sender.join();
    receiver.stop();
    receiver.join();
}
