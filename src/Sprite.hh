/*! Sprite Class */

#ifndef __SPRITE_HH__
#define __SPRITE_HH__

#include <string>

class Sprite
{
public:
    Sprite(std::string filename, short x, short y, 
           unsigned short width, unsigned short height);
    Sprite(const Sprite &other);
    virtual ~Sprite();

    virtual Sprite &operator=(const Sprite &other);

    const std::string &filename() const;
    short x() const;
    short y() const;
    unsigned short width() const;
    unsigned short height() const;

protected:
    short x_;
    short y_;
    const unsigned short width_;
    const unsigned short height_;

private:
    const std::string filename_;
};

#endif //__SPRITE_HH__
