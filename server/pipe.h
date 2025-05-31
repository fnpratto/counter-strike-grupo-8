#include <memory>
#include <utility>

#include "common/message.h"
#include "common/queue.h"
#include "server/player_message.h"

typedef std::pair<std::shared_ptr<Queue<PlayerMessage>>, std::shared_ptr<Queue<Message>>> pipe_t;
