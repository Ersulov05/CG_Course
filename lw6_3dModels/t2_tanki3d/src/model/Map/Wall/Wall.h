#pragma once
#include "../../../common/Geometry.h"
#include "../../Constants.h"

enum class WallType {
    Brick,
    Steel
};

class Wall {
public:
    Wall(WallType type, const Point3D& position)
        : m_type(type), m_position(position) {}

    Point3D GetPosition() const
    {
        return m_position;
    }

    Size3D GetSize() const
    {
        return m_size;
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
    Point3D m_position;
    Size3D m_size = Constants::DEFAULT_WALL_SIZE;
    WallType m_type;
    unsigned int m_health = 1;
};
