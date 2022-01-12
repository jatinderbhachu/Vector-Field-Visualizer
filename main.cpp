#include <Game.h>


/*
 * Author: Jatinder Bhachu
 */

int main(int argc, char* args[])
{
    Game* game = new Game();

    game->run();

    delete game;
    return 0;
}
