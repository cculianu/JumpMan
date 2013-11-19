#ifndef __ROBORALLY_GAME_HH__
#define __ROBORALLY_GAME_HH__

// INCLUDE SDL
// INCLUDE PLAYER
// INCLUDE TILE

class Game
{
  public:
    Game();
    ~Game();

    createMap();
    destroyMap();
    run();

  private:

    const size_t SCREEN_WIDTH;
    const size_t SCREEN_HEIGHT;

    SDL_Surface *screen;
    SDL_Surface *image_cards;
    SDL_Surface *image_tiles;

    Player *players;
    Tile *map;

};

#endif //__ROBORALLY_GAME_HH__
