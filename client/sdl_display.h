#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"

#include "display.h"
#include "sdl_input.h"

class SDLDisplay: public Display {
public:
    explicit SDLDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue);

    void run() override;
    void stop() override;

private:
    std::atomic<bool> quit_flag;
    std::unique_ptr<SDLInput> input_handler;
};
