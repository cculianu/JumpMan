#include <cmath>

#include "Player.hh"

using namespace std;

Player::Player(string filename, short x, short y, 
               unsigned short width, unsigned short height) :
  Sprite(filename, x, y, width, height),
  dx_(0),
  dy_(0),
  standing_on_floor_(true),
  score_(0)
{}

Player::~Player() {}

bool Player::touches(const Sprite &other)
{
  if (sqrt(pow(x_ - other.x(), 2) + pow(y_ - other.y(), 2)) <= 20)
  {
    ++score_;
    jump(true);
    return true;
  }
  return false;
}

void Player::handleGravity(const signed SCREEN_WIDTH)
{
  /* dx can be 
   * -1: go left
   *  0: stand still
   *  1: go right */
  if (dx_ != 0 &&
      -(SCREEN_WIDTH -width_)/2 <= x_ + dx_ && 
      x_ + dx_ <= (SCREEN_WIDTH -width_)/2)
        x_ += dx_;

  if (dy_ != 0 || y_ > 0)
  {
    y_ += dy_;
    dy_ -= 1;
  }
}

void Player::jump(bool force_push)
{
  if (force_push || standing_on_floor_)
  {
    standing_on_floor_ = false;
    if (dy_ > 10)
      dy_ += 10;
    else
      dy_ = 20;
  }
}

void Player::move(short dx)
{
  if (dx > 0)
    dx_ = 10;
  else if (dx < 0)
    dx_ = -10;
  else
    dx_ = 0;
}

size_t Player::score() const
{
  return this->score_;
}
