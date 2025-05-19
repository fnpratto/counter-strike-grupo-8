#include <memory>
#include <utility>

#include "common/message.h"
#include "common/queue.h"

typedef std::pair<std::shared_ptr<Queue<Message>>, std::shared_ptr<Queue<Message>>> pipe_t;
