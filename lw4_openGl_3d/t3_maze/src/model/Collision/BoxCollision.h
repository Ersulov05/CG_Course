#pragma once
#include "../../common/Geometry.h"

class BoxCollision
{
public:
    BoxCollision(Point3D position, Size3D size, Vector3D rotation)
        : m_position(position), m_size(size), m_rotation(rotation)
    {
    }

    Point3D GetPosition() const { return m_position; }
    Vector3D GetRotation() const { return m_rotation; }
    Size3D GetSize() const { return m_size; }

    void SetPosition(const Point3D &position) { m_position = position; }
    void SetRotation(Vector3D rotation) { m_rotation = rotation; }

private:
    Point3D m_position;
    Size3D m_size;
    Vector3D m_rotation;
};