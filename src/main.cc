
#include "Game.hh"

int main()
{
  Game game;
  int return_value;
  while ((return_value = game.run()) == 2)
      ;
  return return_value;
}
