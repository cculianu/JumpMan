#include <iostream>
#include <string>

using namespace std;

#include "Game.hh"

Game::Game() :
  graphics_(NULL),
  player_(NULL)
{
  /* Start graphics: */
  const string TITLE = "Jumpman";
  const unsigned SCREEN_WIDTH = 600;
  const unsigned SCREEN_HEIGHT = 600;
  const unsigned SCREEN_BPP = 32;
  const unsigned FRAME_RATE = 20;
  graphics_= new GraphicsEngine(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT,
                                SCREEN_BPP, FRAME_RATE);

  /* Load images from disk */
  if (graphics_->loadImage("player") == false ||
      graphics_->loadImage("jumpy") == false ||
      graphics_->loadImage("background") == false)
  {
    std::cerr << "Failed to load image: " << graphics_->getLastError() << '\n';
    exit(1);
  }

  /* Spawn player instance */
  const short PLAYER_START_X = 0;
  const short PLAYER_START_Y = 0;
  const unsigned short PLAYER_WIDTH = 40;
  const unsigned short PLAYER_HEIGHT = 40;
  player_ = new Player(PLAYER_START_X, PLAYER_START_Y, 
                       PLAYER_WIDTH, PLAYER_HEIGHT);
}

Game::~Game()
{
  delete player_;
  delete graphics_;
}

int Game::run()
{
  event_t event;
  rect_t dstrect = { 0, 0, player_->width(), player_->height() };


  for (;;)
  {
    graphics_->drawImage("background", NULL, NULL);

    while (graphics_->getEvent(event))
      switch (event)
      {
        case QUIT: return 0; break;
        case LEFT: player_->move(-1); break;
        case UP: player_->jump(); break;
        case RIGHT: player_->move(1); break;
        case STILL: player_->move(0); break;
        default: break;
      }

    player_->handleGravity(static_cast<signed>(graphics_->screen_width()));
    dstrect.x = player_->x();
    dstrect.y = player_->y();
    graphics_->drawImage("player", NULL, &dstrect);

    if (graphics_->updateScreen() == false)
      return 1;
  }

  /* END NOT REACHED */
  return 1;
}
