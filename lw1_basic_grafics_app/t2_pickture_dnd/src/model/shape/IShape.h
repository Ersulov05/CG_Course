#pragma once

#include <vector>
#include "../common/Color.h"
#include "../common/Point.h"

class IShape
{
public:
    virtual ~IShape() = default;
    virtual void Move(float dx, float dy) = 0;
    virtual unsigned GetId() const = 0;
    virtual std::vector<Point> GetPoints() const = 0;
    virtual Color GetFillColor() const = 0;
    virtual Color GetOutlineColor() const = 0;
};