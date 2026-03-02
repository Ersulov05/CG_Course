#include <iostream>
#include "./src/model/Game.h"
#include "./src/view/GameView.h"

int main()
{
    Game game;
    GameView gameView(game);

    gameView.Run();

    return 0;
}