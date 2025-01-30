#include "Game.h"

int main() {

    //Initialize
    Game game("Aether Engine", 1920, 1080, 60);

    //Game loop
    while (game.window.isOpen()) {
        //Handle events
        game.HandleEvents();
        
        //Process Input
        game.ProcessInput();

        //Update game state
        game.Update();

        //Draw the game
        game.Render();
    }

    return 0;
}