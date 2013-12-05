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
     * \param num_images how many images this Sprite has
     */
    Sprite(std::string filename, short x, short y, 
           unsigned short width, unsigned short height, short num_images);
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
    Sprite &operator=(const Sprite &other);

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
    virtual short imageX(); 

    /*!
     * \brief the position of the y-axis this sprite was initiated at
     */
    short initialY() const;

    /*!
     * \brief Modifies the Sprite's position of the y-axis
     * \param mod Y axis modifier
     */
    void modifyY(int mod);

protected:
    short current_image_; /*!< If the sprite has many images, this handles them */
    short num_images_;    /*!< How many images a Sprite has */
    short x_; /*!< Sprite's position on the x-axis */
    short y_; /*!< Sprite's position on the y-acis */
    short initial_y_; /*! Sprite's original position on the y-axis */
    const unsigned short width_;  /*!< Sprite's image's width */
    const unsigned short height_; /*!< Sprite's image's height */
    std::string filename_; /*!< Sprite's image's filename */

private:
};

#endif //__SPRITE_HH__
