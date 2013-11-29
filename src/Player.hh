/*! Player Class */

#ifndef __ROBORALLY_PLAYER_HH__
#define __ROBORALLY_PLAYER_HH__

#include "Sprite.hh"

class Player: public Sprite
{
  public:
    Player(short x, short y, unsigned short width, unsigned short height);
    ~Player();

    void handleGravity(const signed SCREEN_WIDTH);
    void jump();
    void move(short dx);

  private:
    short dx_;
    short dy_;
    bool standing_on_floor_;
};


#endif // __ROBORALLY_PLAYER_HH__
