#include "map.h"

#include <ctime>

#include "server/errors.h"

Map::Map() { std::srand(std::time(nullptr)); }

bool Map::is_collidable(const Vector2D& pos) {
    (void)pos;
    // TODO: check collision
    return false;
}

Vector2D Map::random_initial_pos(Team team) {
    std::vector<Vector2D>& spawns = (team == Team::Terrorist) ? spawns_tts : spawns_cts;
    
    const int max_attempts = 10;
    for (int i = 0; i < max_attempts; ++i) {
        int idx = std::rand() % spawns.size();
        Vector2D pos = spawns.at(idx);
        if (!is_collidable(pos))
            return pos;
    }

    throw ChooseInitialPositionError();
}

void Map::update_player_pos(const std::string& player_name, Vector2D&& pos) {
    players[player_name] = pos;
}

void Map::add_bullet(Bullet&& bullet) {
    (void)bullet;
}

void Map::process_melee_attack(Knife&& knife) {
    (void)knife;
}

void Map::place_bomb(Bomb&& bomb) {
    (void)bomb;
}

// TODO: update bullets
void Map::update() {}

Map::~Map() {}
