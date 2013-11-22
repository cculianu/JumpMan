#ifndef __ROBORALLY_GAME_HH__
#define __ROBORALLY_GAME_HH__

#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

class Game
{
  public:
    /* Constructor and destructor starts/stops SDL */
    Game();
    ~Game();

    /* create/destroyMap will allocate or deallocate map_
     * which is a 2D grid consisting of Tiles */
    void createMap();
    void destroyMap();

    int run();

  private:

    const std::string CAPTION = "Roborally";
    const size_t SCREEN_WIDTH = 600;
    const size_t SCREEN_HEIGHT = 600;
    const size_t SCREEN_BPP = 32;

    SDL_Surface *screen_;
    SDL_Surface *image_cards_;
    SDL_Surface *image_tiles_;

    //Player *players_;
    //Tile **map_;

};

#endif //__ROBORALLY_GAME_HH__
