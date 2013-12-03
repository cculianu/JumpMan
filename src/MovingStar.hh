#ifndef __MOVING_STAR_HH__
#define __MOVING_STAR_HH__

/*! 
 * \file MovingStar.hh
 * \brief File containing the MovingStar class Header
 *
 * \author Olle Kvarnström
 * \date
 */

#include "BasicStar.hh"

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

    ///Destructor
    ~MovingStar();

    ///Overloaded from BasicStar
    void takeAction();

  private:
    short dx_;
    short dy_;


};

#endif //__MOVING_STAR_HH__
