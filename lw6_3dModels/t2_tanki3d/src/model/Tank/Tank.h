#pragma once
#include "../../common/Geometry.h"
#include "./Cannon/Cannon.h"
#include "../Map/Map.h"
#include "./Cannon/CannonFactory.h"
#include <stdexcept>
#include <iostream>
#include <functional>

enum class Direction {
    Forward,
    Backward,
    Left,
    Right
};

class Tank : public std::enable_shared_from_this<Tank> {
public:
    Tank(Map& map, const Point3D& position, unsigned int level)
        : m_map(map), 
          m_position(position), 
          m_level(level),
          m_cannon(CannonFactory::CreateCannonByLevel(level))
    {
        m_cannon.SetLocalPosition(m_cannonPosition);
    }

    void Update(float deltatime) 
    {
        m_cannon.Update(deltatime);
        ApplyFrictionToSpeed(deltatime);
        
        m_position -= m_speed * deltatime;
    }

    Point3D GetPosition() const 
    {
        return m_position;
    }

    Quaternion3D GetRotation() const 
    {
        return m_rotation;
    }

    std::vector<Shell> Fire() 
    {
        auto shells = m_cannon.Fire(m_position, m_rotation);
        for (auto& shell : shells) {
            shell.SetOwner(weak_from_this());
        }
        return std::move(shells); 
    }
    
    void Move(Direction direction, float deltatime) 
    {
        switch (direction) {
            case Direction::Forward:
                m_rotation = Quaternion3D::FromDegrees(0, 0, 0);
                break;
            case Direction::Backward:
                m_rotation = Quaternion3D::FromDegrees(0, 180, 0);
                break;
            case Direction::Left:
                m_rotation = Quaternion3D::FromDegrees(0, 90, 0);
                break;
            case Direction::Right:
                m_rotation = Quaternion3D::FromDegrees(0, -90, 0);
                break;
            default:
                std::invalid_argument("Unknown direction: " + (int)direction);
        }

        m_speed += m_rotation.GetForward() * m_acceleration * GetTraction() * deltatime;
    }

    void SetPosition(const Point3D& position)
    {
        m_position = position;
    }

    void SetSpeed(const Vector3D& speed)
    {
        m_speed = speed;
    }

    unsigned int GetHealth() const 
    {
        return m_health;
    }

    unsigned int GetLevel() const 
    {
        return m_level;
    }

    Vector3D GetSpeed() const
    {
        return m_speed;
    }

    void SetLevel(unsigned int level) 
    {
        m_level = level;
        m_cannon = CannonFactory::CreateCannonByLevel(level);
        m_cannon.SetLocalPosition(m_cannonPosition);
    }

    const Cannon& GetCannon() const 
    {
        return m_cannon;
    }

    Size3D GetSize() const
    {
        return Size3D{3, 2, 6};
    }

private:
    Point3D m_position;
    Vector3D m_speed;
    Cannon m_cannon;
    Quaternion3D m_rotation;
    float m_acceleration = 10;
    float m_sideFrictionCoef = 40;
    Map& m_map;
    unsigned int m_level;
    unsigned int m_health;
    const Point3D m_cannonPosition = {0, 1.5, -1};

    template<typename T>
    T sign(T value) 
    {
        return (T(0) < value) - (value < T(0));
    }

    void ApplyFrictionToSpeed(float deltatime) 
    {
        float friction = GetFriction();
        float sideFriction = friction * m_sideFrictionCoef;
        
        Vector3D forward = m_rotation.GetForward();
        Vector3D right = m_rotation.GetRight();
        
        float forwardSpeed = forward.Dot(m_speed);
        float rightSpeed = right.Dot(m_speed);
        
        // Уменьшаем скорость, но не даём ей перейти через ноль
        forwardSpeed = std::max(0.0f, std::abs(forwardSpeed) - friction * deltatime) * sign(forwardSpeed);
        rightSpeed = std::max(0.0f, std::abs(rightSpeed) - sideFriction * deltatime) * sign(rightSpeed);
        
        m_speed = forward * forwardSpeed + right * rightSpeed;
    }

    float GetFriction() const 
    {
        auto terrarian = m_map.GetTerrarianByPosition(m_position);
        if (!terrarian.has_value()) {
            return 0.5;
        }
        
        auto terrarianType = terrarian->GetType();
        switch (terrarianType) {
            case TerrarianType::Dirt:
                return 0.5;
            case TerrarianType::Ice:
                return 0.1;
            default:
                throw std::invalid_argument("Unknown terrarianType: " + (int)terrarianType);
        }
    }

    float GetTraction() const 
    {
        auto terrarian = m_map.GetTerrarianByPosition(m_position);
        if (!terrarian.has_value()) {
            return 0.5;
        }

        auto terrarianType = terrarian->GetType();
        switch (terrarianType) {
            case TerrarianType::Dirt:
                return 0.7;
            case TerrarianType::Ice:
                return 0.15;
            default:
                throw std::invalid_argument("Unknown terrarianType: " + (int)terrarianType);
        }
    }
};