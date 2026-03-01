#pragma once
#include "../model/Game.h"
#include "./AsteroidView.h"
#include "./SpaceshipView.h"
#include "../model/AsteroidManager.h"
#include "../common/Color.h"
#include "./StarsView.h"
#include <vector>
#include "./UI.h"

class GameView
{
public:
    GameView(Game &game) : m_game(game), m_ui(game), m_spaceship(m_game.GetSpaceshipModel())
    {
        auto &asteroidManager = m_game.GetAsteroidManager();
        for (const auto &asteroidPtr : asteroidManager.GetAsteroids())
        {
            m_asteroids.emplace(asteroidPtr.get(), AsteroidView(asteroidPtr));
        }

        asteroidManager.OnResetSubscribe(
            this,
            [this](AsteroidManager &manager)
            {
                m_asteroids.clear();
                for (const auto &asteroidPtr : manager.GetAsteroids())
                {
                    m_asteroids.emplace(asteroidPtr.get(), AsteroidView(asteroidPtr));
                }
            });

        asteroidManager.OnAddSubscribe(
            this,
            [this](const std::shared_ptr<AsteroidModel> &asteroid)
            {
                m_asteroids.emplace(asteroid.get(), AsteroidView(asteroid));
            });

        asteroidManager.OnRemoveSubscribe(
            this,
            [this](const std::shared_ptr<AsteroidModel> &asteroid)
            {
                m_asteroids.erase(asteroid.get());
            });
    }

    ~GameView()
    {
        auto &manager = m_game.GetAsteroidManager();
        manager.OnResetUnSubscribe(this);
        manager.OnAddUnSubscribe(this);
        manager.OnRemoveUnSubscribe(this);
    }

    void Draw(ICanvas &canvas)
    {
        canvas.PushMatrix();
        canvas.Translate(m_spaceship.GetPosition() * -1);
        m_stars.Draw(canvas, m_spaceship.GetPosition());
        DrawAsteroids(canvas);
        DrawBullets(canvas, m_game.GetSpaceshipModel().GetBullets());
        canvas.PopMatrix();

        m_spaceship.Draw(canvas);
        m_ui.Draw(canvas);
    }

private:
    UI m_ui;
    Game &m_game;
    SpaceshipView m_spaceship;
    StarsView m_stars;
    std::unordered_map<void *, AsteroidView> m_asteroids;

    void DrawAsteroids(ICanvas &canvas)
    {
        for (auto &[_, asteroid] : m_asteroids)
        {
            asteroid.Draw(canvas);
        }
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
};