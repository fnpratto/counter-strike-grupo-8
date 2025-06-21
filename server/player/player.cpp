#include "player.h"

#include <utility>

#include "common/models.h"
#include "server/errors.h"
#include "statuses/idle_status.h"

#include "player_config.h"

Player::Player(Team team, CharacterType character_type, Circle hitbox):
        Logic<PlayerState, PlayerUpdate>(PlayerState(
                team, character_type, hitbox, Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f), false,
                PlayerConfig::full_health, ItemSlot::Secondary)),
        scoreboard_entry(state.get_inventory().get_money(), 0, 0, 0),
        status(std::make_unique<IdleStatus>()) {}

bool Player::is_ready() const { return state.get_ready(); }

bool Player::is_tt() const { return state.get_team() == Team::TT; }

bool Player::is_ct() const { return state.get_team() == Team::CT; }

bool Player::is_moving() const { return state.get_velocity() != Vector2D(0.0f, 0.0f); }

bool Player::is_dead() const { return state.get_health() == 0; }

bool Player::is_attacking() {
    ItemSlot slot = state.get_equipped_item();
    if (slot == ItemSlot::Melee)
        return state.get_inventory().get_knife().is_attacking();
    if (slot == ItemSlot::Primary || slot == ItemSlot::Secondary)
        return state.get_inventory().get_guns().at(slot)->is_attacking();
    return false;
}

CharacterType Player::get_character_type() const { return state.get_character_type(); }

Circle Player::get_hitbox() const { return state.get_hitbox(); }

Vector2D Player::get_move_dir() const { return state.get_velocity(); }

Inventory& Player::get_inventory() { return state.get_inventory(); }

ScoreboardEntry Player::get_scoreboard_entry() const { return scoreboard_entry; }

void Player::set_ready() { state.set_ready(true); }

void Player::take_damage(int damage) {
    int health = state.get_health();
    if (damage < health) {
        state.set_health(health - damage);
    } else {
        state.set_health(0);
        scoreboard_entry.deaths++;
    }
}

void Player::pick_gun(std::unique_ptr<Gun>&& gun) { state.get_inventory().set_gun(std::move(gun)); }

void Player::pick_bomb(Bomb&& bomb) { state.get_inventory().set_bomb(std::move(bomb)); }

void Player::select_team(Team team) { state.set_team(team); }

void Player::select_character(CharacterType character_type) {
    state.set_character_type(character_type);
}

void Player::start_moving(Vector2D velocity) { state.set_velocity(velocity); }

void Player::stop_moving() { state.set_velocity(Vector2D(0.0f, 0.0f)); }

void Player::move_to_pos(Vector2D new_pos) { state.set_pos(new_pos); }

void Player::aim(const Vector2D& direction) { state.set_aim_direction(direction); }

void Player::start_attacking_with_equipped_weapon() {
    ItemSlot slot = state.get_equipped_item();
    if (slot == ItemSlot::Melee) {
        auto& knife = state.get_inventory().get_knife();
        knife.start_attacking();
    }
    if (slot == ItemSlot::Primary || slot == ItemSlot::Secondary) {
        auto& gun = state.get_inventory().get_guns().at(slot);
        gun->start_attacking();
    }
}

void Player::stop_attacking() {
    state.get_inventory().get_knife().stop_attacking();
    for (auto& [_, gun]: state.get_inventory().get_guns()) gun->stop_attacking();
}

std::vector<AttackEffect> Player::attack(TimePoint now) {
    ItemSlot slot = state.get_equipped_item();
    if (slot == ItemSlot::Melee) {
        auto& knife = state.get_inventory().get_knife();
        return knife.attack(state.get_hitbox().center, state.get_aim_direction(), now);
    }
    if (slot == ItemSlot::Primary || slot == ItemSlot::Secondary) {
        auto& gun = state.get_inventory().get_guns().at(slot);
        return gun->attack(state.get_hitbox().center, state.get_aim_direction(), now);
    }
    return {};
}

void Player::equip_item(ItemSlot slot) {
    if (!state.get_inventory().has_item_in_slot(slot) || is_attacking())
        return;
    state.set_equipped_item(slot);
}

void Player::reload() {
    if (is_attacking())
        return;

    ItemSlot slot = state.get_equipped_item();
    if (slot != ItemSlot::Primary && slot != ItemSlot::Secondary)
        return;

    auto& gun = state.get_inventory().get_guns().at(slot);
    gun->reload();
}

void Player::add_kill() { scoreboard_entry.kills++; }

void Player::add_rewards(int score, int bonification) {
    scoreboard_entry.score += score;
    int old_money = state.get_inventory().get_money();
    state.get_inventory().set_money(old_money + bonification);
    scoreboard_entry.money = state.get_inventory().get_money();
}

std::optional<std::unique_ptr<Gun>> Player::drop_primary_weapon() {
    if (!state.get_inventory().has_item_in_slot(ItemSlot::Primary))
        return std::optional<std::unique_ptr<Gun>>();
    equip_item(ItemSlot::Melee);
    auto gun = state.get_inventory().remove_primary_weapon();
    return gun;
}

std::optional<Bomb> Player::drop_bomb() {
    if (!state.get_inventory().get_bomb().has_value())
        return std::optional<Bomb>();
    equip_item(ItemSlot::Melee);
    auto bomb = state.get_inventory().remove_bomb();
    return bomb;
}

void Player::start_planting_bomb(TimePoint now) {
    state.get_inventory().get_bomb().value().start_planting(now);
}

void Player::stop_planting_bomb(TimePoint now) {
    state.get_inventory().get_bomb().value().stop_planting(now);
}

void Player::start_defusing_bomb(Bomb& bomb, TimePoint now) { bomb.start_defusing(now); }

void Player::stop_defusing_bomb(Bomb& bomb, TimePoint now) { bomb.stop_defusing(now); }

void Player::set_status(std::unique_ptr<PlayerStatus> new_status) {
    status = std::move(new_status);
}

void Player::handle_start_moving(const Vector2D& velocity) {
    status->handle_start_moving(*this, velocity);
}

void Player::handle_stop_moving() { status->handle_stop_moving(*this); }

void Player::handle_start_attacking() { status->handle_start_attacking(*this); }

void Player::handle_switch_item(ItemSlot slot) {
    if (!state.get_inventory().has_item_in_slot(slot) || is_attacking())
        return;
    status->handle_switch_item(*this, slot);
}

void Player::handle_start_planting(TimePoint now) {
    if (!is_tt() || !state.get_inventory().get_bomb().has_value())
        return;
    status->handle_start_planting(*this, now);
}

void Player::handle_stop_planting(TimePoint now) {
    if (!is_tt() || !state.get_inventory().get_bomb().has_value())
        return;
    status->handle_stop_planting(*this, now);
}

void Player::handle_start_defusing(Bomb& bomb, TimePoint now) {
    if (!is_ct())
        return;
    status->handle_start_defusing(*this, bomb, now);
}

void Player::handle_stop_defusing(Bomb& bomb, TimePoint now) {
    if (!is_ct())
        return;
    status->handle_stop_defusing(*this, bomb, now);
}

void Player::reset() {
    status = std::make_unique<IdleStatus>();
    state.set_velocity(Vector2D(0.0f, 0.0f));
    state.set_health(PlayerConfig::full_health);
    state.set_equipped_item(ItemSlot::Melee);
    state.get_inventory().get_knife().stop_attacking();
    for (auto& [_, gun]: state.get_inventory().get_guns()) gun->stop_attacking();
}
