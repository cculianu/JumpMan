#ifndef __HIGHSCORE_HH__
#define __HIGHSCORE_HH__

/*!
 * \file Highscore.hh
 * \brief File containing the Highscore class Header
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */

/*!
 * \class Highscore
 *
 * \brief Class managing the game's highscore
 */

class Highscore
{
  public:

    ///Constructor
    Highscore(const std::string &filename);

    ///Disabled copy constructor
    Highscore(const Highscore&) = delete;

    ///Destructor
    ~Highscore();

    ///Disabled copy constructor
    void operator=(const Highscore&) = delete;

    /*!
     * Returns the score at the nth position
     * \param n the score's position, top scorer would be n:0
     * \return value of the score as string
     */
    std::string get(unsigned n) const;

    /*!
     * \brief Adds score to highscore if applicable
     * \param new_score The score to add to highscore
     * \return true if value was added
     */
    bool add(size_t new_score);

    /*!
     * \brief Set nickname of new highscore
     * \param nickname new nickname
     */
    void setNickname(const std::string &nickname);

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

    const std::string filename_;
    std::pair<size_t, std::string> highscore_[10];

};

#endif //__HIGHSCORE_HH__
