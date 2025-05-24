#pragma once

#include <algorithm>
#include <random>

class RandomFloatGenerator {
private:
    float min;
    float max;

public:
    RandomFloatGenerator(float min, float max): min(min), max(max) {}

    float generate() const {
        std::random_device rnd_device;
        std::mt19937 gen(rnd_device());
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }
};
