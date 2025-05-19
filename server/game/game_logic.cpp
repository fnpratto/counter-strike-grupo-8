#include <vector>
#include <utility>

#include "game_logic.h"
#include "server/cons.h"
#include "server/errors.h"
#include "server/utils/vector_2d.h"
#include "game_state_builder.h"
#include "server/weapons/bullet.h"

using namespace GameConfig;
using namespace PlayerInitialConfig;

GameLogic::GameLogic(const Clock& clock, const Shop& shop, Map& map) :
        clock(clock),
        shop(shop),
        phase(clock),
        map(map) { std::srand(std::time(nullptr)); }

GameState GameLogic::get_game_state() const { return GameStateBuilder::build(*this); }

void GameLogic::add_player(const std::string& player_name) {
    if (!can_join(player_name))
        throw JoinGameError();

    Team team = choose_player_team();
    Vector2D initial_pos = map.random_initial_pos(team);
    Player player = Player(team, Inventory(), full_health, initial_pos);
    players.emplace(player_name, player);
    if (team == Team::Terrorist) {
        num_tts++;
    } else {
        num_cts++;
    }
    map.update_player_pos(player_name, std::move(initial_pos));
}

void GameLogic::select_team(const std::string& player_name, Team team) {
    if (!player_can_select_team(team))
        throw SelectTeamError();
    
    Team old_team;
    if (players.at(player_name).is_tt()) {
        old_team = Team::Terrorist;
    } else {
        old_team = Team::CounterTerrorist;
    }
    if (old_team == team)
        return;

    players.at(player_name).select_team(team);

    if (team == Team::Terrorist) {
        num_cts--;
        num_tts++;
    } else {
        num_tts--;
        num_cts++;
    }
}

// TODO: revise logic for starting a game.
void GameLogic::start_game() {
    if (phase.is_started())
        throw StartGameError();
    if (num_tts > 0)
        give_bomb_to_random_tt();
    phase.start_buying_phase();
}

void GameLogic::buy_gun(const std::string& player_name, GunType gun) {
    if (!phase.is_buying_phase())
        throw BuyGunError();
    if (player_not_in_spawn(player_name))
        return;
    int gun_price = shop.get_gun_price(gun);
    players.at(player_name).buy_gun(gun, gun_price);
}

void GameLogic::buy_ammo(const std::string& player_name, GunType gun) {
    if (!phase.is_buying_phase())
        throw BuyAmmoError();
    if (player_not_in_spawn(player_name))
        return;
    int num_mags = 1;
    if (gun == GunType::M3) {
        num_mags = 8;
    }
    WeaponSlot slot = (gun == GunType::Glock) ? WeaponSlot::Secondary : WeaponSlot::Primary;
    int ammo_price = shop.get_ammo_price(gun, num_mags);
    players.at(player_name).buy_ammo(slot, ammo_price, num_mags);
}

bool GameLogic::player_not_in_spawn(const std::string& player_name) {
    Player& player = players.at(player_name);
    Vector2D pos(player.get_pos_x(), player.get_pos_y());
    return (player.is_tt() && !map.is_in_spawn_tt(pos)) || 
                (player.is_ct() && !map.is_in_spawn_ct(pos));
}

void GameLogic::move(const std::string& player_name, int dx, int dy) {
    if (phase.is_buying_phase())
        return;
        
    Player& player = players.at(player_name);
    Vector2D old_pos(player.get_pos_x(), player.get_pos_y());
    Vector2D new_pos = old_pos + calculate_step(Vector2D(dx, dy));

    if (map.is_collidable(new_pos))
        return;

    player.move(new_pos);
    map.update_player_pos(player_name, std::move(new_pos));
}

Vector2D GameLogic::calculate_step(const Vector2D& dir) {
    float tick_duration = 1.0f / tickrate;
    Vector2D step = dir.normalized() * player_speed * tick_duration;
}

void GameLogic::shoot(const std::string& player_name, int x, int y) {
    Player& player = players.at(player_name);
    if (player.get_current_weapon() == WeaponSlot::Melee) {
        Knife knife = player.attack();
        map.process_melee_attack(std::move(knife));
    } else {
        std::vector<Bullet> bullets = player.shoot_gun(x, y, clock.now());
        for (Bullet& b : bullets) {
            map.add_bullet(std::move(b));
        }
    }
}

void GameLogic::switch_weapon(const std::string& player_name, WeaponSlot slot) {
    players.at(player_name).equip_weapon(slot);
}

void GameLogic::reload_weapon(const std::string& player_name) {
    players.at(player_name).reload();
}

void GameLogic::update_map() { map.update(); }

void GameLogic::update_round_phase() {
    if (num_rounds == max_rounds / 2)
        swap_teams();
    phase.update();
    if (phase.is_round_finished())
        num_rounds++;
}

Team GameLogic::choose_player_team() {
    if (num_tts > num_cts)
        return Team::CounterTerrorist;
    return Team::Terrorist;
}

void GameLogic::give_bomb_to_random_tt() {
    std::vector<std::string> tt_names;

    for (auto& [player_name, player] : players) {
        if (player.is_tt()) {
            tt_names.push_back(player_name);
        }
    }

    int random_index = rand() % num_tts;
    players.at(tt_names[random_index]).pick_bomb();
}

void GameLogic::swap_teams() {
    for (auto& [_, player] : players) {
        if (player.is_tt()) {
            player.select_team(Team::CounterTerrorist);
        } else if (player.is_ct()) {
            player.select_team(Team::Terrorist);
        }
    }
}

GameLogic::~GameLogic() {}

bool GameLogic::can_join(const std::string& player_name) const {
    return players.find(player_name) == players.end() && !is_full() && !phase.is_started();
}

bool GameLogic::player_can_select_team(Team& team) const {
    return !team_is_full(team) && !phase.is_started();
}

bool GameLogic::is_full() const { 
    return static_cast<int>(players.size()) == max_players; 
}

bool GameLogic::team_is_full(Team& team) const {
    if (team == Team::Terrorist)
        return num_tts == max_team_players;
    return num_cts == max_team_players;
}
