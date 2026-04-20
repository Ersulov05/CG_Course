#pragma once
#include "../../common/Geometry.h"
#include "./Cannon/Cannon.h"
#include "../Map/Map.h"
#include "./Cannon/CannonFactory.h"
#include <stdexcept>
#include <iostream>
#include <functional>
#include "../Object/GameObject.h"
#include "./Data/TankData.h"
#include "./Data/TanksLevelData.h"
#include <functional>

enum class Direction {
    Forward,
    Backward,
    Left,
    Right
};

class Tank 
    : public GameObject
    , public std::enable_shared_from_this<Tank> {
public:
    Tank(const Map& map, TankType type, unsigned int level, const Point3D& position = {0, 0, 0})
        : GameObject(position, {3, 2, 6}),
          m_map(std::ref(map)),
          m_type(type),
          m_cannon(CannonFactory::CreateCannonByLevel(level))
    {
        auto tankData = TanksLevelData::GetTankDataByTankType(m_type);
        m_size = tankData.size;
        m_maxSpeed = tankData.maxSpeed;
        SetCannonPosition(tankData.cannonPos);
        SetLevel(level);
        m_position.y = 0.5;
    }

    void Update(float deltatime) 
    {
        m_cannon.Update(deltatime);

        if (m_isFreezed) return;
        UpdateSpeed(deltatime);
        m_position -= m_speed * deltatime;
    }

    std::vector<Shell> Fire() 
    {
        auto shells = m_cannon.Fire(m_position, m_rotation);
        for (auto& shell : shells) {
            shell.SetOwner(weak_from_this());
        }
        return std::move(shells); 
    }

    void Rotate(Direction direction)
    {
        if (m_isFreezed) return;
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
    }
    
    void Move() 
    {
        m_isMoved = true;
    }

    void Stop()
    {
        m_isMoved = false;
    }

    void SetSpeed(const Vector3D& speed)
    {
        m_speed = speed;
    }

    unsigned int GetHealth() const 
    {
        return m_health;
    }

    unsigned int GetTotalHealth() const 
    {
        return m_totalHealth;
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
        auto tankLevelData = TanksLevelData::GetTankLevelData(m_type, level);
        m_cannon = CannonFactory::CreateCannonByLevel(tankLevelData.cannonLevel);
        m_cannon.SetLocalPosition(m_cannonPosition);
        m_acceleration = tankLevelData.acceleration;
        m_totalHealth = tankLevelData.health;
        m_health = m_totalHealth;
        m_level = level;
    }

    void SetCannonPosition(const Point3D& position)
    {
        m_cannonPosition = position;
        m_cannon.SetLocalPosition(m_cannonPosition);
    }

    const Cannon& GetCannon() const 
    {
        return m_cannon;
    }

    Cannon& GetCannon() 
    {
        return m_cannon;
    }

    void TakeDamage(unsigned int damage)
    {
        if (m_isProtected) return;
        if (damage > m_health) {
            m_health = 0;
        } else {
            m_health -= damage;
        }
    }

    void AddHealth(unsigned int health)
    {
        m_health += health;
        if (m_health > m_totalHealth) {
            m_health = m_totalHealth;
        }
    }

    void SetProtected(bool isProtected)
    {
        m_isProtected = isProtected;
    }

    bool GetProtected() const 
    {
        return m_isProtected;
    }

    bool IsMoved() const
    {
        return m_isMoved;
    }

    void SetFreezed(bool isFreezed)
    {
        m_isFreezed = isFreezed;
    }

    TankType GetType() const
    {
        return m_type;
    }

private:
    Vector3D m_speed;
    Cannon m_cannon;
    float m_acceleration = 10;
    float m_maxSpeed = 6;
    const float m_sideBreackCoef = 2;
    std::reference_wrapper<const Map> m_map;
    unsigned int m_level;
    unsigned int m_health = 0;
    unsigned int m_totalHealth;
    Point3D m_cannonPosition = {0, 1.5, -1};
    TankType m_type;
    bool m_isMoved = false;
    bool m_isProtected = false;
    bool m_isFreezed = false;

    template<typename T>
    T sign(T value) 
    {
        return (T(0) < value) - (value < T(0));
    }

    void UpdateSpeed(float deltatime)
    {
        ApplyFrictionToSpeed(deltatime);
        Vector3D forward = m_rotation.GetForward();
        Vector3D right = m_rotation.GetRight();
        float forwardSpeed = forward.Dot(m_speed);
        float rightSpeed = right.Dot(m_speed);

        float acceleration = m_acceleration * GetTraction();
        if (m_isMoved) {
            forwardSpeed = std::min(forwardSpeed + acceleration * deltatime, m_maxSpeed);
        } else {
            forwardSpeed = std::max(0.0f, std::abs(forwardSpeed) - acceleration * deltatime) * sign(forwardSpeed);
        }

        rightSpeed = std::max(0.0f, std::abs(rightSpeed) - acceleration * m_sideBreackCoef * deltatime) * sign(rightSpeed);
        m_speed = forward * forwardSpeed + right * rightSpeed;
    }

    void ApplyFrictionToSpeed(float deltatime) 
    {
        Vector3D forward = m_rotation.GetForward();
        Vector3D right = m_rotation.GetRight();
        float forwardSpeed = forward.Dot(m_speed);
        float rightSpeed = right.Dot(m_speed);
        
        float friction = GetFriction();
        forwardSpeed = std::max(0.0f, std::abs(forwardSpeed) - friction * deltatime) * sign(forwardSpeed);
        rightSpeed = std::max(0.0f, std::abs(rightSpeed) - friction * m_sideBreackCoef * deltatime) * sign(rightSpeed);
        
        m_speed = forward * forwardSpeed + right * rightSpeed;
    }

    float GetFriction() const 
    {
        auto terrarian = m_map.get().GetTerrarianByPosition(m_position);
        if (!terrarian.has_value()) {
            return 0.5;
        }

        return Terrarian::GetFriction(terrarian->GetType());
    }

    float GetTraction() const 
    {
        auto terrarian = m_map.get().GetTerrarianByPosition(m_position);
        if (!terrarian.has_value()) {
            return 0.5;
        }

        return Terrarian::GetTraction(terrarian->GetType());
    }
};