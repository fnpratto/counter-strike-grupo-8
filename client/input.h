#pragma once

#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>

#include "common/message.h"

#include "queue.h"
#include "thread.h"

class Input: public Thread {
protected:
    Queue<Message>& queue;

public:
    explicit Input(Queue<Message>& q): queue(q) {}

    void run() override;
    virtual void setup() = 0;
    virtual void work() = 0;
};
