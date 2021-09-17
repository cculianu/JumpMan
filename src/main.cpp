#include "Game.h"

#include <SDL.h>

int main()
{
    SDL_SetMainReady(); // tell libsdl we have our own main, so that it sets things up for us
    return Game{}.run();
}
