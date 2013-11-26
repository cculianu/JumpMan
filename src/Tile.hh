/*! Tile Class */

#ifndef __ROBORALLY_TILE_HH__
#define __ROBORALLY_TILE_HH__

#include "Sprite.hh"

enum tile_t { FLAG, TURN, WALL, HOLE, ARROW, FLOOR };

enum direction_t { LEFT, RIGHT, UP, DOWN, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT };

class Tile: public Sprite
{
public:
  Tile(uint x, uint y, uint image_x, uint image_y, tile_t tiletype, direction_t direction);
  ~Tile();

private:
  /** Determines type of tile.
   *
   */
  int type_;
  int direction_;
};


#endif // __ROBORALLY_TILE_HH__
