#include "input.h"

#include <iostream>
#include <sstream>
#include <string>

#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>

#include "common/message.h"
#include "common/queue.h"
#include "common/thread.h"

void Input::run() {
    setup();

    while (should_keep_running()) {
        work();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
