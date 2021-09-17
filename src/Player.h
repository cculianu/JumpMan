#pragma once

/*!
 * \file Player.hh
 * \brief File containing the Player class Header
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */
#include "Sprite.h"
#include <cstdint>

/*!
 * \brief Player class
 */
class Player : public Sprite
{
public:
    /// Constructor
    Player(unsigned screen_width);

    /// Destructor
    ~Player() override;

    /*!
     * \brief resets player to starting position
     */
    void reset();

    /*!
     * \brief Check if player touches another Sprite class
     * \param other Sprite to check if they touch
     * \return true if they touch
     */
    bool touches(Sprite *other);

    /*!
     * \brief Manages player's movement depending on dx and dy
     */
    void takeAction(double dt) override;

    /*!
     * \brief Jump a short distance into the air
     * \param force_push if true, this is not due to the player jumping
     */
    void jump(bool force_push = false);

    /*!
     * \brief Set player to in movement on the x-axis
     * \param dx -1 to move left, +1 to move right, 0 to stay still
     */
    void move(short dx);

    /*!
     * \return the current player score
     */
    size_t score() const;

    /*!
     * Player image is a bit special so it has its own imageX()
     * \return x of the image the Sprite wants to draw
     */
    short imageX() override;

    /*!
     * \returns y of the image the Sprite wants to draw
     */
    short imageY() const;

private:
    const unsigned screen_width;  /*!< X-axis width of play area */
    double dx_ = 0.;              /*!< Current x-axis movement */
    double dy_ = 0.;              /*!< Current y-axis movement */
    bool standing_on_floor_;      /*!< True if player has not yet jumped */
    size_t score_;                /*!< Current player score */
    bool facing_direction_;       /*!< Direction the player is facing, false = right */
};
