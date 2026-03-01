#pragma once
#include "./ICollisionObject.h"
#include "../../common/Point.h"

class CircleCollision : public ICollisionObject
{
public:
    CircleCollision(Point position = {0, 0}, float radius = 0)
        : m_position(position), m_radius(radius)
    {
    }

    CollisionType GetType() const override { return CollisionType::CIRCLE; }
    Point GetPosition() const override { return m_position; }
    float GetRotation() const override { return 0.0f; }
    float GetRadius() const override { return m_radius; }

    void SetPosition(const Point &pos) { m_position = pos; }
    void SetRadius(float r) { m_radius = r; }

private:
    Point m_position;
    float m_radius;
};
