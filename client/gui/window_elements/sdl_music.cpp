#include "sdl_music.h"

SdlMusic::SdlMusic(const std::string& file): sound(nullptr) {
    sound = Mix_LoadMUS(file.c_str());
    if (sound == nullptr) {
        throw std::string("Error loading sound: ") + Mix_GetError();
    }
}
void SdlMusic::play_sound(int loops) { Mix_PlayMusic(sound, loops); }


void SdlMusic::pause_sound() { Mix_PauseMusic(); }

void SdlMusic::set_volume(float volume) { Mix_VolumeMusic(MIX_MAX_VOLUME * volume); }

SdlMusic::SdlMusic(SdlMusic&& other) noexcept: sound(other.sound) { other.sound = nullptr; }

SdlMusic& SdlMusic::operator=(SdlMusic&& other) noexcept {
    if (this == &other)
        return *this;

    sound = other.sound;
    other.sound = nullptr;

    return *this;
}

Mix_Music* SdlMusic::get_sound() { return sound; }

SdlMusic::~SdlMusic() {
    if (sound != nullptr) {
        Mix_FreeMusic(sound);
    }
}
