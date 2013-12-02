#include <iostream>
#include <string>
#include <algorithm>

#include "Game.hh"

using namespace std;

Game::Game() :
  graphics_(NULL)
{
  /* Start graphics: */
  const string TITLE = "Jumpman";
  const unsigned SCREEN_WIDTH = 1000;
  const unsigned SCREEN_HEIGHT = 600;
  const unsigned SCREEN_BPP = 32;
  const unsigned FRAME_RATE = 24;
  graphics_= new GraphicsEngine(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT,
                                SCREEN_BPP, FRAME_RATE);

  /* Load images from disk */
  if (graphics_->loadImage("background") == false ||
      graphics_->loadImage("player") == false ||
      graphics_->loadImage("star_yellow") == false ||
      graphics_->loadImage("star_orange") == false )
  {
    std::cerr << "Failed to load image: " << graphics_->getLastError() << '\n';
    exit(1);
  }
}

Game::~Game()
{
  delete graphics_;
}

int Game::run()
{
  Player player;

  list<BasicStar> star_list;

  for (;;)
  {
    /* Receive and handle player input */
    event_t event;
    while (graphics_->getEvent(event))
      switch (event)
      {
        case QUIT: return 0; break;
        case LEFT: player.move(-1); break;
        case UP: player.jump(); break;
        case RIGHT: player.move(1); break;
        case STILL: player.move(0); break;
        default: break;
      }

    /* Let the objects move and interact with each other */
    player.handleGravity(static_cast<signed>(graphics_->screen_width()));
    star_list.remove_if([&player](BasicStar star) 
        { return player.touches(star) || star.y() < 0; });
    addStars(star_list);

    /* If player falls below the screen - return game over */
    if (player.y() < -player.height()*2)
      return gameOver();

    /* If player is above the middle of the screen, 
     * lower everything to center the player */
    const int offset_y = player.y() - graphics_->screen_height()/2;
    if (offset_y > 0)
    {
      for_each(star_list.begin(), star_list.end(), [offset_y](BasicStar &star)
          { star.modifyY(-offset_y); });
      player.modifyY(-offset_y);
    }

    /* Draw everything to screen */
    graphics_->drawImage("background", NULL, NULL);

    for (auto star = star_list.begin(); star != star_list.end(); ++star)
    {
      rect_t draw_to = {star->x(), star->y(), star->width(), star->height() };
      rect_t draw_from = {star->imageX(), 0, draw_to.w, draw_to.h };
      graphics_->drawImage(star->filename(), &draw_from, &draw_to);
    }

    rect_t draw_to = {player.x(), player.y(), player.width(), player.height() };
    graphics_->drawImage(player.filename(), NULL, &draw_to);

    graphics_->drawText("Score: " + to_string(player.score()), 
                        graphics_->screen_width(), 100);

    if (graphics_->updateScreen() == false)
      return 1;
  }

  /* END NOT REACHED */
  return 1;
}

void Game::addStars(list<BasicStar> &star_list)
{
  signed screen_height = static_cast<signed>(graphics_->screen_height());
  signed half_screen_width  = graphics_->screen_width()/2;

  if (star_list.size() <= 0)
    star_list.push_back(BasicStar(0, half_screen_width));

  while (star_list.back().y() < screen_height)
    star_list.push_back(BasicStar(star_list.back().y(), half_screen_width));

}

int Game::gameOver()
{
  graphics_->drawText("OH NO! You fell!", graphics_->screen_width(), 
                                          graphics_->screen_height());
  graphics_->drawText("Press any key to continue", graphics_->screen_width(), 
                                          graphics_->screen_height() + 30);
  graphics_->updateScreen();
  graphics_->waitForKeypress();
  return 2;
}
