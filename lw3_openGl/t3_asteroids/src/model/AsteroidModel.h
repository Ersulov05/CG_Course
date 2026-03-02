#pragma once
#include "../common/Point.h"
#include <cmath>
#include "./Collision/CircleCollision.h"
#include <memory>
#include "../common/utils.h"
#include <algorithm>

const float BASE_RADIUS = 12;

class AsteroidModel
{
public:
    void SetPosition(const Point &pos) { m_position = pos; }
    Point GetPosition() const { return m_position; }

    void SetSize(int size)
    {
        m_size = size;
        m_collision.SetRadius(GetRadius());
    }
    int GetSize() const { return m_size; }

    void SetSpeed(float speed)
    {
        m_speed = std::max(speed, MIN_SPEED);
    }
    float GetSpeed() const { return m_speed; }

    void SetFlightAngle(float angle) { m_flightAngle = angle; }
    float GetFlightAngle() const { return m_flightAngle; }

    void SetRotation(float rot) { m_rotation = rot; }
    float GetRotation() const { return m_rotation; }

    void SetRotateSpeed(float rotSpeed) { m_rotate = rotSpeed; }
    float GetRotateSpeed() const { return m_rotate; }

    float GetRadius() const
    {
        return m_size * BASE_RADIUS;
    }

    const CircleCollision GetCollision() const
    {
        return m_collision;
    }

    void Update(float deltatime)
    {
        float radAngle = ToRadians(m_flightAngle - 90);
        float xSpeed = m_speed * cos(radAngle) * deltatime;
        float ySpeed = m_speed * sin(radAngle) * deltatime;

        m_position += {xSpeed, ySpeed};
        m_rotation += m_rotate * deltatime;
        m_collision.SetPosition(m_position);
    }

private:
    Point m_position = {0, 0};
    float m_rotation = 0;
    float m_speed;
    float m_flightAngle;
    float m_rotate;
    unsigned m_size;
    CircleCollision m_collision;
    static constexpr float MIN_SPEED = 30;
};