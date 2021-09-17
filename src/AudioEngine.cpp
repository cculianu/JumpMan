/*!
 * \file AudioEngine.cc
 * \brief File containing the AudioEngine source code
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */

#include <iostream>

#include <SDL.h>

#include "AudioEngine.h"
#include "Game.h"

AudioEngine::AudioEngine()
{
    /* Init SDL_audio - not sure what this does but is needed
     * for SDL_mixer to work */
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
        Game::FatalError(SDL_GetError(), "Failed to Initialize SDL-Audio");

    /* Init SDL_mixer */
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
        Game::FatalError(Mix_GetError(), "Failed to Open SDL-Mixer");
}

AudioEngine::~AudioEngine()
{
    Mix_FreeChunk(this->star_effect_);
    Mix_FreeMusic(this->background_music_);
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

bool AudioEngine::loadBackgroundMusic(const std::string &filename)
{
    if (this->background_music_ != nullptr) {
        Game::Warning("Background music already loaded!");
        return false;
    }

    this->background_music_ = Mix_LoadMUS(filename.c_str());
    if (this->background_music_ == nullptr) {
        Game::Warning(std::string("Failed to load background music: ") + Mix_GetError());
        return false;
    }

    return true;
}

bool AudioEngine::loadStarSoundEffect(const std::string &filename)
{
    if (this->star_effect_ != nullptr) {
        Game::Warning("Star sound effect already loaded!");
        return false;
    }

    this->star_effect_ = Mix_LoadWAV(filename.c_str());
    if (this->star_effect_ == nullptr) {
        Game::Warning(std::string("Failed to load star sound effect: ") + Mix_GetError());
        return false;
    }

    return true;
}

bool AudioEngine::playStarSound() { return Mix_PlayChannel(-1, this->star_effect_, 0) == 0; }

bool AudioEngine::startPlayingBackgroundMusic(short volume)
{
    is_playing_ = true;
    if (volume > -1) Mix_VolumeMusic(volume);
    return Mix_PlayMusic(this->background_music_, -1) == 0;
}

void AudioEngine::togglePausePlayBackgroundMusic()
{
    if (this->is_playing_)
        Mix_PauseMusic();
    else
        Mix_ResumeMusic();

    this->is_playing_ = !this->is_playing_;
}
