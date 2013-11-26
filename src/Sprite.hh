/*! Sprite Class */

#ifndef __ROBORALLY_SPRITE_HH__
#define __ROBORALLY_SPRITE_HH__

typedef unsigned int uint;

class Sprite
{
public:
    Sprite(uint x, uint y, uint image_x, uint image_y);
    ~Sprite();
    
protected:
    uint x_; /**< Coordinate x on game map */
    uint y_; /**< Coordinate y on game map */
    uint image_x_; /**< Coordinate x of image on the sprite sheet */
    uint image_y_; /**< Coordinate y of image on the sprite sheet */

private:

};

#endif //__ROBORALLY_SPRITE_HH__
