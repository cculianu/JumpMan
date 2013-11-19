/*! Tile Class */

#ifndef __ROBORALLY_TILE_HH__
#define __ROBORALLY_TILE_HH__

enum
{
    FLAG, TURN, WALL, HOLE, ARROW, FLOOR
    
};

enum
{
    LEFT, RIGHT, UP, DOWN, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT
    
};

class Tile: public Sprite
{
public:


private:
    /** Determines type of tile.
     *
     */
    int type_;
    int direction_;
};


#endif // __ROBORALLY_TILE_HH__
