#pragma once

#include "./Shape.h"
#include "../common/Point.h"
#include "../common/Color.h"

class Rectangle : public Shape
{
public:
    Rectangle(unsigned shapeId, Point position, float width, float height, Color fillColor, Color outlineColor)
        : Shape(shapeId, fillColor, outlineColor),
          m_position(position),
          m_width(width),
          m_height(height)
    {
    }

    std::vector<Point> GetPoints() const override
    {
        return std::vector<Point>{};
    }

    void Move(float dx, float dy) override
    {
        m_position.x += dx;
        m_position.y += dy;
    }

private:
    Point m_position;
    float m_width;
    float m_height;
};