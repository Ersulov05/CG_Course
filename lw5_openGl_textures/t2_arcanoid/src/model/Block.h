#pragma once
#include "../common/Geometry.h"
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <vector>

enum class BlockColorType {
    RED,
    GREEN,
    ORANGE,
    VIOLET,
    YELLOW,
    LIGHT_BLUE,
    DARK_BLUE,
};

class Block
{
public:
    Block(Point3D position, Size3D size, BlockColorType colorType = BlockColorType::RED)
        : m_position(position), m_size(size), m_colorType(colorType)
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

    BlockColorType GetColorType() const
    {
        return m_colorType;
    }

private:
    Point3D m_position = {0, 0, 0};
    Size3D m_size = {0, 0, 0};
    BlockColorType m_colorType;
};