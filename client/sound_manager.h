#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <map>
#include <memory>
#include <string>
#include <utility>

#include "gui/window_elements/sdl_music.h"
#include "gui/window_elements/sdl_sound.h"

class SoundManager {
private:
    std::map<std::string, std::unique_ptr<SdlSound>> sounds;
    std::map<std::string, std::unique_ptr<SdlMusic>> music_tracks;
    bool muted = false;
    float volume = 1.0f;
    std::string current_track;

public:
    SoundManager();
    ~SoundManager();
    void load_sound(const std::string& name, const std::string& filepath);
    void load_music(const std::string& name, const std::string& filepath);
    void play_music(const std::string& name, int loops = -1);
    void stop_music();
    void play(const std::string& name);
    void set_volume(float v);
    void toggle_mute();
};

#endif  // SOUND_MANAGER_H
