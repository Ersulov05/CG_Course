#pragma once
#include "../../../common/Geometry.h"
#include "../../Constants.h"
#include "../../Object/GameObject.h"

enum class WallType {
    Brick,
    Steel
};

class Wall : public GameObject {
public:
    Wall(WallType type, const Point3D& position)
        : GameObject(position, Constants::DEFAULT_WALL_SIZE)
        , m_type(type) 
    {
    }

    WallType GetType() const
    {
        return m_type;
    }

    unsigned int GetHealth() const
    {
        return m_health;
    }

    void TakeDamage()
    {
        if (m_health > 0) {
            --m_health;
        }
    }

private:
    WallType m_type;
    unsigned int m_health = 1;
};
