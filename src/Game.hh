#ifndef __GAME_HH__
#define __GAME_HH__

#include <string>

#include "GraphicsEngine.hh"
#include "Sprite.hh"
#include "Player.hh"

class Game
{
  public:
    Game();
    ~Game();

    int run();

  private:
    GraphicsEngine *graphics_;

    /* These cannot be used: */
    Game(const Game&);
    void operator=(const Game&);

};

#endif //__GAME_HH__
