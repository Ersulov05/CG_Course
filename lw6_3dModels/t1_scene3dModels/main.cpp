#include <iostream>
#include "./src/view/GameView.h"
#include "./src/model/Game.h"

int main()
{
    Game game;
    GameView gameView(game);

    gameView.Run();

    return 0;
}