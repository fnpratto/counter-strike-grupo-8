#include "weapon.h"

#include "common/utils/random_float_generator.h"

bool Weapon::can_attack(const float attack_rate, TimePoint now) {
    std::chrono::duration<float> time_since_last_attack = now - time_last_attack;
    float min_time_between_attacks = 1.0f / attack_rate;
    return time_since_last_attack.count() >= min_time_between_attacks;
}

int Weapon::get_random_damage(const int min_dam, const int max_dam) {
    RandomFloatGenerator rfg(min_dam, max_dam);
    return rfg.generate();
}
