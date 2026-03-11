#pragma once

enum class CollisionType
{
    CIRCLE,
    RECT,
};

struct CollisionData
{
    CollisionType type;
};

class ICollisionObject
{
public:
    ~ICollisionObject() = default;
    virtual CollisionType GetType() const = 0;
    virtual Point GetPosition() const = 0;
    virtual float GetRotation() const = 0;

    virtual float GetRadius() const { return 0.0f; }
    virtual float GetWidth() const { return 0.0f; }
    virtual float GetHeight() const { return 0.0f; }
};