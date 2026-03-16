#pragma once
#include "../model/Game.h"
#include "./Canvas/Canvas3D.h"
#include "./WallView.h"

class GameView {
public:
    GameView(Game &game): m_game(game) {
        for (auto & wall : game.GetMaze().GetWalls()) {
            m_walls.push_back(WallView(wall));
        }

        auto & keyboardController = m_canvas.GetKeyboardController();

        keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_LEFT, [this](float deltatime)
            { m_game.GetPlayer().Move(Direction::Left, deltatime); });

        keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_RIGHT, [this](float deltatime)
            { m_game.GetPlayer().Move(Direction::Right, deltatime); });

        keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_UP, [this](float deltatime)
            { m_game.GetPlayer().Move(Direction::Forward, deltatime); });

        keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_DOWN, [this](float deltatime)
            { m_game.GetPlayer().Move(Direction::Back, deltatime); });

        keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_W, [this](float deltatime)
            { m_game.GetPlayer().Rotate(70 * deltatime, 0, 0); });

        keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_S, [this](float deltatime)
            { m_game.GetPlayer().Rotate(-70 * deltatime, 0, 0); });

        keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_A, [this](float deltatime)
            { m_game.GetPlayer().Rotate(0, 120 * deltatime, 0); });

        keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_D, [this](float deltatime)
            { m_game.GetPlayer().Rotate(0, -120 * deltatime, 0); });
    }

    void Run() {
        m_canvas.Run(
        [this](ICanvas3D &canvas, float deltatime)
        {
            auto &camera = canvas.GetCamera();
            auto playerPos = m_game.GetPlayer().GetPosition();
            auto playerRotation = m_game.GetPlayer().GetRotation();

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

            camera.SetPosition(playerPos.x, playerPos.y, playerPos.z);
            camera.SetRotation(playerRotation.y, playerRotation.x, playerRotation.z);

            DrawWalls();
        });
    }
private:
    Canvas3D m_canvas;
    Game &m_game;
    std::vector<WallView> m_walls;

    void DrawWalls() {
        for (auto & wall : m_walls) {
            wall.Draw(m_canvas);
        }
    }
};