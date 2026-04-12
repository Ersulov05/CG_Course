#pragma once
#include "../common/Geometry.h"
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <vector>
#include "./Constants.h"

enum class Direction {
    Left,
    Right
};

class Racket
{
public:
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

    void SetPosition(const Point3D& position) 
    {
        m_position = position;
    }

    void SetSize(const Size3D& size)
    {
        m_size = size;
    }

    void Reset()
    {
        m_size = DEFAULT_RACKET_SIZE;
        m_position = {0, 0, -DEFAULT_RACKET_SIZE.depth/2};
    }

private:
    Point3D m_position = {0, 0, -DEFAULT_RACKET_SIZE.depth/2};
    Size3D m_size = DEFAULT_RACKET_SIZE;
    float m_speed = DEFAULT_RACKET_SPEED;
};