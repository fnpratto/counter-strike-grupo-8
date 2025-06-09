#pragma once

#include <QApplication>
#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"

#include "lobby_display.h"

class QtDisplay: public LobbyDisplay {
    int argc = 0;
    QApplication app = QApplication(argc, nullptr);

public:
    explicit QtDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue);

    void run() override;
    void stop() override;
};
