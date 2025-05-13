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

#if UI_TYPE == tui
#include "text_display.h"
#elif UI_TYPE == gui
#include "qt_display.h"
#include "sdl_display.h"
#endif

#include "receiver.h"
#include "sender.h"

Client::Client():
#if UI_TYPE == tui
        display(std::make_unique<TextDisplay>(display_queue, input_queue))
#elif UI_TYPE == gui
        display(std::make_unique<QtDisplay>(display_queue, input_queue))
#endif
{
    display->start();
}

void Client::run() {
    while (display->is_alive()) {
        Message msg;
        input_queue.try_pop(msg);

        if (msg.get_type() != MessageType::CONN_REQ) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
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

    // Feed the input queue to the sender
    sender = std::make_unique<ClientSender>(protocol, input_queue);
    sender->start();

    // Feed the receiver queue to the display
    receiver = std::make_unique<ClientReceiver>(protocol, display_queue);
    receiver->start();

    // TODO switch display from QtDisplay to SDLDisplay
#if UI_TYPE == tui
    // No need to switch, TUI handles both stages
#elif UI_TYPE == gui
    display->stop();
    display->join();
    display = std::make_unique<SDLDisplay>(display_queue, input_queue);
    display->start();
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
