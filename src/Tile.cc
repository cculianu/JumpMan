#include "Tile.hh"

Tile::Tile(unsigned x, unsigned y, unsigned image_x, unsigned image_y, 
           tile_t tiletype, direction_t direction) :
  Sprite(x, y, image_x, image_y),
  type_(tiletype),
  direction_(direction)
{}

Tile::~Tile() {}
