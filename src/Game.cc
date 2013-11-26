
#include <iostream>

#include "Game.hh"

Game::Game() :
  screen_(NULL),
  image_cards_(NULL),
  image_tiles_(NULL)
{
  /* Init SDL-stuff */
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
  {
    std::cerr << "Failed to initialize SDL: " << SDL_GetError() << '\n';
    exit(1);
  }

  /* Create a main Surface */
  screen_ = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 
                             SCREEN_BPP, SDL_SWSURFACE);
  if (screen_ == NULL)
  {
    std::cerr << "Failed to create SDL Surface: " << SDL_GetError() << '\n';
    exit(1);
  }

  /* Load images from disk */
  SDL_Surface *tmp_image_tiles;
  SDL_Surface *tmp_image_cards;
  if ((tmp_image_tiles = IMG_Load("graphics/tiles.png")) == NULL ||
      (tmp_image_cards = IMG_Load("graphics/cards.png")) == NULL)
  {
    std::cerr << "Failed to load images: " << SDL_GetError() << '\n';
    exit(1);
  }

  /* Format images to a optimized format */
  if ((image_tiles_ = SDL_DisplayFormat(tmp_image_tiles)) == NULL ||
      (image_cards_ = SDL_DisplayFormat(tmp_image_cards)) == NULL)
  {
    std::cerr << "Failed to format images: " << SDL_GetError() << '\n';
    exit(1);
  }

  /* Set caption (the name displayed of the application) */
  SDL_WM_SetCaption(CAPTION.c_str(), CAPTION.c_str());

  /* Clean up */
  SDL_FreeSurface(tmp_image_tiles);
  SDL_FreeSurface(tmp_image_cards);

  createMap();
}

Game::~Game()
{
  destroyMap();
  /* Clean up 
   * screen_ will be autoremoved by SDL_Quit(); */
  SDL_FreeSurface(image_tiles_);
  SDL_FreeSurface(image_cards_);
  SDL_Quit();
}

void Game::createMap()
{
  const uint X_TILES = SCREEN_WIDTH / TILE_SIZE;
  const uint Y_TILES = SCREEN_HEIGHT / TILE_SIZE;

  map_ = new Tile *[X_TILES * Y_TILES ];
  for (uint x = 0; x < X_TILES; ++x)
    for (uint y = 0; y < Y_TILES; ++y)
      map_[x + y*X_TILES] = new Tile(x, y, 0, 0, FLOOR, UP);
}

void Game::destroyMap()
{
  const uint X_TILES = SCREEN_WIDTH / TILE_SIZE;
  const uint Y_TILES = SCREEN_HEIGHT / TILE_SIZE;

  if (map_ == NULL)
    return;

  for (uint x = 0; x < X_TILES; ++x)
    for (uint y = 0; y < Y_TILES; ++y)
      delete map_[x + y*X_TILES];

  delete[] map_;
  map_ = NULL;
}

int Game::run()
{
  SDL_Event event;
  while (SDL_WaitEvent(&event))
  {
    if (event.type == SDL_KEYDOWN)
    {
      /* Q to exit */
      if (event.key.keysym.sym == SDLK_q)
        return 0;
    }
    continue;
    //else if (event->type == SDL_MOUSEMOTION)
    //else if (event->type == SDL_MOUSEBUTTONDOWN)
  }

  /* END NOT REACHED */
  return 1;
}
