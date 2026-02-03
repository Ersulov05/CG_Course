#pragma once

#include "../../Common/Point.h"

class IMovementStrategy
{
public:
    virtual ~IMovementStrategy() = default;
    virtual void Update(float deltaTime) = 0;
    virtual const Point GetPosition() const = 0;
};