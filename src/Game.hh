#ifndef __GAME_HH__
#define __GAME_HH__

/*! 
 * \file Game.hh
 * \brief File containing the Game class Header
 *
 * \author Olle Kvarnström
 * \date
 */

#include <list>

#include "GraphicsEngine.hh"
#include "AudioEngine.hh"
#include "Player.hh"
#include "BasicStar.hh"

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

    ///Constructor
    Game();

    ///Disabled copy constructor
    Game(const Game&) = delete;

    ///Destructor
    ~Game();

    ///Disabled copy constructor
    void operator=(const Game&) = delete;

    /*!
     * \brief The game's main loop
     * \return 0 on success, 1 on failure and 2 on restart
     */
    int run();

  private:

    ///Instance for managing graphics
    GraphicsEngine *graphics_;

    ///Instance for managing audio
    AudioEngine *audio_;

    ///List of all flying objects that the player can hit
    std::list <BasicStar *> star_list_;

    ///Player instance
    Player player_;

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

    ///Add stars to star_list_ until they fill up the screen 
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

};

#endif //__GAME_HH__
