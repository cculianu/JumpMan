#ifndef __BASIC_ENEMY_HH__
#define __BASIC_ENEMY_HH__

#include "Sprite.hh"

class BasicEnemy : public Sprite
{
  public:
    BasicEnemy(std::string filename, const Sprite &reference,
               unsigned short width, unsigned short height);
    ~BasicEnemy();


};

#endif //__BASIC_ENEMY_HH__
