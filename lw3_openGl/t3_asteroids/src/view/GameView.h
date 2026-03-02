#pragma once

#include "../model/AsteroidManager.h"
#include "../model/Game.h"
#include "./Canvas/Canvas.h"
#include "./Canvas/ICanvas.h"
#include "./SpaceshipView.h"
#include "./AsteroidsView.h"
#include "./AsteroidView.h"
#include "./StarsView.h"
#include "../common/Color.h"
#include "./UI.h"
#include <vector>

const unsigned WIDTH = 800;
const unsigned HEIGHT = 600;

class GameView
{
public:
    GameView(Game &game)
        : m_canvas(WIDTH, HEIGHT),
          m_gameModel(game),
          m_ui(game),
          m_spaceship(m_gameModel.GetSpaceshipModel()),
          m_asteroids(m_gameModel.GetAsteroidManager())
    {
        SetSubscribes();
    }

    void Run()
    {
        m_canvas.RunWindow(
            [this](ICanvas &canvas, float deltatime)
            {
                m_gameModel.Update(deltatime);

                canvas.PushMatrix();
                canvas.Translate(400, 300);
                Draw(canvas);
                canvas.PopMatrix();
            });
    }

private:
    Game &m_gameModel;
    UI m_ui;

    Canvas m_canvas;
    SpaceshipView m_spaceship;
    StarsView m_stars;
    AsteroidsView m_asteroids;

    void Draw(ICanvas &canvas)
    {
        canvas.PushMatrix();
        canvas.Translate(m_spaceship.GetPosition() * -1);
        m_stars.Draw(canvas, m_spaceship.GetPosition());
        m_asteroids.Draw(canvas);
        DrawBullets(canvas, m_gameModel.GetSpaceshipModel().GetBullets());
        canvas.PopMatrix();

        m_spaceship.Draw(canvas);
        m_ui.Draw(canvas);
    }

    void DrawBullets(ICanvas &canvas, std::vector<BulletModel> &bullets)
    {
        for (auto &bullet : bullets)
        {
            canvas.PushMatrix();
            canvas.Translate(bullet.GetPosition());
            canvas.Rotate(bullet.GetRotation());
            canvas.SetColor(0xDFDF00FF);
            canvas.FillRect({-2, -4}, {4, 8});
            canvas.PopMatrix();
        }
    }

    void SetSubscribes()
    {
        auto &keyboardController = m_canvas.GetKeyboardController();

        keyboardController.OnKeyPressedSubscribe(
            this, GLFW_KEY_UP,
            [this]()
            {
                m_gameModel.GetSpaceshipModel().EngineOn();
            });

        keyboardController.OnKeyReleasedSubscribe(
            this, GLFW_KEY_UP,
            [this]()
            {
                m_gameModel.GetSpaceshipModel().EngineOff();
            });

        keyboardController.OnKeyPressedSubscribe(
            this, GLFW_KEY_SPACE,
            [this]()
            {
                m_gameModel.GetSpaceshipModel().Shoot();
            });

        keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_LEFT,
            [this](float deltatime)
            {
                m_gameModel.GetSpaceshipModel().LeftRotate(deltatime);
            });

        keyboardController.OnKeyHoldSubscribe(
            this, GLFW_KEY_RIGHT,
            [this](float deltatime)
            {
                m_gameModel.GetSpaceshipModel().RightRotate(deltatime);
            });
    }
};