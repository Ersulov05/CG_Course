#pragma once
#include "../common/Geometry.h"
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <vector>

class Ball
{
public:
    Ball(const Point3D& position, Vector3D moveDirection, float radius, float speed)
        : m_position(position), m_moveDirection(moveDirection), m_radius(radius), m_speed(speed) 
    {
        m_moveDirection.Normalized();
    }

    void Update(float deltatime) 
    {
        auto oldPos = m_position;
        m_position = m_position + m_moveDirection * m_speed * deltatime;
    }

    Point3D GetPosition() const
    {
        return m_position;
    }

    float GetRadius() const
    {
        return m_radius;
    }

    float GetSpeed() 
    {
        return m_speed;
    }

    void SetSpeed(float speed) 
    {
        m_speed = speed;
    }

    Vector3D GetMoveDirection() 
    {
        return m_moveDirection;
    }

    void SetPosition(Point3D position) 
    {
        m_position = position;
    }

    void SetMoveDirection(Vector3D moveDirection) 
    {
        m_moveDirection = moveDirection;
        m_moveDirection.Normalized();
    }

    void SetRadius(float radius) 
    {
        m_radius = radius;
    }
private:
    Point3D m_position = {0, 0, 0};
    Vector3D m_moveDirection = {0, 0, 0};
    float m_radius = 2;
    float m_speed = 1;
};