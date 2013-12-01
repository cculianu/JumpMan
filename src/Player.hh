#ifndef __ROBORALLY_PLAYER_HH__
#define __ROBORALLY_PLAYER_HH__

/*! 
 * \file Player.hh
 * \brief File containing the Player class Header
 *
 * \author Olle Kvarnström
 * \date
 */

#include "Sprite.hh"

/*!
 * \brief Player class
 */
class Player: public Sprite
{
  public:

    /*!
     * \brief Constructor
     * \param filename Name of object's image-file inside graphics/
     * \param x starting x-position
     * \param y starting y-position
     * \param width width of image
     * \param height height of image
     */
    Player(std::string filename, short x, short y, 
           unsigned short width, unsigned short height);

    ///Destructor
    ~Player();

    /*!
     * \brief Check if player touches another Sprite class
     * \param other Sprite to check if they touch
     * \return true if they touch
     */
    bool touches(const Sprite &other);

    /*!
     * \brief Manages player's movement depending on dx and dy
     * \param SCREEN_WIDTH width of game screen
     */
    void handleGravity(const signed SCREEN_WIDTH);

    /*!
     * \brief Jump a short distance into the air
     * \param force_push if true, this is not due to the player jumping
     */
    void jump(bool force_push=false);

    /*!
     * \brief Set player to in movement on the x-axis
     * \param dx Negative for movement to the left and vice versa
     */
    void move(short dx);

  private:
    short dx_;               /*!< Current x-axis movement */
    short dy_;               /*!< Current y-axis movement */
    bool standing_on_floor_; /*!< True if player has not yet jumped */
};


#endif // __ROBORALLY_PLAYER_HH__
