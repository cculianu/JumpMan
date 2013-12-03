#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include <random>

#include "Game.hh"

#include "MovingStar.hh"

using namespace std;

Game::Game() :
  graphics_(NULL),
  star_list_()
{
  /* Initialize graphics */
  this->graphics_= new GraphicsEngine
    ( "Jumpman" /* Title */
    , 1000      /* Screen width */
    , 600       /* Screen height */
    , 32        /* Bits per pixel */
    , 24        /* Frame rate */
    );

  /* Load images from disk */
  if (this->graphics_->loadImage("background")  == false ||
      this->graphics_->loadImage("player")      == false ||
      this->graphics_->loadImage("basic_star")  == false ||
      this->graphics_->loadImage("moving_star") == false )
  {
    std::cerr 
      << "Failed to load image: " 
      << this->graphics_->getLastError() << '\n';
    exit(1);
  }
}

Game::~Game()
{
  delete this->graphics_;
  for (auto star : this->star_list_)
    delete star;
}

int Game::run()
{
  Player player;

  if (this->star_list_.size() > 0)
  {
    for (auto star : this->star_list_)
      delete star;
    this->star_list_.clear();
  }

  for (;;)
  {
    /** PART 1 - HANDLE PLAYER INPUT **/

    event_t event;
    while (this->graphics_->getEvent(event))
      switch (event)
      {
        case LEFT: player.move(-1); break;
        case RIGHT: player.move(1); break;
        case STILL: player.move(0); break;
        case UP: player.jump(); break;
        case QUIT: return 0; break;
        default: break;
      }



    /** PART 2 - OBJECTS MOVE/ITERACT **/

    //TODO: Rename handleGravity to takeAction
    player.handleGravity(static_cast<signed>(this->graphics_->screen_width()));
    for (auto star : this->star_list_)
      star->takeAction(); 

    /* Remove stars if the player touches them or they disappear off screen */
    this->star_list_.remove_if([&player](BasicStar *star) 
    { 
      if (player.touches(star) || star->y() < 0)
      {
        delete star;
        return true;
      }
      return  false;
    });

    /* Add stars if there is room */
    this->addStars();

    /* If player falls below the screen - return game over */
    if (player.y() < -player.height()*2)
      return this->gameOver();

    /* If player is above the middle of the screen, 
     * lower everything to center the player */
    const int offset_y = player.y() - this->graphics_->screen_height()/2;
    if (offset_y > 0)
    {
      player.modifyY(-offset_y);
      for (auto star : this->star_list_)
        star->modifyY(-offset_y); 
    }



    /** PART 3 - DRAW TO SCREEN **/

    /* Draw background */
    this->graphics_->drawImage("background", NULL, NULL);

    /* Draw all stars */
    for (auto star : this->star_list_)
    {
      rect_t draw_to = {star->x(), star->y(), star->width(), star->height() };
      rect_t draw_from = {star->imageX(), 0, draw_to.w, draw_to.h };
      this->graphics_->drawImage(star->filename(), &draw_from, &draw_to);
    }

    /* Draw player */
    rect_t draw_to = {player.x(), player.y(), player.width(), player.height() };
    this->graphics_->drawImage(player.filename(), NULL, &draw_to);

    /* Draw score */
    this->graphics_->drawText("Score: " + to_string(player.score()), 
                              this->graphics_->screen_width(), 100);

    /* Flush */
    if (this->graphics_->updateScreen() == false)
      return 1;
  }

  /* END NOT REACHED */
  return 1;
}

void Game::addStars()
{
  default_random_engine
    gen(chrono::system_clock::now().time_since_epoch().count());
  uniform_int_distribution<int> rand(0, 100);

  const signed screen_height = 
    static_cast<signed>(this->graphics_->screen_height());
  const signed half_screen_width = 
    this->graphics_->screen_width()/2;

  if (this->star_list_.size() <= 0)
    this->star_list_.push_back(new BasicStar(0, half_screen_width));

  while (this->star_list_.back()->initialY() < screen_height)
  {
    const short last_y = this->star_list_.back()->initialY();

    this->star_list_.push_back(new BasicStar(last_y, half_screen_width));

    if (rand(gen) % 10 == 1)
      this->star_list_.push_back(new MovingStar(last_y, half_screen_width));
  }

}

int Game::gameOver()
{
  const signed screen_width = this->graphics_->screen_width();
  const signed screen_height = this->graphics_->screen_height();

  this->graphics_->drawText("OH NO! You fell!", screen_width, screen_height);
  this->graphics_->drawText("Press any key to continue", screen_width, 
                                                         screen_height + 30);
  this->graphics_->updateScreen();
  this->graphics_->waitForKeypress();
  return 2;
}
