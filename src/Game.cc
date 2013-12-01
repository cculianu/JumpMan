#include <iostream>
#include <string>
#include <list>

#include "Game.hh"

#include "BasicEnemy.hh"

using namespace std;

Game::Game() :
  graphics_(NULL)
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

}

Game::~Game()
{
  delete graphics_;
}

int Game::run()
{
  Player player("player", 0, 0, 40, 40);

  list<BasicEnemy> enemies;
  size_t current_highest_enemy;
  enemies.push_back(BasicEnemy("jumpy", player, 40, 40));
  do
  {
    enemies.push_back(BasicEnemy("jumpy", enemies.back(), 40, 40));
    current_highest_enemy = enemies.back().y();
  }
  while (current_highest_enemy < graphics_->screen_height());

  rect_t draw_to;

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
    enemies.remove_if([&player](BasicEnemy enemy) { return player.touches(enemy); });

    /* Draw everything to screen */
    graphics_->drawImage("background", NULL, NULL);

    for (auto enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
    {
      draw_to = {enemy->x(), enemy->y(), enemy->width(), enemy->height()};
      graphics_->drawImage(enemy->filename(), NULL, &draw_to);
    }

    draw_to = {player.x(), player.y(), player.width(), player.height()};
    graphics_->drawImage(player.filename(), NULL, &draw_to);

    if (graphics_->updateScreen() == false)
      return 1;
  }

  /* END NOT REACHED */
  return 1;
}
