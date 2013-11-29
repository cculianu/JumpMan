#include "Player.hh"

Player::Player(short x, short y, unsigned short width, unsigned short height) :
  Sprite(x, y, width, height),
  dx_(0),
  dy_(0),
  standing_on_floor_(true)
{}

Player::~Player() {}

void Player::handleGravity(const signed SCREEN_WIDTH)
{
  /* dx can be 
   * -1: go left
   *  0: stand still
   *  1: go right */
  if (dx_ != 0)
    if (-(SCREEN_WIDTH -width_)/2 <= x_ + dx_ && 
        x_ + dx_ <= (SCREEN_WIDTH -width_)/2)
      x_ += dx_;

  if (dy_ >= 0 || y_ > 0)
  {
    y_ += dy_;
    dy_ -= 1;
  }
  else 
  {
    dy_ = 0;
    standing_on_floor_ = true;
  }
}

void Player::jump()
{
  if (!standing_on_floor_)
    return;

  standing_on_floor_ = false;
  this->dy_ += 20;
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
