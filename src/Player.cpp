/*!
 * \file Player.cc
 * \brief File containing the Player class source
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */

#include "Player.h"

#include <cmath>


Player::Player(unsigned sw)
    : Sprite("player", 0, 0, 20, 40, 1), screen_width(sw)
{
    reset();
}

Player::~Player() {}

void Player::reset()
{
    this->x_ = 0.0;
    this->y_ = 0.0;
    this->dx_ = 0.0;
    this->dy_ = 0.0;
    this->standing_on_floor_ = true;
    this->score_ = 0;
    this->facing_direction_ = true;
}

bool Player::touches(Sprite *other)
{
    /* If y-difference is less than their combines height */
    if (abs(this->y_ - other->y()) < (this->height_ + other->height() / 2)) {
        /* If x-difference is less than their combined width */
        if (abs(this->x_ - other->x()) < (this->width_ + other->width()) / 2) {
            this->jump(true);
            return true;
        }
    }
    return false;
}

void Player::takeAction(double dt)
{
    /* X-axis - Make sure that the player does not escape the screen
     * NB that x:0 is in the middle of the screen */
    const double NEW_X = this->x_ + this->dx_ * dt;
    const signed BORDER_X = (int(screen_width) - this->width_) / 2;

    if (this->dx_ != 0.0 && NEW_X >= -BORDER_X && NEW_X <= BORDER_X)
        this->x_ = NEW_X;

    /* Y-axis */
    if (this->dy_ != 0.0 || this->y_ > 0.0) {
        this->y_ += this->dy_ * dt;
        this->dy_ -= 1.0 * dt;

        /* If we gain height, gain score */
        if (this->dy_ > 0.0)
            this->score_ += std::round(this->dy_ * dt);
    }

    // if player moving up or on the floor, increment internal image index
    if (dy_ > 0.0 || (standing_on_floor_ && dx_ != 0.0))
        incrCumImageIndex(dt / 10.0);
}

void Player::jump(bool force_push)
{
    if (force_push) {
        /* Set speed to at least 20, after that, add 10 */
        if (this->dy_ < 10.0)
            this->dy_ = 10.0;
        this->dy_ += 10.0;
    }

    else if (this->standing_on_floor_) {
        /* If we manually jump, remove standing_on_floor_ and recurse */
        this->standing_on_floor_ = false;
        return this->jump(true);
    }
}

void Player::move(short dx)
{
    if (dx > 0.0) {
        this->dx_ = 10.0;
        this->facing_direction_ = true;
    } else if (dx < 0.0) {
        this->dx_ = -10.0;
        this->facing_direction_ = false;
    } else
        this->dx_ = 0.0;
}

size_t Player::score() const { return this->score_ / 10; }

short Player::imageX()
{
    /* If standing on ground and moving, the walking images are 3-4 */
    if (this->standing_on_floor_ and this->dx_ != 0.0)
        return ((getRoundedCumImageIndex() % 2) + 3) * this->width_;

    /* If player is gaining speed, use images 1-2 */
    else if (this->dy_ > 0.0)
        return ((getRoundedCumImageIndex() % 2) + 1) * this->width_;

    else
        return 0;
}

short Player::imageY() const { return this->facing_direction_ * this->height_; }
