#include <iostream>
#include "./src/view/GameView.h"
#include "./src/model/Game.h"
#include "./src/controller/GameController.h"

int main()
{
    Game game;
    GameController gameController(game);
    GameView gameView(gameController);

    gameView.Run();

    return 0;
}