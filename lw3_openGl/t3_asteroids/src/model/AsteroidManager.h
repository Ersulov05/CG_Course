#pragma once
#include "./AsteroidManagerObservable.h"
#include "./AsteroidModel.h"
#include <vector>
#include <cmath>
#include <random>

class AsteroidManager : public AsteroidManagerObservable
{
public:
    AsteroidManager()
    {
        std::random_device rd;
        m_random = std::mt19937(rd());
    }

    const float MAX_DISTANCE = 700;
    const int COUNT_ASTEROID_SEPARATION_PARTS = 2;

    void Reset(const Point &playerPosition = {0, 0})
    {
        m_asteroids.clear();

        for (int i = 0; i < m_maxAsteroidCounts; ++i)
        {
            AddAsteroid(playerPosition);
        }
        NotifyResetCallbacks();
    }

    void Update(float deltatime, const Point &playerPosition)
    {
        for (auto &asteroid : m_asteroids)
        {
            asteroid->Update(deltatime);
        }

        RemoveFarAsteroids(playerPosition);

        while (m_asteroids.size() < m_maxAsteroidCounts)
        {
            AddAsteroid(playerPosition);
        }
    }

    void SplitAsteroid(const std::shared_ptr<AsteroidModel> &asteroid)
    {
        auto it = std::find(m_asteroids.begin(), m_asteroids.end(), asteroid);

        if (it != m_asteroids.end())
        {
            NotifyAsteroidRemoved(*it);
            m_asteroids.erase(it);

            if (asteroid->GetSize() > 1)
            {
                AddSplittedAsteroids(asteroid);
            }
        }
    }

    const std::vector<std::shared_ptr<AsteroidModel>> &GetAsteroids() const
    {
        return m_asteroids;
    }

private:
    std::vector<std::shared_ptr<AsteroidModel>> m_asteroids;
    int m_maxAsteroidCounts = 20;
    std::mt19937 m_random;

    void RemoveFarAsteroids(const Point &playerPosition)
    {
        m_asteroids.erase(
            std::remove_if(
                m_asteroids.begin(), m_asteroids.end(),
                [this, &playerPosition](const std::shared_ptr<AsteroidModel> &asteroid)
                {
                    if (IsAsteroidRemove(asteroid->GetPosition(), playerPosition))
                    {
                        NotifyAsteroidRemoved(asteroid);
                        return true;
                    }
                    return false;
                }),
            m_asteroids.end());
    }

    bool IsAsteroidRemove(const Point &asteroidPosition, const Point &playerPosition)
    {
        Point distanceVec = asteroidPosition - playerPosition;
        float distance = distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y;

        return distance > MAX_DISTANCE * MAX_DISTANCE;
    }

    void AddAsteroid(const Point &playerPosition)
    {
        static std::uniform_real_distribution<float> distAngle(0, 360);
        static std::uniform_real_distribution<float> distSpeed(30, 50);
        static std::uniform_real_distribution<float> distRangeAngle(-60, 60);
        static std::uniform_real_distribution<float> distRotateSpeed(0, 100);
        static std::uniform_int_distribution<int> distSize(1, 3);

        const float angle = distAngle(m_random);
        const float angleRange = distRangeAngle(m_random);
        const float speed = distSpeed(m_random);
        const float distance = MAX_DISTANCE * 0.8f;
        const float rotateSpeed = distRotateSpeed(m_random);
        const int size = distSize(m_random);

        auto asteroid = std::make_shared<AsteroidModel>();

        Point position;
        position.x = playerPosition.x + cos(angle / 180 * M_PI) * distance;
        position.y = playerPosition.y + sin(angle / 180 * M_PI) * distance;

        asteroid->SetPosition(position);
        asteroid->SetSpeed(speed);
        asteroid->SetFlightAngle(angle - 90 + angleRange);
        asteroid->SetRotateSpeed(rotateSpeed);
        asteroid->SetSize(size);

        AddAsteroid(asteroid);
    }

    void AddSplittedAsteroids(const std::shared_ptr<AsteroidModel> &splittedAsteroid)
    {
        static std::uniform_real_distribution<float> distDeviationRotateSpeed(-100, 100);
        static std::uniform_real_distribution<float> distDeviationFlightAngle(-90, 90);
        static std::uniform_real_distribution<float> distDeviationSpeed(-10, 15);

        for (int i = 0; i < COUNT_ASTEROID_SEPARATION_PARTS; ++i)
        {
            float deviationFlightAngle = distDeviationFlightAngle(m_random);
            float deviationRotateSpeed = distDeviationRotateSpeed(m_random);
            float deviationSpeed = distDeviationSpeed(m_random);

            auto asteroid = std::make_shared<AsteroidModel>();

            asteroid->SetPosition(splittedAsteroid->GetPosition());
            asteroid->SetSize(splittedAsteroid->GetSize() - 1);
            asteroid->SetRotation(splittedAsteroid->GetRotation());
            asteroid->SetSpeed(splittedAsteroid->GetSpeed() + deviationSpeed);
            asteroid->SetFlightAngle(splittedAsteroid->GetFlightAngle() + deviationFlightAngle);
            asteroid->SetRotateSpeed(splittedAsteroid->GetRotateSpeed() + deviationRotateSpeed);

            AddAsteroid(asteroid);
        }
    }

    void AddAsteroid(const std::shared_ptr<AsteroidModel> &asteroid)
    {
        m_asteroids.push_back(asteroid);
        NotifyAsteroidAdded(asteroid);
    }
};