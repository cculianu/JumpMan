#include "Player.hh"

using namespace std;

Player::Player() :
  Sprite("player", 0, 0, 20, 40, 1),
  dx_(0),
  dy_(0),
  standing_on_floor_(true),
  score_(0),
  facing_direction_(true)
{}

Player::~Player() {}

void Player::reset()
{
  this->x_ = 0;
  this->y_ = 0;
  this->dx_ = 0;
  this->dy_ = 0;
  this->standing_on_floor_ = true;
  this->score_ = 0;
  this->facing_direction_ = true;
}

bool Player::touches(Sprite *other)
{
  /* If y-difference is less than their combines height */
  if (abs(this->y_ - other->y()) < (this->height_ + other->height()/2))
  {
    /* If x-difference is less than their combined width */
    if (abs(this->x_ - other->x()) < (this->width_ + other->width())/2)
    {
      this->jump(true);
      return true;
    }
  }
  return false;
}

void Player::handleGravity(const signed screen_width)
{
  /* X-axis - Make sure that the player does not escape the screen */
  if (this->dx_ != 0 &&
      this->x_ + this->dx_ >= -(screen_width - this->width_)/2 &&
      this->x_ + this->dx_ <=  (screen_width - this->width_)/2)
        this->x_ += this->dx_;

  /* Y-axis */
  if (this->dy_ != 0 || this->y_ > 0)
  {
    this->y_ += this->dy_;
    this->dy_ -= 1;

    /* If we gain height, gain score */
    if (this->dy_ > 0)
      this->score_ += this->dy_;
  }
}

void Player::jump(bool force_push)
{
  if (force_push)
  {
    /* Set speed to at least 20, after that, add 10 */
    if (this->dy_ < 10)
      this->dy_ = 10;
    this->dy_ += 10;
  }

  else if (this->standing_on_floor_)
  {
    /* If we manually jump, remove standing_on_floor_ and recurse */
    this->standing_on_floor_ = false;
    return this->jump(true);
  }
}

void Player::move(short dx)
{
  if (dx > 0)
  {
    this->dx_ = 10;
    this->facing_direction_ = true;
  }
  else if (dx < 0)
  {
    this->dx_ = -10;
    this->facing_direction_ = false;
  }
  else
    this->dx_ = 0;
}

size_t Player::score() const
{
  return this->score_ / 10;
}

short Player::imageX()
{
  /* If standing on ground, the walking images are 3-4 */
  if (this->standing_on_floor_)
    return ((++this->current_image_ % 2) + 3) * this->width_;

  /* If player is gaining speed, use images 1-2 */
  else if (this->dy_ > 0)
    return ((++this->current_image_ % 2) + 1) * this->width_;

  else return 0;
}

short Player::imageY()
{
  return this->facing_direction_ * this->height_;
}
