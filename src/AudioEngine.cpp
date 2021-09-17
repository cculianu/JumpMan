/*!
 * \file AudioEngine.cc
 * \brief File containing the AudioEngine source code
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */

#include "AudioEngine.h"
#include "Game.h"

#include <SDL.h>

#include <array>
#include <cassert>
#include <iostream>


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
    for (auto *eff : star_effects_)
        Mix_FreeChunk(eff);
    Mix_FreeChunk(jetpack_effect_);
    Mix_FreeMusic(background_music_);
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

bool AudioEngine::loadStarSoundEffect(const std::string &filename1, const std::string &filename2)
{
    assert(std::size(star_effects_) == 2);
    bool ret = true;

    const std::string * filenames[2] = {&filename1, &filename2};
    for (int i = 0; i < 2; ++i) {
        auto * & eff = star_effects_[i];
        const auto &fn = *filenames[i];
        if (eff != nullptr) {
            Game::Warning("Star sound effect already loaded!");
            ret = false;
            continue;
        }
        eff = Mix_LoadWAV(fn.c_str());
        if (eff == nullptr) {
            Game::Warning(std::string("Failed to load star sound effect: ") + Mix_GetError());
            return false;
        }
    }

    return ret;
}

bool AudioEngine::loadJetPackSoundEffect(const std::string &filename)
{
    if (jetpack_effect_ != nullptr) {
        Game::Warning("JetPack sound effect already loaded!");
        return false;
    }

    jetpack_effect_ = Mix_LoadWAV(filename.c_str());
    if (jetpack_effect_ == nullptr) {
        Game::Warning(std::string("Failed to load jetpack sound effect: ") + Mix_GetError());
        return false;
    }

    return true;
}

bool AudioEngine::playStarSound(bool which) const { return Mix_PlayChannel(-1, star_effects_[which], 0) == 0; }
bool AudioEngine::playJetPackSound() const { return Mix_PlayChannel(-1, jetpack_effect_, 0) == 0; }

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
