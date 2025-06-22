#include "sdl_sound.h"

#include <iostream>


SdlSound::SdlSound(const std::string& file): sound(Mix_LoadWAV(file.c_str())) {
    if (!sound) {
        std::cerr << "Failed to load sound: " << file << " - " << Mix_GetError() << std::endl;
        return;
    }
}

void SdlSound::play_sound() { Mix_PlayChannel(-1, sound, 0); }

void SdlSound::set_volume(float volume) { Mix_VolumeChunk(sound, MIX_MAX_VOLUME * volume); }

SdlSound::SdlSound(SdlSound&& other) noexcept: sound(other.sound) { other.sound = nullptr; }

SdlSound& SdlSound::operator=(SdlSound&& other) noexcept {
    if (this == &other)
        return *this;

    sound = other.sound;
    other.sound = nullptr;

    return *this;
}

SdlSound::~SdlSound() {
    if (sound != nullptr) {
        Mix_FreeChunk(sound);
    }
}
