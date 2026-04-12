#pragma once
#include "../common/Geometry.h"
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <vector>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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
        m_position = m_position + m_moveDirection * m_speed * deltatime;

        float distance = m_speed * deltatime;
        float angleRad = distance / m_radius / 2;

        if (std::abs(m_moveDirection.x) > 0.001f || std::abs(m_moveDirection.z) > 0.001f) {            
            float axisX = m_moveDirection.z;
            float axisZ = -m_moveDirection.x;
            
            glm::quat deltaRot = glm::angleAxis(angleRad, glm::vec3(axisX, 0.0f, axisZ));
            m_rotation = deltaRot * m_rotation;  // или m_rotation = m_rotation * deltaRot
            
            // Нормализуем кватернион для предотвращения ошибок накопления
            m_rotation = glm::normalize(m_rotation);
        }
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

    glm::quat GetRotate() const
    {
        return m_rotation;
    }
private:
    Point3D m_position = {0, 0, 0};
    Vector3D m_moveDirection = {0, 0, 0};
    float m_radius = 2;
    float m_speed = 1;
    glm::quat m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
};