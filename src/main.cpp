#include "Engine/Core.h"

int main() {
    //Initialize
    Core game("Aether Engine", 60);

    //Game loop
    while (game.running)
        game.Run();

    return 0;
}