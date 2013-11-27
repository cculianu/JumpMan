/*! Sprite Class */

#ifndef __ROBORALLY_SPRITE_HH__
#define __ROBORALLY_SPRITE_HH__

class Sprite
{
public:
    Sprite(unsigned x, unsigned y, unsigned image_x, unsigned image_y);
    ~Sprite();

    unsigned x();
    unsigned y();

    unsigned image_x();
    unsigned image_y();


    
protected:
    unsigned x_; /**< Coordinate x on game map */
    unsigned y_; /**< Coordinate y on game map */
    unsigned image_x_; /**< Coordinate x of image on the sprite sheet */
    unsigned image_y_; /**< Coordinate y of image on the sprite sheet */

private:

};

#endif //__ROBORALLY_SPRITE_HH__
