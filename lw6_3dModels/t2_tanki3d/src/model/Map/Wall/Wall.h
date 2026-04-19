#pragma once
#include "../../../common/Geometry.h"
#include "../../Constants.h"
#include "../../Object/GameObject.h"
#include <unordered_set>

enum class WallType {
    Brick,
    Steel,
    Empty
};

inline static const std::unordered_set<WallType> UNVURNERABLE_WALLS = {WallType::Empty, WallType::Steel};

inline static bool IsVulnerableWall(WallType type)
{
    return UNVURNERABLE_WALLS.find(type) == UNVURNERABLE_WALLS.end();
}

class Wall : public GameObject {
public:
    Wall(WallType type, const Point3D& position, const Size3D size = Constants::DEFAULT_WALL_SIZE)
        : GameObject(position, size)
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
        if (m_health > 0 && IsVulnerableWall(m_type)) {
            --m_health;
        }
    }

private:
    WallType m_type;
    unsigned int m_health = 1;
};
