#include "game.h"

#include <string>

#include "server/errors.h"

Game::Game(const std::string& name, const Clock& clock, const Shop& shop, Map& map): 
        name(name),
        logic(clock, shop, map) {}

GameState Game::get_game_state() const { 
    return logic.get_game_state(); 
}

void Game::tick(Message msg, const std::string& player_name) {
    chk_valid_player_name_or_fail(player_name);
    logic.update_map();
    logic.update_round_phase();
    handle_msg(msg, player_name);
}

void Game::handle_msg(Message msg, const std::string& player_name) {
    MessageType msg_type = msg.get_type();
    if (msg_type == MessageType::JOIN_GAME_CMD) {
        logic.add_player(player_name);
    } else if (msg_type == MessageType::SELECT_TEAM_CMD) {
        Team team = msg.get_content<SelectTeamCommand>().get_team();
        logic.select_team(player_name, team);
    } else if (msg_type == MessageType::START_GAME_CMD) {
        logic.start_game();
    } else if (msg_type == MessageType::BUY_GUN_CMD) {
        GunType gun = msg.get_content<BuyGunCommand>().get_gun();
        logic.buy_gun(player_name, gun);
    } else if (msg_type == MessageType::BUY_AMMO_CMD) {
        GunType gun = msg.get_content<BuyAmmoCommand>().get_gun();
        logic.buy_ammo(player_name, gun);
    } else if (msg_type == MessageType::MOVE_CMD) {
        int dx = msg.get_content<MoveCommand>().get_dx();
        int dy = msg.get_content<MoveCommand>().get_dy();
        logic.move(player_name, dx, dy);
    } else if (msg_type == MessageType::SHOOT_CMD) {
        int x = msg.get_content<ShootCommand>().get_x();
        int y = msg.get_content<ShootCommand>().get_y();
        logic.shoot(player_name, x, y);
    } else if (msg_type == MessageType::SWITCH_WEAPON_CMD) {
        WeaponSlot slot = msg.get_content<SwitchWeaponCommand>().get_slot();
        logic.switch_weapon(player_name, slot);
    } else if (msg_type == MessageType::SWITCH_WEAPON_CMD) {
        logic.reload_weapon(player_name);
    }
}

Game::~Game() {}

void Game::chk_valid_player_name_or_fail(const std::string& player_name) {
    if (player_name.empty())
        throw InvalidPlayerNameError();
}
