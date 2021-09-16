#include "Game.h"

extern "C" int main(int, char **)
{
  Game game;
  int return_value;

  do {
    return_value = game.run();
  } while (return_value == 2);

  return return_value;
}
