#pragma once
#include <memory>
#include "../../common/Geometry.h"
#include <iostream>
#include "../Constants.h"
#include "../Object/GameObject.h"

enum class ShellType {
    BB,
    OF
};

class Tank;

class Shell : public GameObject {
public:
    Shell(
        const Point3D& position, 
        const Size3D& size,
        const Quaternion3D rotation, 
        unsigned int damage, 
        float speed
    )
        : GameObject(position, size, rotation),
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

    void Boom()
    {
        m_livetime = 0;
    }

private:
    float m_livetime = 5;
    float m_damage;
    float m_speed;
    std::weak_ptr<Tank> m_owner;
};