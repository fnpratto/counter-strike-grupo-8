#ifndef TP_FINAL_SOUND_H
#define TP_FINAL_SOUND_H

#include <string>

#include <SDL_mixer.h>

class SdlSound {
private:
    Mix_Chunk* sound;

public:
    explicit SdlSound(const std::string& file);

    void play_sound();

    void set_volume(float volume);

    // Cant copy
    SdlSound(const SdlSound& other) = delete;
    SdlSound& operator=(const SdlSound& other) = delete;

    // Move constructors
    SdlSound(SdlSound&& other) noexcept;
    SdlSound& operator=(SdlSound&& other) noexcept;

    virtual ~SdlSound();
};

#endif  // TP_FINAL_SOUND_H
