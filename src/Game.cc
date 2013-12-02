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
  if (graphics_->loadImage("player") == false ||
      graphics_->loadImage("jumpy") == false ||
      graphics_->loadImage("background") == false)
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
  Player player("player", 0, 0, 20, 20);

  list<BasicEnemy> enemies;
  enemies.push_back(BasicEnemy("jumpy", player, 20, 20, graphics_->screen_width()/2, 4));

  rect_t draw_to;
  rect_t draw_from;

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
    enemies.remove_if([&player](BasicEnemy enemy) 
        { return player.touches(enemy) || enemy.y() < 0; });
    addEnemies(enemies);

    /* If player is above the middle of the screen, 
     * lower everything to center the player */
    int offset_y = player.y() - graphics_->screen_height()/2;
    if (offset_y > 0)
    {
      player.modifyY(-offset_y);
      for_each(enemies.begin(), enemies.end(), [offset_y](BasicEnemy &enemy)
          { enemy.modifyY(-offset_y); });
    }

    /* If player falls below the screen - return game over */
    if (player.y() < 0)
      return gameOver();

    /* Draw everything to screen */
    graphics_->drawImage("background", NULL, NULL);

    for (auto enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
    {
      draw_from = {enemy->imageX(), 0, enemy->width(), enemy->height() };
      draw_to = {enemy->x(), enemy->y(), enemy->width(), enemy->height()};
      graphics_->drawImage(enemy->filename(), &draw_from, &draw_to);
    }

    draw_to = {player.x(), player.y(), player.width(), player.height()};
    graphics_->drawImage(player.filename(), NULL, &draw_to);

    graphics_->drawText("Score: " + to_string(player.score()), 
                        graphics_->screen_width(), 100);

    if (graphics_->updateScreen() == false)
      return 1;
  }

  /* END NOT REACHED */
  return 1;
}

int Game::addEnemies(list<BasicEnemy> &enemies)
{
  int counter = 0;
  signed screen_height = static_cast<signed>(graphics_->screen_height());

  while (enemies.back().y() < screen_height)
  {
    enemies.push_back(BasicEnemy("jumpy", enemies.back(), 20, 20,
                      graphics_->screen_width()/2, 4));
    ++counter;
  }
  return counter;
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
