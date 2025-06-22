// sound_manager.cpp
#include "sound_manager.h"

#include <iostream>

SoundManager::SoundManager() {}

SoundManager::~SoundManager() {
    music_tracks.clear();
    sounds.clear();
}


void SoundManager::load_sound(const std::string& name, const std::string& filepath) {
    auto sound = std::make_unique<SdlSound>(filepath);
    sound->set_volume(volume);
    sounds[name] = std::move(sound);
}

void SoundManager::play(const std::string& name) {
    if (muted)
        return;
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        it->second->play_sound();
    } else {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}

void SoundManager::load_music(const std::string& name, const std::string& filepath) {
    auto music = std::make_unique<SdlMusic>(filepath);

    music->set_volume(volume);
    music_tracks[name] = std::move(music);
}

void SoundManager::play_music(const std::string& name, int loops) {
    auto it = music_tracks.find(name);
    if (it == music_tracks.end())
        return;

    if (current_track != name) {
        Mix_HaltMusic();
        it->second->play_sound(loops);
        Mix_VolumeMusic(muted ? 0 : static_cast<int>(volume * MIX_MAX_VOLUME));
        current_track = name;
    }
}

void SoundManager::stop_music() {
    Mix_HaltMusic();
    current_track.clear();
}

void SoundManager::toggle_mute() {
    muted = !muted;
    Mix_VolumeMusic(muted ? 0 : static_cast<int>(volume * MIX_MAX_VOLUME));
    std::cout << (muted ? "Muted\n" : "Unmuted\n");
}

void SoundManager::set_volume(float vol) {
    volume = vol;
    if (!muted)
        Mix_VolumeMusic(static_cast<int>(volume * MIX_MAX_VOLUME));
}
