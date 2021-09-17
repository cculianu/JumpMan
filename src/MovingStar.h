#pragma once

/*!
 * \file MovingStar.hh
 * \brief File containing the MovingStar class Header
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */

#include "BasicStar.h"

/*!
 * \class MovingStar
 * \brief A BasicStar which moves around
 */
class MovingStar : public BasicStar
{
public:
    /*!
     * \brief Constructor
     * \param y position of last BasicStar
     * \param edge_coord how many pixels we need to move to escape the screen
     */
    MovingStar(short y, int edge_coord);

    /// Destructor
    ~MovingStar() override;

    /// Overloaded from BasicStar
    void takeAction(double dt) override;

private:
    short dx;
    short dy;
};
