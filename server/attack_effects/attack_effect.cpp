#include "attack_effect.h"

#include "server/player/player.h"

AttackEffect::AttackEffect(Player& player_origin, int damage, const Vector2D& dir, int max_range):
        player_origin(player_origin), damage(damage), dir(dir), max_range(max_range) {}

Vector2D AttackEffect::get_dir() const { return dir; }

int AttackEffect::get_max_range() const { return max_range; }

bool AttackEffect::apply(const std::unique_ptr<Player>& player) {
    int distance = (player->get_pos() - player_origin.get_pos()).length();
    if (!is_target_hit(distance))
        return false;
    int final_damage = compute_damage(distance);
    player->take_damage(final_damage);
    return true;
}
