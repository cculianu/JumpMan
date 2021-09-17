#pragma once

/*!
 * \file AudioEngine.hh
 * \brief File containing the AudioEngine class Header
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */

#include <SDL_mixer.h>
#include <string>

/*!
 * \class AudioEngine
 * \brief Class for managing audio
 */
class AudioEngine
{
  public:

    ///Constructor
    AudioEngine();

    ///Disabled copy constructor
    AudioEngine(const AudioEngine&) = delete;

    ///Destructor
    ~AudioEngine();

    ///Disabled copy constructor
    void operator=(const AudioEngine&) = delete;

    /*!
     * \brief Loads the background music from disk
     * \param path to file
     * \return true on success
     */
    bool loadBackgroundMusic(const std::string &filename);

    /*!
     * \brief Loads the player-touching-star sound effect
     * \param path to file
     * \return true on success
     */
    bool loadStarSoundEffect(const std::string &filename);

    /*!
     * \brief Attempts to play the star sound effect
     * \return true on success
     */
    bool playStarSound();

    /*!
     * \brief Start playing background music
     * \return true on success
     */
    bool startPlaying();

    /*!
     * \brief Toggles pause/play music
     */
    void togglePausePlay();

  private:

    ///Background music
    Mix_Music *background_music_;

    ///Start sound effect
    Mix_Chunk *star_effect_;

    ///Keeps track if music is paused or playing
    bool is_playing_;
};
