#include "sdl_display.h"

#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"

#include "display.h"

// TODO: Implement SDLDisplay

SDLDisplay::SDLDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue):
        Display(input_queue, output_queue) {}
void SDLDisplay::run() {}
void SDLDisplay::stop() {}
