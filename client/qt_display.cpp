#include "qt_display.h"

#include <iostream>
#include <memory>
#include <string>

#include "./qt/log_in_window.h"
#include "common/message.h"

#include "display.h"

QtDisplay::QtDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue):
        LobbyDisplay(input_queue, output_queue) {}

void QtDisplay::run() {
    LogInWindow login_window(input_queue, output_queue);
    login_window.show();
    app.exec();
}

void QtDisplay::stop() { app.quit(); }
