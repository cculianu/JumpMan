#ifndef __BASIC_ENEMY_HH__
#define __BASIC_ENEMY_HH__

/*! 
 * \file BasicEnemy.hh
 * \brief File containing the BasicEnemy class Header
 *
 * \author Olle Kvarnström
 * \date
 */

#include "Sprite.hh"

/*!
 * \class BasicEnemy
 * \brief The most basic type of enemy, cannot move
 *
 */
class BasicEnemy : public Sprite
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
    BasicEnemy(std::string filename, const Sprite &reference,
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
    BasicEnemy(std::string filename, const Sprite &reference,
               unsigned short width, unsigned short height,
               int edge_coord, int rotations);

    ///Destructor
    ~BasicEnemy();

  private:

    /*!
     * \brief Set the enemy's x to a random number
     * \param reference A referense to help enemy place itself
     * \param edge_coor the max pixels we can go in either direction without moving outside the screen
     */
    void randomizeSpawn(const Sprite &reference, int edge_coord);


};

#endif //__BASIC_ENEMY_HH__
