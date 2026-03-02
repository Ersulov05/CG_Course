#pragma once
#include "./AsteroidModel.h"
#include <vector>
#include <cmath>
#include <random>
#include <unordered_map>
#include <functional>

class AsteroidManager
{
public:
    const float MAX_DISTANCE = 700;

    using OnResetCallback = std::function<void(AsteroidManager &)>;
    using OnAddCallback = std::function<void(const std::shared_ptr<AsteroidModel> &)>;
    using OnRemoveCallback = std::function<void(const std::shared_ptr<AsteroidModel> &)>;

    void OnResetSubscribe(void *subscriber, OnResetCallback callback)
    {
        m_resetCallbacks[subscriber] = callback;
    }

    void OnResetUnSubscribe(void *subscriber)
    {
        m_resetCallbacks.erase(subscriber);
    }

    void OnAddSubscribe(void *subscriber, OnAddCallback callback)
    {
        m_addedCallbacks[subscriber] = callback;
    }

    void OnAddUnSubscribe(void *subscriber)
    {
        m_addedCallbacks.erase(subscriber);
    }

    void OnRemoveSubscribe(void *subscriber, OnRemoveCallback callback)
    {
        m_removedCallbacks[subscriber] = callback;
    }

    void OnRemoveUnSubscribe(void *subscriber)
    {
        m_removedCallbacks.erase(subscriber);
    }

    void Reset(const Point &playerPosition = {0, 0})
    {
        m_asteroids.clear();

        for (int i = 0; i < m_maxAsteroidCounts; ++i)
        {
            AddAsteroid(Point{0, 0});
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
            auto asteroidSize = (*it)->GetSize();
            NotifyAsteroidRemoved(*it);
            m_asteroids.erase(it);

            if (asteroidSize > 1)
            {
                for (int i = 0; i < 2; ++i)
                {
                    auto smallAsteroid = std::make_shared<AsteroidModel>();
                    smallAsteroid->SetPosition(asteroid->GetPosition());
                    smallAsteroid->SetSize(asteroidSize - 1);
                    smallAsteroid->SetSpeed(asteroid->GetSpeed() + (rand() % 20 - 10));
                    smallAsteroid->SetFlightAngle(asteroid->GetFlightAngle() + (rand() % 30 - 15));
                    smallAsteroid->SetRotation(asteroid->GetRotation());
                    smallAsteroid->SetRotateSpeed(asteroid->GetRotateSpeed() + (rand() % 20 - 10));

                    AddAsteroid(smallAsteroid);
                }
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
    std::unordered_map<void *, OnResetCallback> m_resetCallbacks;
    std::unordered_map<void *, OnAddCallback> m_addedCallbacks;
    std::unordered_map<void *, OnRemoveCallback> m_removedCallbacks;

    void NotifyResetCallbacks()
    {
        auto callbacks = m_resetCallbacks;
        for (const auto &[_, callback] : callbacks)
        {
            if (callback)
            {
                callback(*this);
            }
        }
    }

    void NotifyAsteroidAdded(const std::shared_ptr<AsteroidModel> &asteroid)
    {
        auto callbacks = m_addedCallbacks;
        for (const auto &[_, callback] : callbacks)
        {
            if (callback)
                callback(asteroid);
        }
    }

    void NotifyAsteroidRemoved(const std::shared_ptr<AsteroidModel> &asteroid)
    {
        auto callbacks = m_removedCallbacks;
        for (const auto &[_, callback] : callbacks)
        {
            if (callback)
                callback(asteroid);
        }
    }

    void RemoveFarAsteroids(const Point &playerPosition)
    {
        m_asteroids.erase(
            std::remove_if(
                m_asteroids.begin(), m_asteroids.end(),
                [this, &playerPosition](const std::shared_ptr<AsteroidModel> &asteroid)
                {
                    Point distanceVec = asteroid->GetPosition() - playerPosition;
                    float distance = sqrt(distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y);

                    if (distance > MAX_DISTANCE)
                    {
                        NotifyAsteroidRemoved(asteroid);
                        return true;
                    }
                    return false;
                }),
            m_asteroids.end());
    }

    void AddAsteroid(const Point &playerPosition)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> distAngle(0, 360);
        static std::uniform_real_distribution<float> distSpeed(30, 50);
        static std::uniform_real_distribution<float> distRangeAngle(-60, 60);
        static std::uniform_real_distribution<float> distRotateSpeed(0, 100);
        static std::uniform_int_distribution<int> distSize(1, 3);

        const float angle = distAngle(gen);
        const float angleRange = distRangeAngle(gen);
        const float speed = distSpeed(gen);
        const float distance = MAX_DISTANCE * 0.8f;
        const float rotateSpeed = distRotateSpeed(gen);
        const int size = distSize(gen);

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

    void AddAsteroid(const std::shared_ptr<AsteroidModel> &asteroid)
    {
        m_asteroids.push_back(asteroid);
        NotifyAsteroidAdded(asteroid);
    }
};