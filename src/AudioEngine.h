/*!
 * \file AudioEngine.h
 * \brief File containing the AudioEngine class Header
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 *
 * Heavily modified by Calin A. Culianu <calin.culianu@gmail.com>
 */
#pragma once

#include <SDL_mixer.h>

#include <string>

/*!
 * \class AudioEngine
 * \brief Class for managing audio
 */
class AudioEngine
{
public:
    /// Constructor
    AudioEngine();

    /// Disabled copy constructor
    AudioEngine(const AudioEngine &) = delete;

    /// Destructor
    ~AudioEngine();

    /// Disabled copy constructor
    void operator=(const AudioEngine &) = delete;

    /*!
     * \brief Loads the background music from disk
     * \param path to file
     * \return true on success
     */
    bool loadBackgroundMusic(const std::string &filename);

    /*!
     * \brief Loads the player-touching-star sound effect
     * \param path to files
     * \return true on success
     */
    bool loadStarSoundEffect(const std::string &filename1, const std::string &filename2);
    /*!
     * \brief Loads the jetpack sound effect
     * \param path to file
     * \return true on success
     */
    bool loadJetpackSoundEffect(const std::string &filename);

    /*!
     * \brief Attempts to play the star sound effect
     * \return true on success
     */
    bool playStarSound(bool movingStar = false) const;
    bool playJetpackSound() const;

    /// Returns the age in milliseconds of the last time we played the jetpack sound
    unsigned lastPlayedJetpackSoundAgeMS() const;

    /*!
     * \brief Start playing background music
     * \return true on success
     */
    bool startPlayingBackgroundMusic(short volume = -1);

    /*!
     * \brief Toggles pause/play music. Returns true if this results in music being paused, false if not paused.
     */
    bool togglePausePlayBackgroundMusic();

    bool isBackgroundMusicPlaying() const { return is_playing_; }

private:
    /// Background music
    Mix_Music *background_music_{};

    /// Star sound effect
    Mix_Chunk *star_effects_[2] = {};

    /// Jetpack sound effect
    Mix_Chunk *jetpack_effect_{};

    /// The tick count at which we last played the jetpack effect
    mutable unsigned last_played_jetpack_effect_{};

    /// Keeps track if music is paused or playing
    bool is_playing_{};
};
