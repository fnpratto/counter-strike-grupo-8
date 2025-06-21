#ifndef TP_FINAL_MUSIC_H
#define TP_FINAL_MUSIC_H

#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class SdlMusic {
private:
    Mix_Music* sound;

public:
    explicit SdlMusic(const std::string& file);

    void play_sound(int loops);

    void pause_sound();

    void set_volume(float volume);

    // Cant copy
    SdlMusic(const SdlMusic& other) = delete;
    SdlMusic& operator=(const SdlMusic& other) = delete;

    // Move constructors
    SdlMusic(SdlMusic&& other) noexcept;
    SdlMusic& operator=(SdlMusic&& other) noexcept;

    Mix_Music* get_sound();

    ~SdlMusic();
};
#endif  // TP_FINAL_SOUND_H
