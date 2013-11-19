/*! Robot Class */

#ifndef __ROBORALLY_ROBOT_HH__
#define __ROBORALLY_ROBOT_HH__

class Robot: public Sprite
{
public:
    /** Move function that takes end result of the cards chosen by player
     *  and moves avatar. If end point is a hinder, function also calculates
     *  the avatars position after the hinder.
     */
    void move(int dx, int dy);
    void to_start();
    
private:
    int direction_;
    
};
#endif // __ROBORALLY_ROBOT_HH__
