/*!
 * \file Highscore.h
 * \brief File containing the Highscore class Header
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 *
 * Heavily modified by Calin A. Culianu <calin.culianu@gmail.com>
 */
#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <optional>
#include <utility>

/*!
 * \class Highscore
 *
 * \brief Class managing the game's highscore
 */

class Highscore
{
public:
    /// Constructor
    Highscore(const std::string &filename);

    /// Disabled copy constructor
    Highscore(const Highscore &) = delete;

    /// Destructor
    ~Highscore();

    /// Disabled copy constructor
    void operator=(const Highscore &) = delete;

    /*!
     * Returns the score at the nth position
     * \param n the score's position, top scorer would be n:0
     * \return The pair of score, playername
     */
    std::pair<size_t, std::string> get(unsigned n) const;

    /*!
     * \brief Adds score to highscore if applicable
     * \param new_score The score to add to highscore
     * \return true if value was added
     */
    bool add(size_t new_score, size_t *new_idx = nullptr);

    /*!
     * \brief Set nickname of new highscore
     * \param nickname new nickname
     */
    void setNickname(const std::string &nickname, std::optional<size_t> indexHint = {});

    /*!
     * \return the numbers of elements in highscore
     */
    size_t size() const;

private:
    /*!
     * \brief Reads the highscore-file to a given string
     * \param highscore_string the string to overwrite
     * \return length of the file after reading
     */
    int readFileToString(std::string &highscore_string) const;

    /*!
     * \brief Reads a highscore-string to highscore array
     * \param highscore_string String to read to array
     */
    void readStringToArray(const std::string &highscore_string);

    /// Save high scores to the .highscore file
    void save() const;

    const std::string filename_;
    std::array<std::pair<size_t, std::string>, 10> highscore_;
};
