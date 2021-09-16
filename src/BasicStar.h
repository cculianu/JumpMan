#pragma once

/*! 
 * \file BasicStar.hh
 * \brief File containing the BasicStar class Header
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */

#include "Sprite.h"

/*!
 * \class BasicStar
 * \brief The most basic type of star, cannot move
 */
class BasicStar : public Sprite
{
  public:

    /*!
     * \brief Constructor
     * \param y position of last BasicStar
     * \param edge_coord how many pixels we need to move to escape the screen
     */
    BasicStar(short y, int edge_coord);

    ///Destructor
    ~BasicStar();

    /*!
     * \brief The common star method for doing things 
     */
    virtual void takeAction();


  private:

    /*!
     * \brief Set the enemy's x to a random number
     * \param edge_coord how many pixels we need to move to escape the screen
     */
    void randomizeSpawn(int edge_coord);


};
