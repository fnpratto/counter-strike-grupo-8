#include "attack_effect.h"

#include "server/player/player.h"

AttackEffect::AttackEffect(Player& player_origin, int damage, const Vector2D& dir):
        player_origin(player_origin), damage(damage), dir(dir) {}

Vector2D AttackEffect::get_dir() const { return dir; }

bool AttackEffect::apply(const std::unique_ptr<Player>& player) {
    if (!is_target_hit(player->get_pos()))
        return false;
    int distance = (player->get_pos() - player_origin.get_pos()).length();
    int final_damage = compute_damage(distance);
    player->take_damage(final_damage);
    if (player->is_dead())
        player_origin.increment_kills();
    return true;
}
