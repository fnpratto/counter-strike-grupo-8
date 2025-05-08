#include "display.h"

#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"

void Display::run() {
    Message message;
    while (should_keep_running()) {
        if (queue.try_pop(message))
            draw(message);
    }
}
