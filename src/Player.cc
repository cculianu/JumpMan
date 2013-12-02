#include "Player.hh"

using namespace std;

Player::Player() :
  Sprite("player", 0, 0, 20, 20, 1),
  dx_(0),
  dy_(0),
  standing_on_floor_(true),
  score_(0)
{}

Player::~Player() {}

bool Player::touches(const Sprite &other)
{
  /* If y-difference is less than their combines height */
  if (abs(y_ - other.y()) < (height_ + other.height()/2))
  {
    /* If x-difference is less than their combined width */
    if (abs(x_ - other.x()) < (width_ + other.width())/2)
    {
      ++score_;
      jump(true);
      return true;
    }
  }
  return false;
}

void Player::handleGravity(const signed SCREEN_WIDTH)
{
  /* X-axis - Make sure that the player does not escape the screen */
  if (dx_ != 0 &&
      -(SCREEN_WIDTH -width_)/2 <= x_ + dx_ && 
      x_ + dx_ <= (SCREEN_WIDTH -width_)/2)
        x_ += dx_;

  /* Y-axis */
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
