#include "Game.h"

int main()
{
    Game game;
    if (!game.initialize()) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}