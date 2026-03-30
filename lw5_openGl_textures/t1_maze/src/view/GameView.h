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
            auto playerRotation = m_game.GetPlayer().GetRotation();

            camera.SetPosition(m_game.GetPlayer().GetPosition());
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