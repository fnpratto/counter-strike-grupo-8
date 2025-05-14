#include "game.h"

#include <string>

#include "server/errors.h"

Game::Game(const std::string& name, const Clock& clock, const GameConfig& conf, const Shop& shop): 
        name(name),
        state(conf, clock),
        shop(shop) {}

GameConfig Game::get_config() { return state.get_config(); }

PhaseType Game::get_phase() { return state.get_phase(); }

void Game::tick(Message msg, const std::string& player_name) {
    chk_valid_player_name_or_fail(player_name);
    handle_msg(msg, player_name);
    state.update_round_phase();
}

void Game::handle_msg(Message msg, const std::string& player_name) {
    MessageType msg_type = msg.get_type();
    if (msg_type == MessageType::JOIN_GAME_CMD) {
        state.add_player(player_name);
    } else if (msg_type == MessageType::SELECT_TEAM_CMD) {
        Team team = msg.get_content<SelectTeamCommand>().get_team();
        state.select_team(player_name, team);
    } else if (msg_type == MessageType::START_GAME_CMD) {
        state.start_game();
    } else if (msg_type == MessageType::BUY_WEAPON_CMD) {
        WeaponType weapon = msg.get_content<BuyWeaponCommand>().get_weapon();
        state.buy_weapon(player_name, weapon, shop.get_weapon_price(weapon));
    } 
    // else if (msg_type == MessageType::MOVE_CMD) {
        
    // } else if (msg_type == MessageType::AIM_CMD) {
        
    // } else if (msg_type == MessageType::SHOOT_CMD) {
        
    // }
}

Game::~Game() {}

void Game::chk_valid_player_name_or_fail(const std::string& player_name) {
    if (player_name.empty())
        throw InvalidPlayerNameError();
}
