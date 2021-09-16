/*!
 * \file Game.cc
 * \brief File containing the Game class source code
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */
#include "Game.h"

#include "MovingStar.h"
#include "Highscore.h"

#include <iostream>
#include <chrono>
#include <random>

using namespace std;

Game::Game() :
  graphics_(NULL),
  audio_(NULL),
  star_list_(),
  player_()
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
  if(this->graphics_->loadImage("player")      == false ||
     this->graphics_->loadImage("basic_star")  == false ||
     this->graphics_->loadImage("moving_star") == false )
  {
    std::cerr 
      << "Failed to load image: " 
      << this->graphics_->getLastError() << '\n';
    exit(1);
  }

  /* Initialize audio */
  this->audio_ = new AudioEngine();
  this->audio_->loadBackgroundMusic("audio/cdk_silence_await.mp3");
  this->audio_->startPlaying();
}

Game::~Game()
{
  delete this->graphics_;
  delete this->audio_;
  for (auto star : this->star_list_)
    delete star;
}

int Game::run()
{
  /* Reset Player */
  player_.reset();

  /* Reset Starlist */
  if (this->star_list_.size() > 0)
  {
    for (auto star : this->star_list_)
      delete star;
    this->star_list_.clear();
  }

  /* Main loop */
  for (;;)
  {
    if (this->handlePlayerInput() == 1)
      return 0;

    if (this->letObjectsInteract() == 1)
      return this->gameOver();

    if (this->drawObjectsToScreen() == 1)
      return 1;
  }

  /* END NOT REACHED */
  return 1;
}

int Game::handlePlayerInput()
{
  event_t event = NOTHING;
  while (this->graphics_->getEvent(event))
    switch (event)
    {
      case LEFT: this->player_.move(-1); break;
      case RIGHT: this->player_.move(1); break;
      case STILL: this->player_.move(0); break;
      case UP: this->player_.jump(); break;
      case PAUSEPLAY: this->audio_->togglePausePlay(); break;
      case QUIT: return 1; break;
      default: break;
    }
  return 0;
}

int Game::letObjectsInteract()
{
  //TODO: Rename handleGravity to takeAction
  this->player_.handleGravity(
      static_cast<signed>(this->graphics_->screen_width()));
  for (auto star : this->star_list_)
    star->takeAction(); 

  /* Remove stars if the player touches them or they disappear off screen */
  for (auto it = this->star_list_.begin(); it != this->star_list_.end();)
    if (this->player_.touches(*it) || (*it)->y() < 0)
    {
      delete *it;
      it = this->star_list_.erase(it);
    }
    else ++it;

  /* Add stars if there is room */
  this->addStars();

  /* If player falls below the screen - return game over */
  if (this->player_.y() < -this->player_.height()*2)
    return 1;

  /* If player is above the middle of the screen, 
   * lower everything to center the player */
  const int offset_y = this->player_.y() - this->graphics_->screen_height()/2;
  if (offset_y > 0)
  {
    this->player_.modifyY(-offset_y);
    for (auto star : this->star_list_)
      star->modifyY(-offset_y); 
  }
  return 0;
}

int Game::drawObjectsToScreen()
{
  rect_t draw_to;
  rect_t draw_from;

  /* Draw background black */
  this->graphics_->makeScreenBlack();

  /* Draw all stars */
  for (auto star : this->star_list_)
  {
    draw_to = { star->x(), star->y(), star->width(), star->height() };
    draw_from = { star->imageX(), 0, draw_to.w, draw_to.h };
    this->graphics_->drawImage(star->filename(), &draw_from, &draw_to);
  }

  /* Draw player */
  draw_to = { this->player_.x() 
            , this->player_.y() 
            , this->player_.width() 
            , this->player_.height() 
            };
  draw_from = { this->player_.imageX()
              , this->player_.imageY()
              , this->player_.width()
              , this->player_.height() 
              };
  this->graphics_->drawImage(this->player_.filename(), &draw_from, &draw_to);

  /* Draw score */
  const string score_string = "Score: " + to_string(this->player_.score());
  this->graphics_->drawText(score_string, 20);

  /* Flush */
  if (this->graphics_->updateScreen() == false)
    return 1;

  return 0;
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

  /* Make sure there's always at least one star in the starlist 
   * This is just to avoid segfaults */
  if (this->star_list_.size() <= 0)
    this->star_list_.push_back(new BasicStar(0, half_screen_width));

  /* Make sure there's a BasicStar every 50 y-pixels,
   * Also add other types of stars if the RNG is with you */
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
  Highscore highscore(".highscore");
  bool new_highscore = highscore.add(this->player_.score());

  const signed screen_height = this->graphics_->screen_height();

  /* Header */
  this->graphics_->drawText("Highscore", 250);

  /* Draw every score from highscore */
  bool highlight_next = new_highscore;
  for (size_t i = 0; i < highscore.size(); ++i)
  {
    text_color_t text_color = YELLOW;
    string new_score = highscore.get(i);

    /* If we managed to get into the highscore, write the score in orange */
    if (highlight_next && this->player_.score() == stoul(new_score))
    {
      text_color = ORANGE;
      highlight_next = false;
      this->graphics_->drawText("New highscore!", screen_height + 150);
    }
    this->graphics_->drawText(new_score, 300 + (i*40), text_color);
  }

  /* If we managed to get into the highscore: Ask for nickname */
  if (new_highscore)
  {
    string nick;
    this->graphics_->drawText("Enter your name (1-5 letters) and press enter", 
                              screen_height + 200);
    this->graphics_->getStringFromPlayer(5, nick, screen_height + 250);
    highscore.setNickname(nick);
  }

  /* Draw message that tells player that the game is over */
  this->graphics_->drawText("Press any key to continue", screen_height + 440);
  this->graphics_->updateScreen();
  this->graphics_->waitForKeypress();
  return 2;
}
