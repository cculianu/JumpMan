#ifndef __GRAPHICS_ENGINE_HH__
#define __GRAPHICS_ENGINE_HH__

#include <string>
#include <map>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

typedef SDL_Rect rect_t;

enum event_t { LEFT, RIGHT, UP, STILL, NOTHING, QUIT };

class GraphicsEngine
{
  public:
    GraphicsEngine(const std::string &title,
                   const unsigned screen_width,
                   const unsigned screen_height,
                   const unsigned screen_bpp,
                   const unsigned frame_rate);
    ~GraphicsEngine();

    /** loadImage
     * filename:  name of the file inside graphics/ folder
     * (example: to load graphics/player.png, filename should be "player")
     * returns true on success, failse of error */
    bool loadImage(const std::string &filename);

    /** getLastError
     * returns a string describing last error that occurred */
    std::string getLastError() const;

    /** drawImage
     * image: filename of image to draw
     * srcrect: part to draw from
     * dstrect: part to draw to 
     * returns true on success, false on error */
    bool drawImage(const std::string &image, rect_t *srcrect, 
                   rect_t *dstrect);

    /** updateScreen
     * Flushes the screen to it's visible to the user 
     * return true on success, false on error */
    bool updateScreen();

    /** getEvent
     * Sets event as event
     * Returns true if there are more pending events */
    bool getEvent(event_t &event) const;

    unsigned screen_width() const;
    unsigned screen_height() const;


  private:
    const std::string TITLE;
    const unsigned SCREEN_WIDTH = 600;
    const unsigned SCREEN_HEIGHT = 600;
    const unsigned SCREEN_BPP = 32;
    const unsigned short FRAME_RATE = 20;

    std::map<std::string, SDL_Surface *> images_;

    size_t time_of_last_refresh_;

    SDL_Surface *screen_;


    /* Just to get rid of compiler warnings: */
    GraphicsEngine(const GraphicsEngine&);
    void operator=(const GraphicsEngine&);
};

#endif //__GRAPHICS_ENGINE_HH__
