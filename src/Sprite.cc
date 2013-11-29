#include "Sprite.hh"

Sprite::Sprite(short x, short y, unsigned short width, unsigned short height) :
  x_(x),
  y_(y),
  width_(width),
  height_(height)
{}

Sprite::~Sprite() {}

short Sprite::x() const
{
  return this->x_;
}

short Sprite::y() const
{
  return this->y_;
}

unsigned short Sprite::width() const
{
  return this->width_;
}

unsigned short Sprite::height() const
{
  return this->height_;
}
