#pragma once
#include "./SpaceshipModel.h"
#include "./AsteroidManager.h"
#include "./Collision/CollisionSystem.h"
#include "./GameObservable.h"

const int BASE_SCORE = 50;

class Game : public GameObservable
{
public:
    Game()
    {
        Reset();
    }

    void Reset()
    {
        m_spaceship.Reset();
        m_asteroidManager.Reset(m_spaceship.GetPosition());
        m_isEndGame = false;
        m_score = 0;
    }

    void Update(float deltatime)
    {
        if (m_isEndGame)
            return;

        m_asteroidManager.Update(deltatime, m_spaceship.GetPosition());
        m_spaceship.Update(deltatime);
        if (m_spaceship.GetHealth() == 0)
        {
            m_isEndGame = true;
            NotifyEndGameCallbacks();
        }

        CheckCollisionBulletsWithAsteroids();
        CheckCollisionSpaceshipWithAsteroids();
    }

    AsteroidManager &GetAsteroidManager()
    {
        return m_asteroidManager;
    }

    SpaceshipModel &GetSpaceshipModel()
    {
        return m_spaceship;
    }

    void AddScore(int score = BASE_SCORE)
    {
        m_score += score;
    }

    int GetScore() const
    {
        return m_score;
    }

private:
    SpaceshipModel m_spaceship;
    AsteroidManager m_asteroidManager;
    int m_score = 0;
    bool m_isEndGame = false;

    void CheckCollisionSpaceshipWithAsteroids()
    {
        std::vector<std::shared_ptr<AsteroidModel>> collidedAsteroids;

        for (const auto &asteroidPtr : m_asteroidManager.GetAsteroids())
        {
            if (CollisionSystem::CheckCollision(asteroidPtr->GetCollision(), m_spaceship.GetCollision()))
            {
                collidedAsteroids.push_back(asteroidPtr);
            }
        }

        for (const auto &asteroidPtr : collidedAsteroids)
        {
            m_spaceship.TakeDamage();
            AddScore(asteroidPtr->GetSize() * BASE_SCORE);
            m_asteroidManager.SplitAsteroid(asteroidPtr);
        }
    }

    void CheckCollisionBulletsWithAsteroids()
    {
        std::vector<std::shared_ptr<AsteroidModel>> collidedAsteroids;

        for (const auto &asteroidPtr : m_asteroidManager.GetAsteroids())
        {
            for (auto &bullet : m_spaceship.GetBullets())
            {
                if (CollisionSystem::CheckCollision(asteroidPtr->GetCollision(), bullet.GetCollision()))
                {
                    collidedAsteroids.push_back(asteroidPtr);
                    bullet.Remove();
                }
            }
        }

        for (const auto &asteroidPtr : collidedAsteroids)
        {
            AddScore(asteroidPtr->GetSize() * BASE_SCORE);
            m_asteroidManager.SplitAsteroid(asteroidPtr);
        }
    }
};