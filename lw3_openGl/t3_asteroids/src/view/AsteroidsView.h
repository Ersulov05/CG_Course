#pragma once
#include "../model/AsteroidManager.h"
#include "./AsteroidView.h"
#include "./Canvas/ICanvas.h"

class AsteroidsView
{
public:
    AsteroidsView(AsteroidManager &asteroidManager)
        : m_asteroidManager(asteroidManager)
    {
        InitAsteroids();
        SetSubscribes();
    }

    ~AsteroidsView()
    {
        m_asteroidManager.OnResetUnSubscribe(this);
        m_asteroidManager.OnAddUnSubscribe(this);
        m_asteroidManager.OnRemoveUnSubscribe(this);
    }

    void Draw(ICanvas &canvas)
    {
        for (auto &[_, asteroid] : m_asteroids)
        {
            asteroid.Draw(canvas);
        }
    }

private:
    AsteroidManager &m_asteroidManager;
    std::unordered_map<void *, AsteroidView> m_asteroids;

    void SetSubscribes()
    {
        m_asteroidManager.OnResetSubscribe(
            this,
            [this]()
            {
                InitAsteroids();
            });

        m_asteroidManager.OnAddSubscribe(
            this,
            [this](const std::shared_ptr<AsteroidModel> &asteroid)
            {
                m_asteroids.emplace(asteroid.get(), AsteroidView(asteroid));
            });

        m_asteroidManager.OnRemoveSubscribe(
            this,
            [this](const std::shared_ptr<AsteroidModel> &asteroid)
            {
                m_asteroids.erase(asteroid.get());
            });
    }

    void InitAsteroids()
    {
        m_asteroids.clear();
        for (const auto &asteroidPtr : m_asteroidManager.GetAsteroids())
        {
            m_asteroids.emplace(asteroidPtr.get(), AsteroidView(asteroidPtr));
        }
    }
};