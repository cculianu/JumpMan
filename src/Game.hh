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

    ///Destructor
    ~Game();

    /*!
     * \brief The game's main loop
     * \return 0 on success, 1 on failure and 2 on restart
     */
    int run();

  private:

    ///Add stars to star_list_ until they fill up the screen 
    void addStars();

    /*!
     * \brief Triggers when the player fails
     * \return always returns 2 
     */
    int gameOver();

    ///Instance for managing graphics
    GraphicsEngine *graphics_;

    ///List of all flying objects that the player can hit
    std::list <BasicStar *> star_list_;

    ///Disabled copy constructor
    Game(const Game&);

    ///Disabled copy constructor
    void operator=(const Game&);

};

#endif //__GAME_HH__
