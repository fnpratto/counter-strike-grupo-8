#include "bomb.h"

#include "items_config.h"

Bomb::Bomb(): Logic<BombState, BombUpdate>(BombState(false)) {}

bool Bomb::is_planted() const { return state.is_planted(); }

int Bomb::get_secs_to_explode() const { return BombConfig::secs_to_explode; }

void Bomb::plant() { state.set_planted(true); }
