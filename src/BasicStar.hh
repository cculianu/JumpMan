#ifndef __BASIC_STAR_HH__
#define __BASIC_STAR_HH__

/*! 
 * \file BasicStar.hh
 * \brief File containing the BasicStar class Header
 *
 * \author Olle Kvarnström
 * \date
 */

#include "Sprite.hh"

/*!
 * \class BasicStar
 * \brief The most basic type of star, cannot move
 *
 */
class BasicStar : public Sprite
{
  public:

    /*!
     * \brief Constructor
     * \param filename Name of the object's image-file inside graphics/
     * \param reference Our starting position will be slightly above given reference
     * \param width width of image
     * \param height height of image
     * \param edge_coord the max pixels we can go in either direction without moving outside the screen
     */
    BasicStar(std::string filename, const Sprite &reference,
              unsigned short width, unsigned short height,
              int edge_coord);

    /*!
     * \brief Constructor
     * \param filename Name of the object's image-file inside graphics/
     * \param reference Our starting position will be slightly above given reference
     * \param width width of image
     * \param height height of image
     * \param edge_coor the max pixels we can go in either direction without moving outside the screen
     * \param rotations how many images this Sprite has
     */
    BasicStar(std::string filename, const Sprite &reference,
              unsigned short width, unsigned short height,
              int edge_coord, int rotations);

    ///Destructor
    ~BasicStar();

  private:

    /*!
     * \brief Set the enemy's x to a random number
     * \param reference A referense to help enemy place itself
     * \param edge_coor the max pixels we can go in either direction without moving outside the screen
     */
    void randomizeSpawn(const Sprite &reference, int edge_coord);


};

#endif //__BASIC_STAR_HH__
