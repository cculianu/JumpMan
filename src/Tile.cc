#include "Tile.hh"

Tile::Tile(uint x, uint y, uint image_x, uint image_y, 
           tile_t tiletype, direction_t direction) :
  Sprite(x, y, image_x, image_y),
  type_(tiletype),
  direction_(direction)
{}

Tile::~Tile() {}
