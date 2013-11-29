/*! Sprite Class */

#ifndef __SPRITE_HH__
#define __SPRITE_HH__

class Sprite
{
public:
    Sprite(short x, short y, unsigned short width, unsigned short height);
    virtual ~Sprite();

    short x() const;
    short y() const;
    unsigned short width() const;
    unsigned short height() const;

    
protected:
    short x_;
    short y_;
    unsigned short width_;
    unsigned short height_;


private:
    Sprite(const Sprite&);
    void operator=(const Sprite&);

};

#endif //__SPRITE_HH__
