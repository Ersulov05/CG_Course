#pragma once
#include "../../Object/GameObject.h"
#include "../../Constants.h"

class Headquarters : public GameObject {
public:
    Headquarters(const Point3D& position = {0, 0, 0})
        : GameObject(position, Constants::HEADQUARTERS_SIZE)
        , m_totalHealth(Constants::HEADQUARTERS_HEALTH)
        , m_health(Constants::HEADQUARTERS_HEALTH)
    {
    }

    unsigned int GetHealth() const
    {
        return m_health;
    }

    unsigned int GetTotalHealth() const 
    {
        return m_totalHealth;
    }

    void TakeDamage(unsigned int damage) 
    {
        if (damage > m_health) {
            m_health = 0;
        } else {
            m_health -= damage;
        }
    }

private:
    unsigned int m_health;
    unsigned int m_totalHealth;
};