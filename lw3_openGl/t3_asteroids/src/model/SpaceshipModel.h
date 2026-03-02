#pragma once
#include "../common/Point.h"
#include "../common/utils.h"
#include "./BulletModel.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include "./Collision/RectCollision.h"
#include "./Collision/CircleCollision.h"
#include "./Collision/ICollisionObject.h"

class SpaceshipModel
{
public:
    const float SPACESHIP_WIDTH = 20;
    const float SPACESHIP_HEIGHT = 40;
    const float SPACESHIP_PROTECTION_RADIUS = 30;
    const float RECHARGE_TIME = 0.1;
    const float BULLET_SPEED = 400;
    const float BULLET_LIFETIME = 20;
    const int MAX_HEALTH = 3;
    const float MAX_SPEED = 350;
    const float ROTATE_SPEED = 180;
    const float BOOST = 250;
    const float BREAK_BOOST = 150;
    const float PROTECTION_TIME = 2;

    SpaceshipModel() {}

    void Reset()
    {
        m_health = MAX_HEALTH;
        m_speed = 0;
        m_isEngineOn = false;
        m_bullets.clear();
        m_rotation = 0;
        UpdateCollisions();
    }

    void Update(float deltatime)
    {
        if (m_rechargeTime > 0)
        {
            m_rechargeTime -= deltatime;
        }

        if (m_protectionTime > 0)
        {
            m_protectionTime -= deltatime;
        }

        if (m_isEngineOn && m_speed < MAX_SPEED)
        {
            m_speed = std::min(m_speed + BOOST * deltatime, MAX_SPEED);
        }

        if (m_speed > 0)
        {
            float radAngle = ToRadians(m_rotation - 90);
            float xSpeed = m_speed * cos(radAngle) * deltatime;
            float ySpeed = m_speed * sin(radAngle) * deltatime;

            m_position += {xSpeed, ySpeed};

            if (!m_isEngineOn)
            {
                m_speed = std::max(m_speed - BREAK_BOOST * deltatime, 0.0f);
            }
        }

        UpdateCollisions();
        UpdateBullets(deltatime);
    }

    void Shoot()
    {
        if (m_rechargeTime > 0)
        {
            return;
        }
        BulletModel bullet;

        bullet.SetPosition(m_position);
        bullet.SetRotation(m_rotation);
        bullet.SetSpeed(BULLET_SPEED + m_speed);
        bullet.SetLifeTime(BULLET_LIFETIME);

        m_bullets.push_back(bullet);
        m_rechargeTime = RECHARGE_TIME;
    }

    void EngineOn()
    {
        m_isEngineOn = true;
    }

    void EngineOff()
    {
        m_isEngineOn = false;
    }

    bool IsEngineOn() const
    {
        return m_isEngineOn;
    }

    Point GetPosition() const
    {
        return m_position;
    }

    float GetRotation() const
    {
        return m_rotation;
    }

    int GetHealth() const
    {
        return m_health;
    }

    const ICollisionObject &GetCollision() const
    {
        if (IsProtected())
        {
            return m_protectCollision;
        }
        return m_collision;
    }

    void TakeDamage()
    {
        if (IsProtected())
        {
            return;
        }

        m_health = std::max(m_health - 1, 0);
        m_protectionTime = PROTECTION_TIME;
    }

    bool IsProtected() const
    {
        return m_protectionTime > 0;
    }

    std::vector<BulletModel> &GetBullets()
    {
        return m_bullets;
    }

    void LeftRotate(float deltatime)
    {
        m_rotation -= ROTATE_SPEED * deltatime;
        UpdateCollisions();
    }

    void RightRotate(float deltatime)
    {
        m_rotation += ROTATE_SPEED * deltatime;
        UpdateCollisions();
    }

    float GetSpeed() const
    {
        return m_speed;
    }

private:
    Point m_position = Point(0, 0);
    float m_rotation = 0;
    float m_speed = 0;
    bool m_isEngineOn = false;
    int m_health = MAX_HEALTH;
    RectCollision m_collision = RectCollision({0, 0}, SPACESHIP_WIDTH, SPACESHIP_HEIGHT);
    CircleCollision m_protectCollision = CircleCollision({0, 0}, SPACESHIP_PROTECTION_RADIUS);
    std::vector<BulletModel> m_bullets;
    float m_rechargeTime = 0;
    float m_protectionTime = 0;

    void UpdateBullets(float deltatime)
    {
        RemoveBullets();

        for (auto &bullet : m_bullets)
        {
            bullet.Update(deltatime);
        }
    }

    void RemoveBullets()
    {
        m_bullets.erase(
            std::remove_if(
                m_bullets.begin(), m_bullets.end(),
                [this](const BulletModel &bullet)
                {
                    return bullet.GetLifeTime() <= 0;
                }),
            m_bullets.end());
    }

    void UpdateCollisions()
    {
        m_collision.SetPosition(m_position);
        m_collision.SetRotation(m_rotation);
        m_protectCollision.SetPosition(m_position);
    }
};