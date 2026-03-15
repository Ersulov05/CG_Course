#pragma once
#include "../common/Point3D.h"
#include "../common/Size3D.h"
#include "../common/Color.h"

class Wall
{
public:
    Wall(Point3D position, Size3D size, Color color): m_position(position), m_size(size), m_color(color) {

    }

    Point3D GetPosition() const {
        return m_position;
    }

    Size3D GetSize() const {
        return m_size;
    }

    Color GetColor() const {
        return m_color;
    }
private:
    Point3D m_position;
    Size3D m_size;
    Color m_color;
};