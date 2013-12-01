/*! Player Class */

#ifndef __ROBORALLY_PLAYER_HH__
#define __ROBORALLY_PLAYER_HH__

#include "Sprite.hh"

class Player: public Sprite
{
  public:
    Player(std::string filename, short x, short y, 
           unsigned short width, unsigned short height);
    ~Player();

    bool touches(const Sprite &other);

    void handleGravity(const signed SCREEN_WIDTH);
    void jump(bool force_push=false);
    void move(short dx);

  private:
    short dx_;
    short dy_;
    bool standing_on_floor_;
};


#endif // __ROBORALLY_PLAYER_HH__