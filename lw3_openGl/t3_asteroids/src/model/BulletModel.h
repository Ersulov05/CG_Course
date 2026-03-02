#pragma once
#include "../common/Point.h"
#include "./Collision/RectCollision.h"
#include "../common/utils.h"

class BulletModel
{
public:
    void Update(float deltatime)
    {
        float radAngle = ToRadians(m_rotation - 90);
        float xSpeed = m_speed * cos(radAngle) * deltatime;
        float ySpeed = m_speed * sin(radAngle) * deltatime;

        m_position += {xSpeed, ySpeed};
        m_lifeTime -= deltatime;

        m_collision.SetPosition(m_position);
    }

    void SetPosition(const Point &position)
    {
        m_position = position;
        m_collision.SetPosition(m_position);
    }

    void SetRotation(float rotation)
    {
        m_rotation = rotation;
        m_collision.SetRotation(m_rotation);
    }

    Point GetPosition() const
    {
        return m_position;
    }

    float GetRotation() const
    {
        return m_rotation;
    }

    void SetSpeed(float speed)
    {
        m_speed = speed;
    }

    void SetLifeTime(float lifeTime)
    {
        m_lifeTime = lifeTime;
    }

    float GetLifeTime() const
    {
        return m_lifeTime;
    }

    void Remove()
    {
        m_lifeTime = 0;
    }

    const RectCollision GetCollision() const
    {
        return m_collision;
    }

private:
    Point m_position = {0, 0};
    float m_rotation;
    float m_speed;
    float m_lifeTime;
    RectCollision m_collision = RectCollision(Point{0, 0}, 4, 8);
};