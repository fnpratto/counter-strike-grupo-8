#include "qt_display.h"

#include <iostream>
#include <memory>
#include <string>

#include "common/message.h"

#include "display.h"

// TODO: Implement QtDisplay

QtDisplay::QtDisplay(Queue<Message>& input_queue, Queue<Message>& output_queue):
        Display(input_queue, output_queue) {}
void QtDisplay::run() { output_queue.push(Message(ConnectionRequest("localhost", "6969"))); }
void QtDisplay::stop() { Thread::stop(); }
