#pragma once
#include "./SpaceshipModel.h"
#include "./AsteroidManager.h"
#include "./Collision/CollisionSystem.h"

const int BASE_SCORE = 50;

class Game
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
    }

    void Update(float deltatime)
    {
        m_asteroidManager.Update(deltatime, m_spaceship.GetPosition());
        m_spaceship.Update(deltatime);

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

    int GetScore()
    {
        return m_score;
    }

private:
    SpaceshipModel m_spaceship;
    AsteroidManager m_asteroidManager;
    int m_score = 0;

    void CheckCollisionSpaceshipWithAsteroids()
    {
        // std::cout << m_spaceship.GetCollision().GetPosition().x << std::endl;
        std::vector<std::shared_ptr<AsteroidModel>> collidedAsteroids;

        for (const auto &asteroidPtr : m_asteroidManager.GetAsteroids())
        {
            // std::cout << asteroidPtr->GetCollision().GetPosition().x << std::endl;
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