#include <iostream>
#include "./src/view/Canvas/Canvas.h"
#include "./src/view/Canvas/ICanvas.h"
#include "./src/model/Game.h"
#include "./src/view/GameView.h"

const unsigned WIDTH = 800;
const unsigned HEIGHT = 600;
const Color WHITE_COLOR = Color(0xFFFFFFFF);

int main()
{
    Canvas canvas(WIDTH, HEIGHT);
    Game game;
    GameView gameView(game);

    auto &keyboardController = canvas.GetKeyboardController();

    keyboardController.OnKeyPressedSubscribe(
        &canvas, GLFW_KEY_UP,
        [&game]()
        {
            game.GetSpaceshipModel().EngineOn();
        });

    keyboardController.OnKeyReleasedSubscribe(
        &canvas, GLFW_KEY_UP,
        [&game]()
        {
            game.GetSpaceshipModel().EngineOff();
        });

    keyboardController.OnKeyPressedSubscribe(
        &canvas, GLFW_KEY_SPACE,
        [&game]()
        {
            game.GetSpaceshipModel().Shoot();
        });

    keyboardController.OnKeyHoldSubscribe(
        &canvas, GLFW_KEY_LEFT,
        [&game](float deltatime)
        {
            game.GetSpaceshipModel().LeftRotate(deltatime);
        });

    keyboardController.OnKeyHoldSubscribe(
        &canvas, GLFW_KEY_RIGHT,
        [&game](float deltatime)
        {
            game.GetSpaceshipModel().RightRotate(deltatime);
        });

    canvas.RunWindow(
        [&gameView, &game](ICanvas &canvas, float deltatime)
        {
            static float fpsTimer = 0.0f;
            static int frameCount = 0;
            static int currentFPS = 0;

            frameCount++;
            fpsTimer += deltatime;

            if (fpsTimer >= 1.0f)
            {
                currentFPS = (frameCount / fpsTimer);
                std::cout << "FPS: " << currentFPS << " | Frame time: " << deltatime * 1000.0f << " ms" << std::endl;

                frameCount = 0;
                fpsTimer = 0.0f;
            }
            game.Update(deltatime);

            canvas.PushMatrix();
            canvas.Translate(400, 300);
            gameView.Draw(canvas);
            canvas.PopMatrix();
        });

    return 0;
}