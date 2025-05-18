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

// Default to GUI if not defined
// Actually not necessary just better for IntelliSense
#ifndef UI_TYPE_GUI
#define UI_TYPE_GUI
#endif  // !UI_TYPE_GUI

#ifdef UI_TYPE_GUI
#include "qt_display.h"
#include "sdl_display.h"
#elif defined(UI_TYPE_TUI)
#include "text_display.h"
#endif

#include "receiver.h"
#include "sender.h"

Client::Client():
#ifdef UI_TYPE_GUI
        display(std::make_unique<QtDisplay>(display_queue, input_queue)),
#elif defined(UI_TYPE_TUI)
        display(std::make_unique<TextDisplay>(display_queue, input_queue)),
#endif
        sender(nullptr),
        receiver(nullptr) {
    display->start();
}

void Client::run() {
    while (true) {
        Message msg;
        input_queue.try_pop(msg);

        if (msg.get_type() != MessageType::CONN_REQ) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            // fuck this specific race condition
            // if (!display->is_alive()) {
            //     display->stop();
            //     display->join();
            //     return;
            // }

            continue;
        }

        auto conn_req = msg.get_content<ConnectionRequest>();

        try {
            protocol = std::make_shared<ClientProtocol>(
                    Socket(conn_req.get_ip().c_str(),
                           conn_req.get_port().c_str()));  // TODO this might throw
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            continue;
        }

        break;
    }

    std::cout << "Connected to server" << std::endl;

    // Feed the input queue to the sender
    sender = std::make_unique<ClientSender>(protocol, input_queue);
    sender->start();

    // Feed the receiver queue to the display
    receiver = std::make_unique<ClientReceiver>(protocol, display_queue);
    receiver->start();

#ifdef UI_TYPE_GUI
    display->stop();
    display->join();
    display = std::make_unique<SDLDisplay>(display_queue, input_queue);
    display->start();
    std::cout << "Switched to SDLDisplay" << std::endl;
#elif defined(UI_TYPE_TUI)
    // No need to switch, TUI handles both stages
#endif

    while (display->is_alive() && sender->is_alive() && receiver->is_alive())
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    display->stop();
    display->join();
    sender->stop();
    sender->join();
    receiver->stop();
    receiver->join();

    // TODO: this is all happy path code, we should handle non happy paths (i.e. uninitialized
    // threads)
}
