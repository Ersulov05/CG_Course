#pragma once
#include <memory>
#include "../../common/Geometry.h"
#include <iostream>
#include "../Constants.h"

enum class ShellType {
    BB,
    OF
};

class Tank;

class Shell {
public:
    Shell(
        const Point3D& position, 
        const Size3D& size,
        const Quaternion3D rotation, 
        unsigned int damage, 
        float speed
    )
        : m_position(position),
         m_size(size),
         m_rotation(rotation),
         m_damage(damage),
         m_speed(speed)
    {
    }

    void Update(float deltatime) 
    {
        if (m_livetime > 0) {
            m_livetime -= deltatime;
            m_position -= m_rotation.GetForward() * m_speed * deltatime;
        }   
    }

    Point3D GetPosition() const 
    {
        return m_position;
    }

    Quaternion3D GetRotation() const 
    {
        return m_rotation;
    }

    float GetDamage() const 
    {
        return m_damage;
    }

    const std::weak_ptr<Tank> GetOwner() const 
    {
        return m_owner;
    }

    bool IsAlive() const
    {
        return m_livetime > 0;
    }

    void SetOwner(std::weak_ptr<Tank> owner)
    {
        m_owner = owner;
    }

    Size3D GetSize() const
    {
        return m_size;
    }

    void Boom()
    {
        m_livetime = 0;
    }

private:
    Point3D m_position;
    Quaternion3D m_rotation;
    Size3D m_size;
    float m_livetime = 5;
    float m_damage;
    float m_speed;
    std::weak_ptr<Tank> m_owner;
};