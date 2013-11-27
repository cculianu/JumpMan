#include "Sprite.hh"

Sprite::Sprite(unsigned x, unsigned y, unsigned image_x, unsigned image_y) :
  x_(x),
  y_(y),
  image_x_(image_x),
  image_y_(image_y)
{}

Sprite::~Sprite() {}

unsigned Sprite::x()
{
  return this->x_;
}

unsigned Sprite::y()
{
  return this->y_;
}

unsigned Sprite::image_x()
{
  return this->image_x_;
}

unsigned Sprite::image_y()
{
  return this->image_y_;
}
