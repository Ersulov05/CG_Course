#pragma once
#include "./ICollisionObject.h"
#include "../../common/Point.h"

class RectCollision : public ICollisionObject
{
public:
    RectCollision(Point position, float width, float height, float rotation = 0.0f)
        : m_position(position), m_width(width), m_height(height), m_rotation(rotation)
    {
    }

    CollisionType GetType() const override { return CollisionType::RECT; }
    Point GetPosition() const override { return m_position; }
    float GetRotation() const override { return m_rotation; }
    float GetWidth() const override { return m_width; }
    float GetHeight() const override { return m_height; }

    void SetPosition(const Point &pos) { m_position = pos; }
    void SetRotation(float rotation) { m_rotation = rotation; }

private:
    Point m_position;
    float m_width;
    float m_height;
    float m_rotation;
};