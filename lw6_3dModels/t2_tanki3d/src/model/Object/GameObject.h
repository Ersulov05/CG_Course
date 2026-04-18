#pragma once
#include "../../common/Geometry.h"

class GameObject
{
public:
    GameObject(const Point3D& position = {0, 0, 0}, const Size3D& size = {1, 1, 1}, const Quaternion3D& rotation = Quaternion3D())
        : m_position(position), m_size(size), m_rotation(rotation) {}

    virtual Point3D GetPosition() const 
    {
        return m_position;
    }

    virtual Quaternion3D GetRotation() const 
    {
        return m_rotation;
    }

    virtual Size3D GetSize() const 
    {
        return m_size;
    }

    virtual void SetPosition(const Point3D& position) 
    {
        m_position = position;
    }

    virtual void SetRotation(const Quaternion3D& rotation) 
    {
        m_rotation = rotation;
    }

    virtual void SetSize(const Size3D& size) 
    {
        m_size = size;
    }
protected:
    Point3D m_position = {0, 0, 0};
    Quaternion3D m_rotation;
    Size3D m_size = {1, 1, 1};
};