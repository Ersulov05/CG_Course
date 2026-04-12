#pragma once
#include "../model/Game.h"
#include "./Canvas/Canvas3D.h"
#include "./BallView.h"
#include "./BonusView.h"
#include "./BlockView.h"
#include "./RacketView.h"
#include "./SceneView.h"

class GameView {
public:
    GameView(Game &game): m_game(game), m_racket(RacketView(m_game.GetRacket())) {
        auto & keyboardController = m_canvas.GetKeyboardController();

        keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_LEFT, [this](float deltatime)
            { m_game.GetRacket().Move(Direction::Left, deltatime); });

        keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_RIGHT, [this](float deltatime)
            { m_game.GetRacket().Move(Direction::Right, deltatime); });
    }

    void Run() {
        m_canvas.SetLightPos({0, 4, 0});
        m_canvas.Run(
        [this](ICanvas3D &canvas, float deltatime)
        {
            auto &camera = canvas.GetCamera();

            camera.SetPosition(Point3D{0, 1.15, 0.45});
            camera.SetRotation(0, -40, 0);

            m_game.Update(deltatime);
            DrawScene();
            DrawBalls();
            DrawBonuses();
            DrawBlocks();
            DrawRacket();
        });
    }

private:
    Canvas3D m_canvas;
    Game &m_game;
    RacketView m_racket;
    SceneView m_scene;

    void DrawBonuses() {
        for (const auto & bonus : m_game.GetBonuses()) {
            BonusView::Draw(bonus, m_canvas);
        }
    }

    void DrawBalls() {
        for (const auto & ball : m_game.GetBalls()) {
            BallView::Draw(ball, m_canvas);
        }
    }

    void DrawBlocks() {
        for (auto & block : m_game.GetBlocks()) {
            BlockView::Draw(block, m_canvas);
        }
    }

    void DrawScene() {
        m_scene.Draw(m_canvas);
    }

    void DrawRacket() {
        m_racket.Draw(m_canvas);
    }    
};