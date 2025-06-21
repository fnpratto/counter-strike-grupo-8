// AudioManager.cpp
#include "audio_manager.h"

#include <iostream>

#include <SDL.h>

AudioManager::AudioManager():
        menu_music(nullptr),
        game_music(nullptr),
        ct_win_sound(nullptr),
        tt_win_sound(nullptr),
        item_pick_sound(nullptr),
        recharge_sound(nullptr),
        hit_sound(nullptr),
        bomb_planted_sound(nullptr),
        bomb_defused_sound(nullptr) {}

AudioManager::~AudioManager() { cleanUp(); }

bool AudioManager::load() {
    bool success = true;

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
        return false;
    }

    // Load music
    menu_music = Mix_LoadMUS(std::string(GameConfig::Paths::MENU_MUSIC_PATH).c_str());
    if (!menu_music) {
        std::cerr << "Failed to load menu music! SDL_mixer Error: " << Mix_GetError() << "\n";
        success = false;
    }

    game_music = Mix_LoadMUS(std::string(GameConfig::Paths::GAME_MUSIC_PATH).c_str());
    if (!game_music) {
        std::cerr << "Failed to load game music! SDL_mixer Error: " << Mix_GetError() << "\n";
        success = false;
    }

    // Load sound effects
    ct_win_sound = Mix_LoadWAV(std::string(GameConfig::Paths::CT_WIN_SOUND_PATH).c_str());
    if (!ct_win_sound) {
        std::cerr << "Failed to load CT win sound effect! SDL_mixer Error: " << Mix_GetError()
                  << "\n";
        success = false;
    }

    tt_win_sound = Mix_LoadWAV(std::string(GameConfig::Paths::TT_WIN_SOUND_PATH).c_str());
    if (!tt_win_sound) {
        std::cerr << "Failed to load TT win sound effect! SDL_mixer Error: " << Mix_GetError()
                  << "\n";
        success = false;
    }

    item_pick_sound = Mix_LoadWAV(std::string(GameConfig::Paths::ITEM_PICK_SOUND_PATH).c_str());
    if (!item_pick_sound) {
        std::cerr << "Failed to load item pick sound effect! SDL_mixer Error: " << Mix_GetError()
                  << "\n";
        success = false;
    }

    recharge_sound = Mix_LoadWAV(std::string(GameConfig::Paths::RECHARGE_SOUND_PATH).c_str());
    if (!recharge_sound) {
        std::cerr << "Failed to load recharge sound effect! SDL_mixer Error: " << Mix_GetError()
                  << "\n";
        success = false;
    }

    hit_sound = Mix_LoadWAV(std::string(GameConfig::Paths::HIT_SOUND_PATH).c_str());
    if (!hit_sound) {
        std::cerr << "Failed to load hit sound effect! SDL_mixer Error: " << Mix_GetError() << "\n";
        success = false;
    }

    bomb_planted_sound =
            Mix_LoadWAV(std::string(GameConfig::Paths::BOMB_PLANTED_SOUND_PATH).c_str());
    if (!bomb_planted_sound) {
        std::cerr << "Failed to load bomb planted sound effect! SDL_mixer Error: " << Mix_GetError()
                  << "\n";
        success = false;
    }

    bomb_defused_sound =
            Mix_LoadWAV(std::string(GameConfig::Paths::BOMB_DEFUSED_SOUND_PATH).c_str());
    if (!bomb_defused_sound) {
        std::cerr << "Failed to load bomb defused sound effect! SDL_mixer Error: " << Mix_GetError()
                  << "\n";
        success = false;
    }

    return success;
}

void AudioManager::playEffect(int key) {
    switch (key) {
        case SDLK_1:
            Mix_PlayChannel(-1, ct_win_sound, 0);
            break;
        case SDLK_2:
            Mix_PlayChannel(-1, tt_win_sound, 0);
            break;
        case SDLK_3:
            Mix_PlayChannel(-1, item_pick_sound, 0);
            break;
        case SDLK_4:
            Mix_PlayChannel(-1, recharge_sound, 0);
            break;
        case SDLK_5:
            Mix_PlayChannel(-1, hit_sound, 0);
            break;
        case SDLK_6:
            Mix_PlayChannel(-1, bomb_planted_sound, 0);
            break;
        case SDLK_7:
            Mix_PlayChannel(-1, bomb_defused_sound, 0);
            break;
    }
}

void AudioManager::toggleMusic() {
    if (Mix_PlayingMusic() == 0) {
        // Play the menu music
        Mix_PlayMusic(menu_music, -1);
    } else {
        // If music is playing
        if (Mix_PausedMusic() == 1) {
            Mix_ResumeMusic();
        } else {
            Mix_PauseMusic();
        }
    }
}

void AudioManager::stopMusic() { Mix_HaltMusic(); }

void AudioManager::cleanUp() {
    Mix_FreeChunk(ct_win_sound);
    Mix_FreeChunk(tt_win_sound);
    Mix_FreeChunk(item_pick_sound);
    Mix_FreeChunk(recharge_sound);
    Mix_FreeChunk(hit_sound);
    Mix_FreeChunk(bomb_planted_sound);
    Mix_FreeChunk(bomb_defused_sound);
    Mix_FreeMusic(menu_music);
    Mix_FreeMusic(game_music);

    ct_win_sound = nullptr;
    tt_win_sound = nullptr;
    item_pick_sound = nullptr;
    recharge_sound = nullptr;
    hit_sound = nullptr;
    bomb_planted_sound = nullptr;
    bomb_defused_sound = nullptr;
    menu_music = nullptr;
    game_music = nullptr;

    Mix_Quit();
}
