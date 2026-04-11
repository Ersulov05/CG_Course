#pragma once
#include "../common/Geometry.h"
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <vector>

enum class Direction {
    Left,
    Right
};

class Racket
{
public:
    Racket() {}
    Racket(Point3D position, Size3D size, float speed)
        : m_position(m_position), m_size(size), m_speed(speed) 
    {
    }

    void Move(Direction direction, float deltatime) {
        switch (direction) {                
            case Direction::Left:
                m_position.x -= m_speed * deltatime;
                break;
            
            case Direction::Right:
                m_position.x += m_speed * deltatime;
                break;
        }
    }

    Point3D GetPosition() const
    {
        return m_position;
    }

    Size3D GetSize() const
    {
        return m_size;
    }

    void SetPosition(Point3D position) {
        m_position = position;
    }

private:
    Point3D m_position = {0, 0, -0.05};
    Size3D m_size = {0.5, 0.1, 0.1};
    float m_speed = 2;
};