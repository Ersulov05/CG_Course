#pragma once
#include "../common/Geometry.h"
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <vector>

class Block
{
public:
    Block() {}
    Block(Point3D position, Size3D size)
        : m_position(position), m_size(size)
    {
    }

    Point3D GetPosition() const
    {
        return m_position;
    }

    Size3D GetSize() const
    {
        return m_size;
    }

private:
    Point3D m_position = {0, 0, 0};
    Size3D m_size = {0, 0, 0};
};