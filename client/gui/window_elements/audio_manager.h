// AudioManager.h
#pragma once
#include <string>

#include <SDL_mixer.h>

#include "../game_config.h"

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool load();
    void playEffect(int key);
    void toggleMusic();
    void stopMusic();
    void cleanUp();

private:
    Mix_Music* menu_music;
    Mix_Music* game_music;
    Mix_Chunk* ct_win_sound;
    Mix_Chunk* tt_win_sound;
    Mix_Chunk* item_pick_sound;
    Mix_Chunk* recharge_sound;
    Mix_Chunk* hit_sound;
    Mix_Chunk* bomb_planted_sound;
    Mix_Chunk* bomb_defused_sound;
    // Mix_Chunk* bomb_exploded_sound;
    // Mix_Chunk* error_sound;
};
