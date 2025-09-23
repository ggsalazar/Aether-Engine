#include "Engine/Engine.h"

int main() {

    //Initialize
    Engine game("Aether Engine", 60);

    //Game loop
    while (game.running)
        game.Run();

    return 0;
}