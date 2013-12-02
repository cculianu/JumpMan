/*! 
 * \file Sprite.hh
 * \brief File containing the Sprite class Header
 *
 * \author Olle Kvarnström
 * \date
 */

#ifndef __SPRITE_HH__
#define __SPRITE_HH__

#include <string>

/*!
 * \class Sprite
 * \brief Base class for all images
 */
class Sprite
{
public:
    
    /*!
     * \brief Constructor
     * \param filename Name of the object's image-file inside graphics/
     * \param x starting x-position
     * \param y starting y-position
     * \param width width of image
     * \param height height of image
     */
    Sprite(std::string filename, short x, short y, 
           unsigned short width, unsigned short height);

    
    /*!
     * \brief Constructor
     * \param filename Name of the object's image-file inside graphics/
     * \param x starting x-position
     * \param y starting y-position
     * \param width width of image
     * \param height height of image
     * \param rotations how many images this Sprite has
     */
    Sprite(std::string filename, short x, short y, 
           unsigned short width, unsigned short height, short rotations);
    /*!
     * \brief Copy Constructor
     * \param other Sprite to copy
     */
    Sprite(const Sprite &other);

    ///Destructor
    virtual ~Sprite();

    /*!
     * \brief Copy Constructor
     * \param other Sprite to copy
     */
    virtual Sprite &operator=(const Sprite &other);

    /*!
     * \return filename of image file
     */
    const std::string &filename() const;

    /*!
     * \return Sprite's position of the x-axis
     */
    short x() const;

    /*!
     * \return Sprite's position of the y-axis
     */
    short y() const;

    /*!
     * \return Sprite's image's width
     */
    unsigned short width() const;

    /*!
     * \return Sprite's image's height
     */
    unsigned short height() const;

    /*!
     * \return x of the image the Sprite wants to draw
     */
    short imageX(); 

    /*!
     * \brief Modifies the Sprite's position of the y-axis
     * \param mod Y axis modifier
     */
    void modifyY(int mod);

protected:
    short rotation_; /*!< If the sprite has many images, this handles them */
    short rotations_; /*!< How many images a Sprite has */
    short x_; /*!< Sprite's position on the x-axis */
    short y_; /*!< Sprite's position on the y-acis */
    const unsigned short width_;  /*!< Sprite's image's width */
    const unsigned short height_; /*!< Sprite's image's height */

private:
    const std::string filename_; /*!< Sprite's image's filename */
};

#endif //__SPRITE_HH__
