#include "bomb.h"

#include "weapons_config.h"

Bomb::Bomb():
        Logic<BombState, BombUpdate>(BombState(false)),
        secs_to_explode(BombConfig::secs_to_explode) {}

int Bomb::get_secs_to_explode() const { return secs_to_explode; }

void Bomb::plant() { state.set_planted(true); }
