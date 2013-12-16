/*!
 * \file AudioEngine.cc
 * \brief File containing the AudioEngine source code
 *
 * \author Olle Kvarnström
 * \date
 */

#include <iostream>

#include <SDL/SDL.h>

#include "AudioEngine.hh"

using namespace std;

AudioEngine::AudioEngine() :
  background_music_(NULL),
  is_playing_(false)
{
  /* Init SDL_audio - not sure what this does but is needed
   * for SDL_mixer to work */
  if (SDL_InitSubSystem(SDL_INIT_AUDIO) == -1)
  {
    cerr
      << "Failed to initialize SDL-audio: "
      << SDL_GetError() << '\n';
    exit(1);
  }

  /* Init SDL_mixer */
  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
  {
    cerr
      << "Failed to open SDL-mixer: "
      << Mix_GetError() << '\n';
    exit(1);
  }
}

AudioEngine::~AudioEngine()
{
  Mix_FreeMusic(this->background_music_);
  Mix_CloseAudio();
  SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

bool AudioEngine::loadBackgroundMusic(const string &filename)
{
  this->background_music_ = Mix_LoadMUS(filename.c_str());
  if (this->background_music_ == NULL)
  {
    std::cerr
      << "Failed to load background music: "
      << Mix_GetError() << '\n';
    return false;
  }
  return true;
}

bool AudioEngine::startPlaying()
{
  is_playing_ = true;
  return Mix_PlayMusic(background_music_, -1) == 0;
}

void AudioEngine::togglePausePlay()
{
  if (this->is_playing_)
    Mix_PauseMusic();
  else
    Mix_ResumeMusic();

  this->is_playing_ = !this->is_playing_;
}

