#include "common/game_state.h"

#include <utility>

#include "server/game/game_phase.h"

GameState::GameState(std::unique_ptr<Clock>&& clock): phase(std::move(clock)) {
    updates.set_phase(phase.get_updates());
    updates.set_num_rounds(num_rounds);
}
