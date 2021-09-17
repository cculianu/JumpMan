#pragma once

/*!
 * \file Game.hh
 * \brief File containing the Game class Header
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */
#include "Player.h"

#include <list>
#include <memory>
#include <random>
#include <string>

class AudioEngine;
class BasicStar;
class GraphicsEngine;

/*!
 * \class Game
 *
 * \brief Main game class
 *
 * Game is the main class, which acts as the core of the application
 * It's role is glue the other projects together in a structured way
 * without doing any work by itself
 */
class Game
{
public:
    /// Constructor
    Game();

    /// Disabled copy constructor
    Game(const Game &) = delete;

    /// Destructor
    ~Game();

    /// Disabled copy constructor
    void operator=(const Game &) = delete;

    /*!
     * \brief The game's main loop. Runs forever until either game exit of if there is an error.
     * \return 0 on successful exit, 1 on failure
     */
    int run();

    /// Shows a simple SDL message box with errMsg and then quits the application
    [[noreturn]] static void FatalError(const std::string &errMsg, const std::string &title = "Fatal Error");
    /// Log a warning message to console
    static void Warning(const std::string &msg);

    /// Get a random number in the range [from, to]
    static int GetRand32(int from, int to) { return GetRandGen(from, to)(); }

    class RandGen; ///< fwd decl; has operator()()
    /// Get a random number generator for the range [from, to]
    static RandGen GetRandGen(int from, int to);

private:
    /// Instance for managing graphics
    std::unique_ptr<GraphicsEngine> graphics_{};

    /// Instance for managing audio
    std::unique_ptr<AudioEngine> audio_{};

    /// List of all flying objects that the player can hit
    std::list<std::unique_ptr<BasicStar>> star_list_;

    /// Player instance
    std::unique_ptr<Player> player_;

    /*!
     * \brief handles player input
     * \return 1 if user want to quit the game
     */
    int handlePlayerInput();

    /*!
     * All action happens here
     *
     * \brief lets objects interact with each other
     * \return 1 if player has died
     */
    int letObjectsInteract();

    /*!
     * \brief draw updates to screen
     * \return 1 on graphics failure
     */
    int drawObjectsToScreen();

    /// Add stars to star_list_ until they fill up the screen
    void addStars();

    /*!
     * \brief Triggers when the player fails
     * \return always returns 2
     *
     * This basically adds a "You have lost"-text to the screen and
     * restarts the players game upon keypress. It will also
     * handle player's highscore
     */
    int gameOver();

public:
    class RandGen {
        friend class Game;
        std::mt19937 &gen;
        std::uniform_int_distribution<int> dist;
        RandGen(std::mt19937 &g, int from, int to) : gen(g), dist(from, to) {}
    public:
        int operator()() { return dist(gen); }
    };
};
