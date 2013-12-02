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
#include "BasicEnemy.hh"

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

    /*!
     * \brief Add enemies until they fill up the screen 
     * \param enemies list of enemies in run()-function
     * \return numbers of enemies added
     */
    int addEnemies(std::list<BasicEnemy> &enemies);

    /*!
     * \brief Triggers when the player fails
     * \return always returns 2 
     */
    int gameOver();

    ///Instance for managing graphics
    GraphicsEngine *graphics_;

    ///Disabled copy constructor
    Game(const Game&);

    ///Disabled copy constructor
    void operator=(const Game&);

};

#endif //__GAME_HH__
