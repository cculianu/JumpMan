/*! Sprite Class */

#ifndef __ROBORALLY_SPRITE_HH__
#define __ROBORALLY_SPRITE_HH__

class Sprite
{
public:
    /** Draws graphics.
    *
    */
    
    Sprite();
    ~Sprite();
    
    /** Creates picture
     *
     */
    void draw();
    
    /** Takes away picture
     *
     */
    void undraw();
   
protected:
    int x; /**< Coordinate x on game map */
    int y; /**< Coordinate y on game map */
    std::string imagename; /**< Name of image to be used */
    int image_x; /**< Coordinate x of image on the sprite sheet */
    int image_y; /**< Coordinate y of image on the sprite sheet */

private:

};

#endif //__ROBORALLY_SPRITE_HH__