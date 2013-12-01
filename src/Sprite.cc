#include "Sprite.hh"

using namespace std;

Sprite::Sprite(string filename, short x, short y, 
               unsigned short width, unsigned short height) :
  x_(x),
  y_(y),
  width_(width),
  height_(height),
  filename_(filename)
{}

Sprite::Sprite(const Sprite &other) :
  x_(other.x_),
  y_(other.y_),
  width_(other.width_),
  height_(other.height_),
  filename_(other.filename_)
{}

Sprite::~Sprite() {}

Sprite &Sprite::operator=(const Sprite &other)
{
  if (this != &other)
  {
    Sprite clone(other);
    *this = clone;
  }

  return *this;
}

const std::string &Sprite::filename() const
{
  return this->filename_;
}

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

void Sprite::modifyY(int mod)
{
  this->y_ += mod;
}
