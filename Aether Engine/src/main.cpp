#include "Game/Game.h"

int main() {

    //Initialize
    Game game("Aether Engine", 60);

    //Game loop
    while (game.running)
        game.Run();

    return 0;
}