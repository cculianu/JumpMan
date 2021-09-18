/*!
 * \file Sprite.hh
 * \brief File containing the Sprite class Header
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 *
 * Heavily modified by Calin A. Culianu <calin.culianu@gmail.com>
 */
#pragma once

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
    Sprite(const std::string &filename, short x, short y, unsigned short width, unsigned short height, short num_images);

    /// Destructor
    virtual ~Sprite();

    /// May be called every frame to animate or otherwise have the sprite do something (Player and BasicStar reimplement)
    /// Default implementation just increments current_image_ whenever cumulative dt exceeds 1.0.
    virtual void takeAction(double dt);

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
    short getRoundedCumImageIndex() const;
    void incrCumImageIndex(double dt);

    double cum_image_dt{0.0};     /*!< If the sprite has many images, this handles them */
    short num_images_;            /*!< How many images a Sprite has */
    double x_;                    /*!< Sprite's position on the x-axis */
    double y_;                    /*!< Sprite's position on the y-acis */
    short initial_y_;             /*! Sprite's original position on the y-axis */
    const unsigned short width_;  /*!< Sprite's image's width */
    const unsigned short height_; /*!< Sprite's image's height */
    std::string filename_;        /*!< Sprite's image's filename */
};
