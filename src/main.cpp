#include "Game.h"

#include <SDL.h>

int main()
{
    SDL_SetMainReady(); // tell libsdl we have our own main, so that it sets things up for us
    Game game;
    int return_value;

    do {
        return_value = game.run();
    } while (return_value == 2);

    return return_value;
}
