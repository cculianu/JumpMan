#ifndef __AUDIO_ENGINE_HH__
#define __AUDIO_ENGINE_HH__

/*!
 * \file AudioEngine.hh
 * \brief File containing the AudioEngine class Header
 *
 * \author Olle Kvarnström
 * \date
 */

#include <SDL/SDL_mixer.h>

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
     * \brief Start playing background music
     * \return true on success
     */
    bool startPlaying();

  private:

    ///Background music
    Mix_Music *background_music_;
};

#endif //__AUDIO_ENGINE_HH__
