#include "map.h"

#include <ctime>

#include "server/errors.h"

Map::Map(std::string&& name) : name(std::move(name)) { 
    std::srand(std::time(nullptr)); 
}

bool Map::is_collidable(const Vector2D& pos) {
    (void)pos;
    // TODO: check collision
    return false;
}

bool Map::is_in_spawn_tt(const Vector2D& pos) {
    return is_pos_in_vector(spawns_tts, pos);
}

bool Map::is_in_spawn_ct(const Vector2D& pos) {
    return is_pos_in_vector(spawns_cts, pos);
}

bool Map::is_pos_in_vector(const std::vector<Vector2D>& vector, const Vector2D& pos) {
    for (const Vector2D& p : vector) {
        if (p == pos)
            return true;
    }
    return false;
}

void Map::add_tiles_row(std::vector<MapTileType>&& tiles_row) { 
    tiles.push_back(std::move(tiles_row)); 
}

void Map::add_bullet(Bullet&& bullet) { bullets.push_back(std::move(bullet)); }

void Map::add_spawn_tt(Vector2D&& spawn_tt) { spawns_tts.push_back(std::move(spawn_tt)); }

void Map::add_spawn_ct(Vector2D&& spawn_ct) { spawns_cts.push_back(std::move(spawn_ct)); }

void Map::add_bomb_site(Vector2D&& bomb_site) { bomb_sites.push_back(std::move(bomb_site)); }

Vector2D Map::random_initial_pos(Team team) {
    // std::vector<Vector2D>& spawns = (team == Team::Terrorist) ? spawns_tts : spawns_cts;
    
    // const int max_attempts = 10;
    // for (int i = 0; i < max_attempts; ++i) {
    //     int idx = std::rand() % spawns.size();
    //     Vector2D pos = spawns.at(idx);
    //     if (!is_collidable(pos))
    //         return pos;
    // }

    // throw ChooseInitialPositionError();
    (void)team;
    return Vector2D();
}

void Map::update_player_pos(const std::string& player_name, Vector2D&& pos) {
    players[player_name] = pos;
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
